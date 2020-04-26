//
// Created by igor on 23/04/2020.
//

#ifndef PGM_DEPTH_MAP_H
#define PGM_DEPTH_MAP_H

#include <iostream>

namespace vcl {

class depth_map {
public:
    explicit depth_map(GLuint resolution = 1024);

    void bind();

    void unbind(int windowWidth, int windowHeight);

private:
    const GLuint resolution;
    GLuint texture_id;
    GLuint fbo;
};

}


#endif //PGM_DEPTH_MAP_H
