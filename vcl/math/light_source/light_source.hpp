//
// Created by igor on 23/04/2020.
//

#ifndef PGM_LIGHT_SOURCE_H
#define PGM_LIGHT_SOURCE_H

#include "math/mat/mat.hpp"

namespace vcl {

/**
 * Light source implementation for shadow rendering.
 * Check out <https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping> for tutorial.
 */
class light_source {
public:
    explicit light_source(vec3 pos = {10, 10, 10});

    vec3 pos;
    mat4 matrix;
private:
    mat4 to_matrix();

    mat4 ortho(float const &left, float const &right, float const &bottom, float const &top, float const &zNear,
               float const &zFar);

    mat4 lookAt(vec3 const &center, vec3 const &up) const;
};

}

#endif //PGM_LIGHT_SOURCE_H
