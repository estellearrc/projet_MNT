
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
        double longitude, latitude, elevation;
        double x, y;
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
            // cout << "x=" << x << " y=" << y << endl;
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

float dist(const float ax, const float ay, const float bx, const float by)
{
    const float dx = ax - bx;
    const float dy = ay - by;
    return sqrtf(dx * dx + dy * dy);
}
void convert_raw_data_to_pixels_delaunay(const map<pair<float, float>, float> &elevations, vector<double> &coords, Image &image, const float min_elevation, const float max_elevation, const float xmin, const float xmax, const float ymin, const float ymax)
{
    const int height = image.get_height();
    const int width = image.get_width();
    delaunator::Delaunator d(coords); //Delaunay's triangulation
    float dist_min = 99;
    float x0, y0, x1, y1, x2, y2; //triangle's summits
    //look for the corresponding triangle
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Pixel *p = image.get_pixel(i, j);
            float x = p->get_x();
            float y = p->get_y();
            // cout << "x=" << x << " y=" << y << endl;

            for (std::size_t i = 0; i < d.triangles.size(); i += 3)
            {
                float tx0 = d.coords[2 * d.triangles[i]];         //tx0
                float ty0 = d.coords[2 * d.triangles[i] + 1];     //ty0
                float tx1 = d.coords[2 * d.triangles[i + 1]];     //tx1
                float ty1 = d.coords[2 * d.triangles[i + 1] + 1]; //ty1
                float tx2 = d.coords[2 * d.triangles[i + 2]];     //tx2
                float ty2 = d.coords[2 * d.triangles[i + 2] + 1]; //ty2
                float dist_to_summits = dist(tx0, ty0, x, y) + dist(tx1, ty1, x, y) + dist(tx2, ty2, x, y);
                if (dist_to_summits < dist_min)
                {
                    x0 = tx0;
                    y0 = ty0;
                    x1 = tx1;
                    y1 = ty1;
                    x2 = tx2;
                    y2 = ty2;
                    dist_min = dist_to_summits;
                }
            }
        }
    }
}

void find_triangle() //to do
{
}

void convert_raw_data_to_pixels(const map<pair<float, float>, float> &elevations, vector<double> &coords, Image &image, const float min_elevation, const float max_elevation, const float xmin, const float xmax, const float ymin, const float ymax)
{
    const int height = image.get_height();
    const int width = image.get_width();
    for (auto it = elevations.begin(); it != elevations.end(); ++it)
    {
        //retrieve field coordinates
        float x = it->first.first;
        float y = it->first.second;
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
        // p->set_x_y(i, j, width, height, xmin, xmax, ymin, ymax);
        printf("xth = %f y = %f", p->get_x(), p->get_y());
    }
}

void interpolate_pixel_intensity(const Image &image, vector<double> &coords, const float min_elevation, const float max_elevation, const float xmin, const float xmax, const float ymin, const float ymax)
{
    //triangulation happens here
    delaunator::Delaunator d(coords);
    // cout << "points (x,y) triangle = " << d.triangles.size() << " points (x,y) coords = " << coords.size() << endl;
    // int start_index = d.hull_start;
    // cout << "start index = " << start_index << endl;
    cout << xmin << " " << ymin << endl;
    cout << "First coordinates x=" << coords[0] << " y=" << coords[0 + 1] << endl;
    cout << "First coordinates x=" << d.coords[0] << " y=" << d.coords[0 + 1] << endl;
    cout << "First triangle x=" << d.triangles[0] << endl;
    int i = 0;
    printf(
        "First triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
        d.coords[2 * d.triangles[i]],         //tx0
        d.coords[2 * d.triangles[i] + 1],     //ty0
        d.coords[2 * d.triangles[i + 1]],     //tx1
        d.coords[2 * d.triangles[i + 1] + 1], //ty1
        d.coords[2 * d.triangles[i + 2]],     //tx2
        d.coords[2 * d.triangles[i + 2] + 1]  //ty2
    );

    for (std::size_t i = 0; i < d.triangles.size(); i += 3)
    {
        float x0, y0, x1, y1, x2, y2;              //triangle's summits
        x0 = d.coords[2 * d.triangles[i]];         //tx0
        y0 = d.coords[2 * d.triangles[i] + 1];     //ty0
        x1 = d.coords[2 * d.triangles[i + 1]];     //tx1
        y1 = d.coords[2 * d.triangles[i + 1] + 1]; //ty1
        x2 = d.coords[2 * d.triangles[i + 2]];     //tx2
        y2 = d.coords[2 * d.triangles[i + 2] + 1]; //ty2
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
                ofile << (uint8_t)intensity; //unsigned int on 8 bits (from 0 to 255)
            else
                ofile << intensity << " ";
        }
        else //image is in color
        {
            int R = it->second.get_R();
            int G = it->second.get_G();
            int B = it->second.get_B();
            if (image.is_binary())
                ofile << (uint8_t)R << (uint8_t)G << (uint8_t)B; //unsigned int on 8 bits (from 0 to 255)
            else
                ofile << " " << R << " " << G << " " << B << " ";
        }
    }

    ofile.close();
}