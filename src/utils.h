#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <proj.h>
#include <cstdio>
// #include <stdint.h>
#include <vector>
#include <map>
#include "Image.h"

void read_data_file(const char *file_path, std::map<std::pair<float, float>, float> &elevations, std::vector<double> &coords, float &min_elevation, float &max_elevation, float &xmin, float &xmax, float &ymin, float &ymax);
void write_image_file(const Image &image, std::string file_name);
void convert_raw_data_to_pixels(const std::map<std::pair<float, float>, float> &elevations, Image &image, const float min_elevation, const float max_elevation, const float xmin, const float xmax, const float ymin, const float ymax);
void interpolate_pixel_intensity();
#endif
