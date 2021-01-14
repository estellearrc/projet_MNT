#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>

/**
 * @file
 */

/**
 * @class Pixel
 * @brief A class defining a custom pixel : RGB components with custom attributes :
 * * (x,y) theoretical coordinates in the terrain
 * * the associated triangular zones after Delaunay's triangulation of the terrain (if applicable)
 */
class Pixel
{
public:
    /**
     * @brief Default constructor : construct a new Pixel object
     */
    Pixel();
    /**
     * @brief Construct a new Pixel object
     * @param R an integer : the red component of the pixel
     * @param G an integer : the green component of the pixel
     * @param B an integer : the blue component of the pixel
     */
    Pixel(int R, int G, int B);
    /**
     * @brief Get the R component
     * 
     * @return int 
     */
    int get_R() const;
    /**
     * @brief Get the G component
     * 
     * @return int 
     */
    int get_G() const;
    /**
     * @brief Get the B component
     * 
     * @return int 
     */
    int get_B() const;
    /**
     * @brief Get the x theoretical coordinate of the pixel on the terrain
     * 
     * @return float 
     */
    float get_x() const;
    /**
     * @brief Get the y theoretical coordinate of the pixel on the terrain
     * 
     * @return float 
     */
    float get_y() const;
    /**
     * @brief Get the possible triangles' indexes for the pixel. 
     * Enable to identify to which triangular zone of the terrain the pixel belongs to
     * 
     * @return const std::vector<int>* a pointer to the vector of indexes
     */
    const std::vector<int> *get_triangles_indexes() const;
    /**
     * @brief Set the RGB components
     * 
     * @param elevation a float giving the altitude of the corresponding point on the terrain
     * @param min_elevation a float giving the minimal altitude on the terrain
     * @param max_elevation a float giving the maximal altitude on the terrain
     */
    void set_RGB(float elevation, const float min_elevation, const float max_elevation);
    /**
     * @brief Set the the RGB components for grayscale images
     * @param elevation a float giving the altitude of the corresponding point on the terrain
     * @param min_elevation a float giving the minimal altitude on the terrain
     * @param max_elevation a float giving the maximal altitude on the terrain
     */
    void set_gray_intensity(float elevation, const float min_elevation, const float max_elevation);
    /**
     * @brief Set the theoretical coordinates (x,y) of the pixel on the terrain (projection)
     * 
     * @param i an integer giving the line of the pixel
     * @param j an interger giving the column of the pixel
     * @param img_width a constant integer 
     * @param img_height a constant integer 
     * @param xmin a constant float giving the minimal x coordinate on the sampled terrain
     * @param xmax a constant float giving the maximal x coordinate on the sampled terrain
     * @param ymin a constant float giving the minimal y coordinate on the sampled terrain
     * @param ymax a constant float giving the maximal y coordinate on the sampled terrain
     */
    void set_x_y(int i, int j, const int img_width, const int img_height, const float xmin, const float xmax, const float ymin, const float ymax);
    /**
     * @brief Add the index of a possible triangular zone of belonging
     * 
     * @param index an integer giving the index of the triangle in Delaunay's vector
     */
    void add_triangle_index(int index);

private:
    float m_x = 0, m_y = 0;                                    ///< theoretical x, y position of the projected point
    int m_R, m_G, m_B;                                         ///< red, green and blue intensities
    std::vector<int> m_triangles_indexes = std::vector<int>(); ///< init empty vector : gives the triangles after Delaunay's triangulation to which Pixel(i,j) could belong
};

#endif