//
// Created by igor on 23/04/2020.
//

#ifndef PGM_LIGHT_SOURCE_H
#define PGM_LIGHT_SOURCE_H

#include "math/mat/mat.hpp"

namespace vcl {

mat4 light_source_matrix();

mat4 ortho(float const &left, float const &right, float const &bottom, float const &top, float const &zNear,
           float const &zFar);

mat4 lookAt(vec3 const &eye, vec3 const &center, vec3 const &up);

}

#endif //PGM_LIGHT_SOURCE_H
