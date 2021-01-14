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

/**
 * @file utils.h
 * @brief A file containing useful structure and functions to generate a DTM or MNT in French
 */

/**
 * @struct MNT
 * @brief Digital Terrain Model structure (DTM or MNT in French) composed of:
 * * Some parameters (xmin, xmax, ymin, yman, min_elevation, max_elevation, triangulation)
 * * All the (x,y) coordinates of the sampled terrain and their corresponding elevation
 * * An image representing the sampled terrain
 * 
 * Details about attributes:
 * @param xmin a constant float giving the minimal x coordinate on the sampled terrain
 * @param xmax a constant float giving the maximal x coordinate on the sampled terrain
 * @param ymin a constant float giving the minimal y coordinate on the sampled terrain
 * @param ymax a constant float giving the maximal y coordinate on the sampled terrain
 * @param triangulation a boolean indicating whether Delaunay's is to be performed
 * @param elevations a map of pair of floating coordinates (x,y) associated to the floating altitude of the point on the terrain
 * @param image an Image object representing the Digital Terrain Model (final output of the program)
 */
struct MNT
{
    float xmin = 0, xmax = 0, ymin = 0, ymax = 0, min_elevation = 0, max_elevation = 0;
    bool triangulation = false;
    std::map<std::pair<float, float>, float> elevations;
    Image image;
};

/**
 * @brief Read the raw sampled data of the terrain and init an MNT object.
 * The raw data have 3 coordinates (longitude, latitude, elevation) to be projected - thanks to Lambert 93 projection - into (northing, easting, elevation).
 * They are saved into **coords** vector and an MNT object is created.
 * 
 * 
 * @param file_path a string indicating the full-path or relative one to the text document containing the terrain data to read
 * @param coords a vector of the coordinates (x,y) of the sampled data of the terrain
 * @param image_width an integer chosen by the user to define the resolution if the image output
 * @param triangulation a bool to indicate whether Delaunay's triangulation is to be performed on the (x,y) sampled coordinates of the terrain
 * @param is_gray a bool indicating if the output image should be in grayscale or in color
 * @param is_binary a bool indicating if the output image should be encoded in binary or not
 * @return MNT object
 */

MNT read_data(const char *file_path, std::vector<double> &coords, const int image_width, bool triangulation, bool is_gray, bool is_binary);
/**
 * @brief Write an image file according to its encoding parameters.
 * 
 * @param image an Image object
 * @param file_name a string giving the full path or relative one of the image file and the name of it. 
 * The file extension is determined thanks to the encoding parameters of the Image object.
 */

void write_image(const Image &image, std::string file_name);
/**
 * @brief Convert raw data elevations into RGB components for the image's pixels.
 * 
 * @param raster an MNT object
 */

void convert_data_to_pixels(MNT &raster);
/**
 * @brief Convert raw data elevations into RGB components for the image's pixels after performing Delaunay's triangulation of the points of the terrain.
 * 
 * @param coords a vector of the coordinates (x,y) of the sampled data of the terrain
 * @param raster an MNT object
 */

void convert_data_to_pixels_delaunay(std::vector<double> &coords, MNT &raster);
/**
 * @brief Only applicable if Delaunay's triangulation is performed on the sampled points of the terrain.
 * Associate the possible triangular zones of belonging for each pixel of the image.
 * 
 * @param raster an MNT object
 * @param triangles a vector containing all the summits of the triangles determined by Delaunay's triangulation of the terrain.
 * @param coords a vector of the coordinates (x,y) of the sampled data of the terrain
 */
void associate_triangle_summits_to_pixels(MNT &raster, const std::vector<size_t> &triangles, const std::vector<double> &coords);

#endif
