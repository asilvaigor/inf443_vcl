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
class depth_map {
public:
    /**
     * Creates the buffers and the texture.
     * @param resolution Size of the square matrix that represents the depth map.
     */
    explicit depth_map(GLuint resolution = 1024);

    /**
     * Clears depth map and binds its frame, to draw on it.
     */
    void bind();

    /**
     * Binds back the window frame and stores the depth map in a texture.
     * @param windowWidth
     * @param windowHeight
     */
    void unbind(int windowWidth, int windowHeight);

private:
    const GLuint resolution;
    GLuint texture_id;
    GLuint fbo;
    GLuint layout;
    static GLuint layoutCnt;
};

}


#endif //PGM_DEPTH_MAP_H
