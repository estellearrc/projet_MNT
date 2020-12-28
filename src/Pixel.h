#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <cmath>
#include <fstream>

class Pixel
{
public:
    Pixel(double longitude, double latitude, int i, int j, int R, int G, int B);
    Pixel(double longitude, double latitude, int i, int j, int gray_intensity);
    Pixel(double longitude, double latitude, int i, int j, double elevation);
    ~Pixel();

private:
    double long_shit = 6.709 * std::pow(10, -6); //theorerical shift in longitude between 2 pixels
    double lat_shift = 3.4 * std::pow(10, -7);   //theorerical shift in latitude between 2 pixels
    int m_i = 0;                                 //lign i in the picture
    int m_j = 0;                                 //column j in the picture
    double longitude = 0, latitude = 0;          //corresponding theoretical long, lat
    int R = 0, G = 0, B = 0;                     //red, green and blue intensities
};

#endif