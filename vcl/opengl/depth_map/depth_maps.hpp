//
// Created by igor on 23/04/2020.
//

#ifndef PGM_DEPTH_MAP_H
#define PGM_DEPTH_MAP_H

#include <iostream>

namespace vcl {

/**
 * Depth map for shadow calculation.
 * Code based on <https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping>.
 */
class depth_maps {
public:
    /**
     * Creates the buffers and the texture.
     * @param n Number of depth maps which will be used.
     * @param resolution Size of the square matrix that represents the depth map.
     */
    explicit depth_maps(int n, GLuint resolution = 1024);

    ~depth_maps();

    /**
     * Clears depth map and binds its frame, to draw on it.
     * @param i Id of the depth map to be binded.
     */
    void bind(int i);

    /**
     * Binds back the window frame and stores the depth maps in textures.
     * @param windowWidth
     * @param windowHeight
     */
    void unbind_all(int windowWidth, int windowHeight);

    /**
     * Binds back the window frame and stores the depth map in a texture.
     * @param i Id of the depth map to unbind.
     * @param windowWidth
     * @param windowHeight
     */
    void unbind(int i, int windowWidth, int windowHeight);

private:
    const GLuint resolution;
    GLuint fbo;
    GLuint *texture_ids;
    std::vector<GLuint> layouts;
    std::vector<bool> binded;
    int nBinded;
};

}


#endif //PGM_DEPTH_MAP_H
