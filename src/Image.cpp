#include "Image.h"

using namespace std;

Image::Image(const int width, const float xmin, const float xmax, const float ymin, const float ymax, bool gray, bool binary) : m_width(width), m_height((int)(abs(xmax - xmin) * width / abs(ymax - ymin))), m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax)
{
    m_gray = gray;
    m_binary = binary;
    determine_magic_number();
    //init pixels
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            m_pixels[make_pair(-i, j)] = Pixel(0, 0, 0);
            m_pixels[make_pair(-i, j)].set_x_y(i, j, m_width, m_height, xmin, xmax, ymin, ymax);
        }
    }
}
int Image::get_width() const
{
    return m_width;
}
int Image::get_height() const
{
    return m_height;
}
int Image::get_magic_number() const
{
    return m_magic_number;
}
bool Image::is_gray() const
{
    return m_gray;
}
bool Image::is_binary() const
{
    return m_binary;
}
Pixel *Image::get_pixel(int i, int j)
{
    return &m_pixels[make_pair(-i, j)];
}
const map<pair<int, int>, Pixel> *Image::get_pixels() const
{
    return &m_pixels;
}
void Image::determine_magic_number()
{
    if (m_gray) //PGM file
    {
        if (m_binary) //binary
            m_magic_number = 5;
        else //ASCII
            m_magic_number = 2;
    }
    else //PPM file
    {
        if (m_binary) //binary
            m_magic_number = 6;
        else //ASCII
            m_magic_number = 3;
    }
}
