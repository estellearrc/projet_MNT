#include "Image.h"
#include "formatter.h"

using namespace std;

Image::Image(const int width, const float xmin, const float xmax, const float ymin, const float ymax, bool gray = true, bool binary = false)
{
    m_width = width;
    m_height = (int)(abs(xmax - xmin) * width / abs(ymax - ymin));
    m_xmin = xmin;
    m_xmax = xmax;
    m_ymin = ymin;
    m_ymax = ymax;
    m_gray = gray;
    m_binary = binary;
    //init pixels
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            m_pixels[make_pair(-i, j)] = Pixel();
        }
    }
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
void Image::write() {}
