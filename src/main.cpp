// #include <cstdlib>
// #include <iostream>
// #include <fstream>
// #include <string>
// #include <proj.h>
// #include <opencv2/opencv.hpp>
// #include "delaunator.hpp"
// #include <cstdio>
// #include <vector>
// #include <map>
#include "utils.h"

using namespace std;

// void read_text_file(const char *file_name, const int width)
// {
//     map<pair<float, float>, float> elevations;
//     map<pair<int, int>, int> pixels;
//     vector<double> coords;
//     float min_elevation, max_elevation;
//     float xmin, xmax;
//     float ymin, ymax;
//     int imin, imax, jmin, jmax;
//     ifstream in_file(file_name);
//     // int nb_data = 1048576;

//     if (!in_file.is_open())
//         cout << "Impossible d'ouvrir le fichier en lecture" << endl;

//     else
//     {
//         string s;
//         PJ_CONTEXT *C = PJ_DEFAULT_CTX;
//         // PJ *P = proj_create(C, "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000"); //lambert93 projection
//         PJ *P = proj_create_crs_to_crs(C, "EPSG:4326", "EPSG:2154", NULL); //wgs84 geocentric to lambert93 projection
//         PJ *P_for_GIS = proj_normalize_for_visualization(C, P);            //using the traditional GIS order, that is longitude, latitude (followed by elevation, time)
//         proj_destroy(P);
//         P = P_for_GIS;
//         PJ_COORD coord_wgs84, coord_lambert93;
//         float longitude, latitude, elevation;
//         float x, y;
//         bool first_data = true;
//         while (!in_file.eof())
//         {
//             getline(in_file, s) >> latitude >> longitude >> elevation;
//             coord_wgs84 = proj_coord(longitude, latitude, elevation, 0);
//             coord_lambert93 = proj_trans(P, PJ_FWD, coord_wgs84);
//             // printf("easting: %.3f, northing: %.3f , elevation: %g\n", coord_lambert93.enu.e, coord_lambert93.enu.n, coord_lambert93.lpz.z);
//             x = coord_lambert93.enu.e;
//             y = coord_lambert93.enu.n;
//             // coord_lambert93 = proj_trans(P, PJ_INV, coord_lambert93);
//             // printf("longitude: %g, latitude: %g, elevation: %g\n", coord_lambert93.lpz.lam, coord_lambert93.lpz.phi, coord_lambert93.lpz.z);
//             coords.push_back(x);
//             coords.push_back(y);
//             // elevations.insert(make_pair(make_pair(x, y), elevation));

//             elevations[make_pair(x, y)] = elevation;
//             //update elevation range
//             if (first_data)
//             {
//                 min_elevation = elevation;
//                 max_elevation = elevation;
//                 xmin = x;
//                 xmax = x;
//                 ymin = y;
//                 ymax = y;
//                 first_data = false;
//             }
//             else
//             {
//                 if (elevation < min_elevation)
//                     min_elevation = elevation;
//                 else
//                 {
//                     if (elevation > max_elevation)
//                         max_elevation = elevation;
//                 }
//                 if (x < xmin)
//                     xmin = x;
//                 else
//                 {
//                     if (x > xmax)
//                         xmax = x;
//                 }
//                 if (y < ymin)
//                     ymin = y;
//                 else
//                 {
//                     if (y > ymax)
//                         ymax = y;
//                 }
//             }
//         }
//         proj_destroy(P);
//         in_file.close();

//         // // triangulation happens here
//         // delaunator::Delaunator d(coords);
//         // cout << "nb triangles found = " << d.triangles.size() << endl;
//         // for (std::size_t i = 0; i < d.triangles.size(); i += 3)
//         // {
//         //     // printf(
//         //     //     "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
//         //     //     d.coords[2 * d.triangles[i]],         //tx0
//         //     //     d.coords[2 * d.triangles[i] + 1],     //ty0
//         //     //     d.coords[2 * d.triangles[i + 1]],     //tx1
//         //     //     d.coords[2 * d.triangles[i + 1] + 1], //ty1
//         //     //     d.coords[2 * d.triangles[i + 2]],     //tx2
//         //     //     d.coords[2 * d.triangles[i + 2] + 1]  //ty2
//         //     // );
//         //     float x0 = d.coords[2 * d.triangles[i]];
//         //     float y0 = d.coords[2 * d.triangles[i] + 1];
//         //     float x1 = d.coords[2 * d.triangles[i + 1]];
//         //     float y1 = d.coords[2 * d.triangles[i + 1] + 1];
//         //     float x2 = d.coords[2 * d.triangles[i + 2]];
//         //     float y2 = d.coords[2 * d.triangles[i + 2] + 1];
//         // }

//         cout << "xmin =" << xmin << " xmax=" << xmax << " ymin=" << ymin << " ymax=" << ymax << endl;
//         int height = (int)(abs(xmax - xmin) * width / abs(ymax - ymin));
//         for (int i = 0; i < height; i++)
//         {
//             for (int j = 0; j < width; j++)
//             {
//                 pixels[make_pair(-i, j)] = 0;
//             }
//         }
//         ofstream ofile;
//         ofile.open("ascii_gray.pgm");
//         ofile << "P2\n"
//               << width << " " << height << "\n255\n";
//         cout << "height=" << height << endl;

//         imin = (int)height / 2;
//         imax = (int)height / 2;
//         jmin = (int)width / 2;
//         jmax = (int)width / 2;
//         // for (auto it = elevations.begin(); it != elevations.end(); ++it)
//         cout << coords.size() << endl;
//         // int k = 0;
//         // k < coords.size();
//         // k += 2;

//         for (auto it = elevations.begin(); it != elevations.end(); ++it)
//         {
//             // cout << "x=" << it->first.first << " y=" << it->first.second << endl;
//             int x = it->first.first;
//             int y = it->first.second;
//             float elevation = it->second;
//             float rescaled_elevation = 0;
//             if (elevation != 0)
//                 rescaled_elevation = (elevation - min_elevation) / (max_elevation - min_elevation); //elevation normalization
//             int j = (int)((x - xmin) * (width - 1) / (xmax - xmin));
//             int i = (int)((y - ymin) * (height - 1) / (ymax - ymin));
//             if (j < jmin)
//                 jmin = j;
//             else
//             {
//                 if (j > jmax)
//                     jmax = j;
//             }
//             if (i < imin)
//                 imin = i;
//             else
//             {
//                 if (i > imax)
//                     imax = i;
//             }
//             int intensity = (int)(255 * rescaled_elevation);
//             pixels[make_pair(-i, j)] = intensity;
//             cout << "x=" << x << " y=" << y << " j=" << j << " i=" << i << " z=" << elevation << " r_z=" << rescaled_elevation << endl;
//         }
//         cout << "imin = " << imin << " imax = " << imax << " jmin = " << jmin << " jmax = " << jmax << endl;

//         int cpt = 1;
//         for (auto it = pixels.begin(); it != pixels.end(); ++it)
//         {
//             cout << "i=" << it->first.first << " j=" << it->first.second << endl;
//             int intensity = it->second;
//             ofile << intensity << " ";
//             if (cpt % 15 == 0)
//                 ofile << "\n";
//             cpt++;
//         }

//         ofile.close();
//     }
// }

void process(const char *file_path, const int image_width)
{
    //./create-raster ../doc/Guerledan_Feb19_50cm_wgs84.txt 800
    // read_text_file(file_path, image_width);
    map<pair<float, float>, float> elevations;
    vector<double> coords;
    float min_elevation, max_elevation;
    float xmin, xmax;
    float ymin, ymax;
    read_data_file(file_path, elevations, coords, min_elevation, max_elevation, xmin, xmax, ymin, ymax);
    Image image = Image(image_width, xmin, xmax, ymin, ymax, false, false);
    convert_raw_data_to_pixels(elevations, image, min_elevation, max_elevation, xmin, xmax, ymin, ymax);
    write_image_file(image, "test");
}

void usage(const char *s)
{
    std::cerr << "Usage: " << s << " file_path image_width\n"
              << std::endl;
    exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char *argv[])
{

    if (argc != (param + 1))
        usage(argv[0]);
    process(argv[1], atoi(argv[2]));

    return EXIT_SUCCESS;
}