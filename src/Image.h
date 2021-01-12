#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdlib>
#include <map>
#include "Pixel.h"

class Image
{
public:
    Image(const int width, const float xmin, const float xmax, const float ymin, const float ymax, bool gray, bool binary);
    // ~Image();
    void determine_magic_number();
    int get_width() const;
    int get_height() const;
    int get_magic_number() const;
    bool is_gray() const;
    bool is_binary() const;
    Pixel *get_pixel(int i, int j);
    const std::map<std::pair<int, int>, Pixel> *get_pixels() const;

private:
    int m_magic_number;
    bool m_gray;
    bool m_binary;
    const int m_width, m_height;
    const float m_xmin, m_xmax, m_ymin, m_ymax;
    std::map<std::pair<int, int>, Pixel> m_pixels;
};

#endif