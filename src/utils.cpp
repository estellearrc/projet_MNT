
#include "utils.h"
#include "delaunator.hpp"

using namespace std;

void read_data_file(const char *file_path, map<pair<float, float>, float> &elevations, vector<double> &coords, float &min_elevation, float &max_elevation, float &xmin, float &xmax, float &ymin, float &ymax)
{
    ifstream in_file(file_path);
    if (!in_file.is_open())
        cout << "Impossible d'ouvrir le fichier en lecture" << endl;
    else
    {
        string s;
        PJ_CONTEXT *C = PJ_DEFAULT_CTX;
        //wgs84 geocentric to lambert93 projection
        PJ *P = proj_create_crs_to_crs(C, "EPSG:4326", "EPSG:2154", NULL);
        //using the traditional GIS order, that is longitude, latitude (followed by elevation, time)
        PJ *P_for_GIS = proj_normalize_for_visualization(C, P);
        proj_destroy(P);
        P = P_for_GIS;
        PJ_COORD coord_wgs84, coord_lambert93;
        float longitude, latitude, elevation;
        float x, y;
        bool first_data = true;
        while (!in_file.eof())
        {
            //read a line
            getline(in_file, s) >> latitude >> longitude >> elevation;
            coord_wgs84 = proj_coord(longitude, latitude, elevation, 0);
            //coordinates projection
            coord_lambert93 = proj_trans(P, PJ_FWD, coord_wgs84);
            x = coord_lambert93.enu.e; //easting
            y = coord_lambert93.enu.n; //northing
            //save data into objects
            coords.push_back(x);
            coords.push_back(y);
            elevations[make_pair(x, y)] = elevation;

            //init useful extrema
            if (first_data)
            {
                min_elevation = elevation;
                max_elevation = elevation;
                xmin = x;
                xmax = x;
                ymin = y;
                ymax = y;
                first_data = false;
            }
            else //update useful extrema
            {
                if (elevation < min_elevation)
                    min_elevation = elevation;
                else
                {
                    if (elevation > max_elevation)
                        max_elevation = elevation;
                }
                if (x < xmin)
                    xmin = x;
                else
                {
                    if (x > xmax)
                        xmax = x;
                }
                if (y < ymin)
                    ymin = y;
                else
                {
                    if (y > ymax)
                        ymax = y;
                }
            }
        }
        proj_destroy(P);
        in_file.close();
    }
}
void convert_raw_data_to_pixels(const std::map<std::pair<float, float>, float> &elevations, Image &image, const float min_elevation, const float max_elevation, const float xmin, const float xmax, const float ymin, const float ymax)
{
    const int height = image.get_height();
    const int width = image.get_width();
    for (auto it = elevations.begin(); it != elevations.end(); ++it)
    {
        //retrieve field coordinates
        int x = it->first.first;
        int y = it->first.second;
        float elevation = it->second;
        //compute corresponding picture indexes
        int j = (int)((x - xmin) * (width - 1) / (xmax - xmin));
        int i = (int)((y - ymin) * (height - 1) / (ymax - ymin));
        Pixel *p = image.get_pixel(i, j);
        //set pixel's intensity
        if (image.is_gray())
            p->set_gray_intensity(elevation, min_elevation, max_elevation);
        else
            p->set_RGB(elevation, min_elevation, max_elevation);
        //set pixel's theoretical projected position
        p->set_x_y(i, j, width, height, xmin, xmax, ymin, ymax);
    }
}

void write_image_file(const Image &image, string file_name)
{
    const int height = image.get_height();
    const int width = image.get_width();
    const int magic_number = image.get_magic_number();
    string file_extension;
    //determine right image extension
    switch (magic_number)
    {
    case 5:
        file_extension = "_gray_bin.pgm";
        break;
    case 2:
        file_extension = "_gray_ascii.pgm";
        break;
    case 6:
        file_extension = "_color_bin.ppm";
        break;
    case 3:
        file_extension = "_color_ascii.ppm";
        break;
    }
    //file headers
    ofstream ofile;
    ofile.open(file_name + file_extension);
    ofile << "P" << magic_number << "\n"
          << width << " " << height << "\n"
          << 255 << "\n";

    //write pixels' data
    const map<pair<int, int>, Pixel> *pixels = image.get_pixels();
    for (auto it = (*pixels).begin(); it != (*pixels).end(); ++it)
    {
        if (image.is_gray()) //image is in grayscale
        {
            int intensity = it->second.get_R();
            if (image.is_binary())
                ofile << (uint8_t)intensity; //unsigned int on 8 bits from 0 to 255
            else
                ofile << intensity << " ";
        }
        else //image is in color
        {
            int R = it->second.get_R();
            int G = it->second.get_G();
            int B = it->second.get_B();
            if (image.is_binary())
                ofile << (uint8_t)R << (uint8_t)G << (uint8_t)B; //unsigned int on 8 bits from 0 to 255
            else
                ofile << " " << R << " " << G << " " << B << " ";
        }
    }

    ofile.close();
}