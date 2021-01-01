#include "Pixel.h"

using namespace std;

Pixel::Pixel(int R, int G, int B) : m_R(R), m_G(G), m_B(B)
{
}
Pixel::Pixel() : m_R(0), m_G(0), m_B(0)
{
}
int Pixel::get_R() const
{
    return m_R;
}
int Pixel::get_G() const
{
    return m_G;
}
int Pixel::get_B() const
{
    return m_B;
}
void Pixel::set_gray_intensity(float elevation, const float min_elevation, const float max_elevation)
{
    float normalized_elevation = 0;
    if (elevation != 0) //elevation normalization only for non-black pixels
        normalized_elevation = (elevation - min_elevation) / (max_elevation - min_elevation);
    int intensity = (int)(255 * normalized_elevation);
    m_R = intensity;
    m_G = intensity;
    m_B = intensity;
}
void Pixel::set_RGB(float elevation, const float min_elevation, const float max_elevation)
{
    //create a short rainbow color map

    /* To convert a scalar to a rainbow, we divide the scalar into four groups. Then based on which group we are in, we perform one of the following linear interpolations:

    Group 1: Red is maximum (255), blue is zero, and green varies linearly from 0 to 255.
    Group 2: Green is kept maximum and blue is still zero. Red varies linearly from 255 to 0.
    Group 3: Keep red at zero and green and maximum. Blue varies linearly from 0 to 255.
    Group 4: Red is kept at zero and blue at maximum. Green varies linearly from 255 to 0.

    This ordering follows the image in the Wikipedia article. One downside of this approach is that it will vary from red to blue as f varies from 0 to 1. Typically, we want the opposite, with 0 mapping to blue and 1 to red. So we simply invert the scalar as a=1-f. */

    float normalized_elevation = 0;
    if (elevation != 0) //elevation normalization only for non-black pixels
        normalized_elevation = 1 - (elevation - min_elevation) / (max_elevation - min_elevation);
    double a = (1 - normalized_elevation) / 0.25; //invert and group
    int x = (int)floor(a);                        //integer part
    int y = (int)floor(255 * (a - x));            //fractional part from 0 to 255
    switch (x)
    {
    case 0:
        m_R = 255;
        m_G = y;
        m_B = 0;
        break;
    case 1:
        m_R = 255 - y;
        m_G = 255;
        m_B = 0;
        break;
    case 2:
        m_R = 0;
        m_G = 255;
        m_B = y;
        break;
    case 3:
        m_R = 0;
        m_G = 255 - y;
        m_B = 255;
        break;
    case 4:
        m_R = 0;
        m_G = 0;
        m_B = 255;
        break;
    }
}
void Pixel::set_x_y(int i, int j, const int width, const int height, const float xmin, const float xmax, const float ymin, const float ymax)
{
    // compute theoretical projected position of the pixel on the field (interpolation is needed next)
    m_x = xmin + (j + 1) * (xmax - xmin) / width;
    m_y = ymin + (i + 1) * (ymax - ymin) / height;
}