#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <proj.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat read_text_file(const char *file_name, const int width)
{
    cout << "hello" << endl;
    float *elevations[10000][width];
    Mat image_dest;
    ifstream f(file_name);

    if (!f.is_open())
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
        bool first_data = true;
        while (!f.eof())
        {
            getline(f, s) >> latitude >> longitude >> elevation;
            coord_wgs84 = proj_coord(longitude, latitude, elevation, 0);
            coord_lambert93 = proj_trans(P, PJ_FWD, coord_wgs84);
            // printf("easting: %.3f, northing: %.3f , elevation: %g\n", coord_lambert93.enu.e, coord_lambert93.enu.n, coord_lambert93.lpz.z);
            // coord_lambert93 = proj_trans(P, PJ_INV, coord_lambert93);
            // printf("longitude: %g, latitude: %g, elevation: %g\n", coord_lambert93.lpz.lam, coord_lambert93.lpz.phi, coord_lambert93.lpz.z);

            //update elevation range
            if (first_data)
            {
                min_elevation = elevation;
                max_elevation = elevation;
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
            }
            cout << nb_lines << " " << nb_columns << endl;
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
        // cout << nb_lines << " " << nb_columns << endl;
        // cout << min_elevation << " " << max_elevation << endl;
        image_dest = Mat(nb_lines + 1, width, CV_8UC1);
        // for (int i = 0; i < nb_lines; i++)
        // {
        //     for (int j = 0; j < nb_columns; j++)
        //     {
        //         float rescaled_elevation = (elevations[i][j] - min_elevation) / (max_elevation - min_elevation); //elevation normalization
        //         // cout << rescaled_elevation << endl;
        //         image_dest.ptr<uchar>(i)[j] = (int)(255 * rescaled_elevation);
        //     }
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