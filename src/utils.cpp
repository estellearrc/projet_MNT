
#include "utils.h"
#include "delaunator.hpp"

using namespace std;

MNT read_data(const char *file_path, std::vector<double> &coords, const int image_width, bool triangulation, bool is_gray, bool is_binary)
{
    map<pair<float, float>, float> elevations;
    float min_elevation, max_elevation;
    float xmin, xmax;
    float ymin, ymax;
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
    //create raster
    MNT raster;
    raster.xmin = xmin;
    raster.xmax = xmax;
    raster.ymin = ymin;
    raster.ymax = ymax;
    raster.min_elevation = min_elevation;
    raster.max_elevation = max_elevation;
    raster.triangulation = triangulation;
    raster.elevations = elevations;
    raster.image = Image(image_width, xmin, xmax, ymin, ymax, is_gray, is_binary);
    return raster;
}
void associate_triangle_summits_to_pixels(MNT &raster, const vector<size_t> &triangles, const vector<double> &coords)
{
    const int height = raster.image.get_height();
    const int width = raster.image.get_width();
    vector<pair<int, int>> pair_indexes;
    for (std::size_t k = 0; k < triangles.size(); k += 3)
    {
        //triangle's summits
        float x0 = coords[2 * triangles[k]];
        float y0 = coords[2 * triangles[k] + 1];
        float x1 = coords[2 * triangles[k + 1]];
        float y1 = coords[2 * triangles[k + 1] + 1];
        float x2 = coords[2 * triangles[k + 2]];
        float y2 = coords[2 * triangles[k + 2] + 1];
        //compute corresponding picture indexes
        float j0f = (x0 - raster.xmin) * (width - 1) / (raster.xmax - raster.xmin);
        float i0f = (y0 - raster.ymin) * (height - 1) / (raster.ymax - raster.ymin);
        float j1f = (x1 - raster.xmin) * (width - 1) / (raster.xmax - raster.xmin);
        float i1f = (y1 - raster.ymin) * (height - 1) / (raster.ymax - raster.ymin);
        float j2f = (x2 - raster.xmin) * (width - 1) / (raster.xmax - raster.xmin);
        float i2f = (y2 - raster.ymin) * (height - 1) / (raster.ymax - raster.ymin);
        int i0_floor = (int)floorf(i0f), i0_ceil = (int)ceilf(i0f), j0_floor = (int)floorf(j0f), j0_ceil = (int)ceilf(j0f);
        int i1_floor = (int)floorf(i1f), i1_ceil = (int)ceilf(i1f), j1_floor = (int)floorf(j1f), j1_ceil = (int)ceilf(j1f);
        int i2_floor = (int)floorf(i2f), i2_ceil = (int)ceilf(i2f), j2_floor = (int)floorf(j2f), j2_ceil = (int)ceilf(j2f);
        //save possible pixels corresponding to triangle summit 0
        pair_indexes.push_back(make_pair(i0_floor, j0_floor));
        pair_indexes.push_back(make_pair(i0_floor, j0_ceil));
        pair_indexes.push_back(make_pair(i0_ceil, j0_floor));
        pair_indexes.push_back(make_pair(i0_ceil, j0_ceil));
        //save possible pixels corresponding to triangle summit 1
        pair_indexes.push_back(make_pair(i1_floor, j1_floor));
        pair_indexes.push_back(make_pair(i1_floor, j1_ceil));
        pair_indexes.push_back(make_pair(i1_ceil, j1_floor));
        pair_indexes.push_back(make_pair(i1_ceil, j1_ceil));
        //save possible pixels corresponding to triangle summit 2
        pair_indexes.push_back(make_pair(i2_floor, j2_floor));
        pair_indexes.push_back(make_pair(i2_floor, j2_ceil));
        pair_indexes.push_back(make_pair(i2_ceil, j2_floor));
        pair_indexes.push_back(make_pair(i2_ceil, j2_ceil));
        //sort out indexes
        sort(pair_indexes.begin(), pair_indexes.end());
        //remove duplicates
        pair_indexes.erase(unique(pair_indexes.begin(), pair_indexes.end()), pair_indexes.end());
        //add triangles' indexes to pixels
        for (auto it = pair_indexes.begin(); it != pair_indexes.end(); ++it)
        {
            raster.image.get_pixel((*it).first, (*it).second)->add_triangle_index(k);
        }
        //remove all elements in the vector
        pair_indexes.clear();
    }
}

void convert_data_to_pixels_delaunay(std::vector<double> &coords, MNT &raster)
{
    const int height = raster.image.get_height();
    const int width = raster.image.get_width();
    delaunator::Delaunator d(coords); //Delaunay's triangulation
    associate_triangle_summits_to_pixels(raster, d.triangles, d.coords);

    //for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Pixel *p = raster.image.get_pixel(i, j);
            float x = p->get_x();
            float y = p->get_y();
            //look for the corresponding triangle into the possible triangles for the pixel
            const vector<int> *triangles_indexes = p->get_triangles_indexes();
            for (auto it = (*triangles_indexes).begin(); it != (*triangles_indexes).end(); ++it)
            {
                int k = *it;
                //triangle's summits
                float tx0 = d.coords[2 * d.triangles[k]];
                float ty0 = d.coords[2 * d.triangles[k] + 1];
                float tx1 = d.coords[2 * d.triangles[k + 1]];
                float ty1 = d.coords[2 * d.triangles[k + 1] + 1];
                float tx2 = d.coords[2 * d.triangles[k + 2]];
                float ty2 = d.coords[2 * d.triangles[k + 2] + 1];
                //barycenter coefficients
                float alpha = ((ty1 - ty2) * (x - tx2) + (tx2 - tx1) * (y - ty2)) /
                              ((ty1 - ty2) * (tx0 - tx2) + (tx2 - tx1) * (ty0 - ty2));
                float beta = ((ty2 - ty0) * (x - tx2) + (tx0 - tx2) * (y - ty2)) /
                             ((ty1 - ty2) * (tx0 - tx2) + (tx2 - tx1) * (ty0 - ty2));
                float gamma = 1.0f - alpha - beta;
                if (alpha >= 0 && beta >= 0 && alpha <= 1 && beta <= 1)
                {
                    //retrieve correpsonding elevations
                    float elev_0 = raster.elevations[make_pair(tx0, ty0)];
                    float elev_1 = raster.elevations[make_pair(tx1, ty1)];
                    float elev_2 = raster.elevations[make_pair(tx2, ty2)];
                    //interpolate triangle summits' elevations
                    float elevation = alpha * elev_0 + beta * elev_1 + gamma * elev_2;
                    //set pixel's intensity
                    if (raster.image.is_gray())
                        p->set_gray_intensity(elevation, raster.min_elevation, raster.max_elevation);
                    else
                        p->set_RGB(elevation, raster.min_elevation, raster.max_elevation);
                }
            }
        }
    }
}

void convert_data_to_pixels(MNT &raster)
{
    const int height = raster.image.get_height();
    const int width = raster.image.get_width();
    for (auto it = raster.elevations.begin(); it != raster.elevations.end(); ++it)
    {
        //retrieve field coordinates
        float x = it->first.first;
        float y = it->first.second;
        float elevation = it->second;
        //compute corresponding picture indexes
        int j = (int)((x - raster.xmin) * (width - 1) / (raster.xmax - raster.xmin));
        int i = (int)((y - raster.ymin) * (height - 1) / (raster.ymax - raster.ymin));
        Pixel *p = raster.image.get_pixel(i, j);
        //set pixel's intensity
        if (raster.image.is_gray())
            p->set_gray_intensity(elevation, raster.min_elevation, raster.max_elevation);
        else
            p->set_RGB(elevation, raster.min_elevation, raster.max_elevation);
    }
}

void write_image(const Image &image, string file_name)
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
    ofile.open("../images/" + file_name + file_extension);
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