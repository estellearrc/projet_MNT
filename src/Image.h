#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdlib>
#include <map>
#include "Pixel.h"

/**
 * @class Image
 * @brief A class defining a custom image with some classic attributes :
 * * width
 * * height
 * * grayscale or in color
 * And some custom ones :
 * * the format of encoding (PGM ASCII, PGM binary, PPM ASCII, PPM binary)
 */
class Image
{
public:
    /**
     * @brief Default constructor : construct a new Image object
     * 
     */
    Image();
    /**
     * @brief Construct a new Image object
     * 
     * @param width a constant integer chosen par the user
     * @param xmin a constant float giving the minimal x coordinate on the sampled terrain
     * @param xmax a constant float giving the maximal x coordinate on the sampled terrain
     * @param ymin a constant float giving the minimal y coordinate on the sampled terrain
     * @param ymax a constant float giving the maximal y coordinate on the sampled terrain
     * @param gray a boolean. If true, the image is in grayscale ; in color otherwise
     * @param binary a boolean. If true, the image is encoded in binary ; in ASCII otherwise
     */
    Image(const int width, const float xmin, const float xmax, const float ymin, const float ymax, bool gray, bool binary);
    /**
     * @brief Determine the magic number to encode the image in the right format when writing it as a file
     * 
     */
    void determine_magic_number();
    /**
     * @brief Get the image's width in number of pixels
     * 
     * @return int 
     */
    int get_width() const;
    /**
     * @brief Get the image's height in number of pixels
     * 
     * @return int 
     */
    int get_height() const;
    /**
     * @brief Get the magic number
     * 
     * @return int 
     */
    int get_magic_number() const;
    /**
     * @brief Check if the image is in grayscale
     * 
     * @return bool
     */
    bool is_gray() const;
    /**
     * @brief Check if the image is to be encoded in binary
     * 
     * @return bool
     */
    bool is_binary() const;
    /**
     * @brief Get the pixel object at position (i,j)
     * 
     * @param i an integer giving the line of the pixel
     * @param j an integer giving the column of the pixel
     * @return Pixel* a pointer to a Pixel object
     */
    Pixel *get_pixel(int i, int j);
    /**
     * @brief Get all the pixels of the image
     * 
     * @return const std::map<std::pair<int, int>, Pixel>* a constant pointer to a map of pixels associated to their (i,j) position
     */
    const std::map<std::pair<int, int>, Pixel> *get_pixels() const;

private:
    int m_magic_number; ///< the number to correctly encode the image when writing a PPM or PGM file
    bool m_gray;
    bool m_binary;
    int m_width, m_height;
    std::map<std::pair<int, int>, Pixel> m_pixels;
};

#endif