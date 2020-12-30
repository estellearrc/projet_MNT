#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <fstream>
#include <cmath>

class Pixel
{
public:
    // we suppose imin=0 and jmin=0
    Pixel(int R, int G, int B);
    Pixel(int gray_intensity);
    Pixel();
    // ~Pixel();
    const int get_R();
    const int get_G();
    const int get_B();
    void set_RGB(float elevation, const float min_elevation, const float max_elevation);
    void set_gray_intensity(float elevation, const float min_elevation, const float max_elevation);
    void set_x_y(int i, int j, const int width, const int height, const float xmin, const float xmax, const float ymin, const float ymax);

private:
    float m_x = 0, m_y = 0; //theoretical x, y position of the projected point
    int m_R, m_G, m_B;      //red, green and blue intensities
};

#endif