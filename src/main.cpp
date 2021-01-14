#include "utils.h"

using namespace std;

void create_raster(const char *file_path, const int image_width, bool triangulation, bool gray, bool binary)
{
    cout << "\n\n======================================= GENERATING RASTER ======================================= " << endl;
    vector<double> coords;
    MNT raster = read_data(file_path, coords, image_width, triangulation, gray, binary);
    if (raster.triangulation) //with Delaunay's triangulation
        convert_data_to_pixels_delaunay(coords, raster);
    else //without Delaunay's triangulation
        convert_data_to_pixels(raster);
    write_image(raster.image, "MNT");
    cout << "=========================================== RASTER DONE =========================================== " << endl;
}

void process(const char *file_path, const int image_width)
{
    //ASCII grayscale
    create_raster(file_path, image_width, true, true, false);
    //Binary grayscale
    create_raster(file_path, image_width, true, true, true);
    //ASCII color
    create_raster(file_path, image_width, true, false, false);
    //Binary color
    create_raster(file_path, image_width, true, false, true);
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