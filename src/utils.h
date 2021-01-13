#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <proj.h>
#include <cstdio>
#include <vector>
#include <map>
#include <bitset>
#include "Image.h"

struct MNT
{
    float xmin = 0, xmax = 0, ymin = 0, ymax = 0, min_elevation = 0, max_elevation = 0;
    bool triangulation = false;
    std::map<std::pair<float, float>, float> elevations;
    Image image;
};

MNT read_data(const char *file_path, std::vector<double> &coords, const int image_width, bool triangulation, bool is_gray, bool is_binary);
void write_image(const Image &image, std::string file_name);
void convert_data_to_pixels(MNT &raster);
void convert_data_to_pixels_delaunay(std::vector<double> &coords, MNT &raster);
void associate_triangle_summits_to_pixels(MNT &raster, const std::vector<size_t> &triangles, const std::vector<double> &coords);

#endif
