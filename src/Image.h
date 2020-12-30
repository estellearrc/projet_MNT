#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdlib>
#include <fstream>
#include <map>
#include "Pixel.h"

class Image
{
public:
    Image(const int width, const float xmin, const float xmax, const float ymin, const float ymax, bool gray = true, bool binary = false);
    // ~Image();
    void determine_magic_number();
    void write();

private:
    int m_magic_number;
    bool m_gray;
    bool m_binary;
    int m_width, m_height;
    float m_xmin, m_xmax, m_ymin, m_ymax;
    std::map<std::pair<int, int>, Pixel> m_pixels;
};

#endif