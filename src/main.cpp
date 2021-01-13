#include "utils.h"

using namespace std;

void process(const char *file_path, const int image_width)
{
    bool triangulation = true;
    bool gray = false;
    bool binary = true;
    vector<double> coords;
    //create raster
    MNT raster = read_data(file_path, coords, image_width, triangulation, gray, binary);
    if (raster.triangulation) //with Delaunay's triangulation
        convert_data_to_pixels_delaunay(coords, raster);
    else //without Delaunay's triangulation
        convert_data_to_pixels(raster);
    write_image(raster.image, "MNT");
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