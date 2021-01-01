#include "utils.h"

using namespace std;

void process(const char *file_path, const int image_width)
{
    //./create-raster ../doc/Guerledan_Feb19_50cm_wgs84.txt 800
    map<pair<float, float>, float> elevations;
    vector<double> coords;
    float min_elevation, max_elevation;
    float xmin, xmax;
    float ymin, ymax;
    read_data_file(file_path, elevations, coords, min_elevation, max_elevation, xmin, xmax, ymin, ymax);
    Image image = Image(image_width, xmin, xmax, ymin, ymax, true, false);
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