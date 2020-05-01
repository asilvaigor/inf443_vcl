//
// Created by igor on 23/04/2020.
//

#ifndef PGM_LIGHT_SOURCE_H
#define PGM_LIGHT_SOURCE_H

#include "math/mat/mat.hpp"
#include "interaction/camera/camera.hpp"

namespace vcl {

/**
 * Light source implementation for shadow rendering.
 * Check out <https://ahbejarano.gitbook.io/lwjglgamedev/chapter26> for tutorial.
 */
class light_source {
public:
    light_source();

    light_source(vec3 pos, vec3 dir);

    mat4 get_matrix(const camera_scene &camera, float z_near = 0.01f, float z_far = 25.0f) const;

    vec3 get_pos() const;

private:
    vec3 dir;
    vec3 pos;

    mat4 calculate_view_matrix(vec3 const &pos) const;

    static mat4 calculate_ortho_matrix(std::vector<vec3> &corners, mat4 view);

    static mat4 ortho(float const &left, float const &right, float const &bottom, float const &top,
                      float const &zNear, float const &zFar);
};

}

#endif //PGM_LIGHT_SOURCE_H
