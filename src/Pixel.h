#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <iostream>
#include <cmath>

class Pixel
{
public:
    Pixel(int R, int G, int B);
    Pixel();
    // ~Pixel();
    int get_R() const;
    int get_G() const;
    int get_B() const;
    int get_x() const;
    int get_y() const;
    void set_RGB(float elevation, const float min_elevation, const float max_elevation);
    void set_gray_intensity(float elevation, const float min_elevation, const float max_elevation);
    void set_x_y(int i, int j, const int width, const int height, const float xmin, const float xmax, const float ymin, const float ymax);

private:
    float m_x = 0, m_y = 0; //theoretical x, y position of the projected point
    int m_R, m_G, m_B;      //red, green and blue intensities
};

#endif