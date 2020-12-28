#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <proj.h>
#include <opencv2/opencv.hpp>
#include "delaunator.hpp"
#include <cstdio>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

// void writePGM(const Image &image, std::string fname)
// {
//     ofstream ofile;
//     ofile.open(fname + ".pgm");

//     ofile << "P2\n"
//           << image.dim(0) << " " << image.dim(1) << "\n255\n";
//     int c = 0;
//     for (int i = 0; i < image.dim(0); ++i)
//     {
//         for (int j = 0; j < image.dim(1); ++j)
//         {
//             ofile << (int)(image.data[c++]) << " ";
//         }
//         ofile << "\n";
//     }

//     ofile.close();
// }

// /* x0, y0, x1, y1, ... */
//     std::vector<double> coords = {-1, 1, 1, 1, 1, -1, -1, -1};

//     //triangulation happens here
//     delaunator::Delaunator d(coords);

//     for (std::size_t i = 0; i < d.triangles.size(); i += 3)
//     {
//         printf(
//             "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
//             d.coords[2 * d.triangles[i]],         //tx0
//             d.coords[2 * d.triangles[i] + 1],     //ty0
//             d.coords[2 * d.triangles[i + 1]],     //tx1
//             d.coords[2 * d.triangles[i + 1] + 1], //ty1
//             d.coords[2 * d.triangles[i + 2]],     //tx2
//             d.coords[2 * d.triangles[i + 2] + 1]  //ty2
//         );
//     }

Mat read_text_file(const char *file_name, const int width)
{
    map<pair<float, float>, float> elevations;
    vector<double> coords;
    Mat image_dest;
    ifstream in_file(file_name);
    // int nb_data = 1048576;

    if (!in_file.is_open())
        cout << "Impossible d'ouvrir le fichier en lecture" << endl;

    else
    {
        string s;
        PJ_CONTEXT *C = PJ_DEFAULT_CTX;
        // PJ *P = proj_create(C, "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000"); //lambert93 projection
        PJ *P = proj_create_crs_to_crs(C, "EPSG:4326", "EPSG:2154", NULL); //wgs84 geocentric to lambert93 projection
        PJ *P_for_GIS = proj_normalize_for_visualization(C, P);            //using the traditional GIS order, that is longitude, latitude (followed by elevation, time)
        proj_destroy(P);
        P = P_for_GIS;
        PJ_COORD coord_wgs84, coord_lambert93;
        float longitude, latitude, elevation;
        int nb_lines = 0;
        int nb_columns = 0;
        float min_elevation, max_elevation;
        float min_lat, max_lat;
        float min_long, max_long;
        bool first_data = true;
        while (!in_file.eof())
        {
            getline(in_file, s) >> latitude >> longitude >> elevation;
            coord_wgs84 = proj_coord(longitude, latitude, elevation, 0);
            coord_lambert93 = proj_trans(P, PJ_FWD, coord_wgs84);
            // printf("easting: %.3f, northing: %.3f , elevation: %g\n", coord_lambert93.enu.e, coord_lambert93.enu.n, coord_lambert93.lpz.z);
            // coord_lambert93 = proj_trans(P, PJ_INV, coord_lambert93);
            // printf("longitude: %g, latitude: %g, elevation: %g\n", coord_lambert93.lpz.lam, coord_lambert93.lpz.phi, coord_lambert93.lpz.z);
            coords.push_back(longitude);
            coords.push_back(latitude);
            elevations[make_pair(longitude, latitude)] = elevation;
            //update elevation range
            if (first_data)
            {
                min_elevation = elevation;
                max_elevation = elevation;
                min_lat = latitude;
                max_lat = latitude;
                min_long = longitude;
                max_long = longitude;

                first_data = false;
            }
            else
            {
                if (elevation < min_elevation)
                    min_elevation = elevation;
                else
                {
                    if (elevation > max_elevation)
                        max_elevation = elevation;
                }
                if (latitude < min_lat)
                    min_lat = latitude;
                else
                {
                    if (latitude > max_lat)
                        max_lat = latitude;
                }
                if (longitude < min_long)
                    min_long = longitude;
                else
                {
                    if (longitude > max_long)
                        max_long = longitude;
                }
            }
            // cout << nb_lines << " " << nb_columns << endl;
            // elevations[nb_lines][nb_columns % width] = elevation;
            //update indexes for image size
            nb_columns++;
            if (nb_columns % width == 0)
            {
                nb_lines++;
                nb_columns = 0;
            }
        }
        proj_destroy(P);
        in_file.close();

        ofstream ofile;
        ofile.open("ascii_gray.pgm");
        cout << abs(max_lat - min_lat) << endl;
        cout << abs(max_long - min_long) << endl;
        int height = (int)(abs(max_lat - min_lat) * width / abs(max_long - min_long));
        ofile << "P2\n"
              << width << " " << height << "\n120\n";

        cout << "height=" << height << endl;
        int cpt = 1;
        for (auto it = elevations.begin(); it != elevations.end(); ++it)
        {
            ofile << (int)abs(it->second) << " ";
            if (cpt % 15 == 0)
                ofile << "\n";
            cpt++;
        }
        ofile.close();
        // cout << nb_lines << " " << nb_columns << endl;
        // cout << min_elevation << " " << max_elevation << endl;
        // image_dest = Mat(nb_lines + 1, width, CV_8UC1);
        // for (int i = 0; i < nb_lines; i++)
        // {
        //     for (int j = 0; j < nb_columns; j++)
        //     {
        //         float rescaled_elevation = (elevations[i][j] - min_elevation) / (max_elevation - min_elevation); //elevation normalization
        //         // cout << rescaled_elevation << endl;
        //         image_dest.ptr<uchar>(i)[j] = (int)(255 * rescaled_elevation);
        //     }
        // }
        //triangulation happens here
        delaunator::Delaunator d(coords);
        cout << "nb triangles found = " << d.triangles.size() << endl;
        // for (std::size_t i = 0; i < d.triangles.size(); i += 3)
        // {
        //     printf(
        //         "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
        //         d.coords[2 * d.triangles[i]],         //tx0
        //         d.coords[2 * d.triangles[i] + 1],     //ty0
        //         d.coords[2 * d.triangles[i + 1]],     //tx1
        //         d.coords[2 * d.triangles[i + 1] + 1], //ty1
        //         d.coords[2 * d.triangles[i + 2]],     //tx2
        //         d.coords[2 * d.triangles[i + 2] + 1]  //ty2
        //     );
        // }
    }
    return image_dest;
}

void process(const char *file_name, const int width)
{
    //../doc/Guerledan_Feb19_50cm_wgs84.txt
    Mat image = read_text_file(file_name, width);
    // imwrite(imdname, image);
}

void usage(const char *s)
{
    std::cerr << "Usage: " << s << " file_name width\n"
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