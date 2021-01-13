#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>

class Pixel
{
public:
    Pixel(int R, int G, int B);
    Pixel();
    // ~Pixel();
    int get_R() const;
    int get_G() const;
    int get_B() const;
    float get_x() const;
    float get_y() const;
    const std::vector<int> *get_triangles_indexes() const;
    void set_RGB(float elevation, const float min_elevation, const float max_elevation);
    void set_gray_intensity(float elevation, const float min_elevation, const float max_elevation);
    void set_x_y(int i, int j, const int img_width, const int img_height, const float xmin, const float xmax, const float ymin, const float ymax);
    void add_triangle_index(int index);

private:
    float m_x = 0, m_y = 0;                                    //theoretical x, y position of the projected point
    int m_R, m_G, m_B;                                         //red, green and blue intensities
    std::vector<int> m_triangles_indexes = std::vector<int>(); //init empty vector : gives the triangles after Delaunay's triangulation to which Pixel(i,j) could belong
};

#endif