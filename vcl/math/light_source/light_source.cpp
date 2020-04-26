//
// Created by igor on 23/04/2020.
//

#include "light_source.hpp"

namespace vcl {

light_source::light_source(vec3 pos) {
    this->pos = pos;
    matrix = to_matrix();
}

mat4 light_source::to_matrix() {
    float near_plane = 1.0f, far_plane = 7.5f;

    vcl::mat4 lightPerspective = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    vcl::mat4 lightView = lookAt(vec3(0.0f, 0.0f, 0.0f),
                                 vec3(0.0f, 1.0f, 0.0f));

    // Shaders matrix operations use transpose
    return vcl::transpose(lightPerspective * lightView);
}

mat4 light_source::ortho(float const &left, float const &right, float const &bottom, float const &top,
        float const &zNear, float const &zFar) {
    mat4 result = mat4::identity();
    result(0, 0) = 2.0f / (right - left);
    result(1, 1) = 2.0f / (top - bottom);
    result(2, 2) = -2.0f / (zFar - zNear);
    result(0, 3) = -(right + left) / (right - left);
    result(1, 3) = -(top + bottom) / (top - bottom);
    result(2, 3) = -(zFar + zNear) / (zFar - zNear);
    return result;
}

mat4 light_source::lookAt(vec3 const &center, vec3 const &up) const {
    vec3 const f((center - pos).normalized());
    vec3 const s(cross(f, up).normalized());
    vec3 const u(cross(s, f));

    mat4 result = mat4::identity();
    result(0, 0) = s.x;
    result(0, 1) = s.y;
    result(0, 2) = s.z;
    result(1, 0) = u.x;
    result(1, 1) = u.y;
    result(1, 2) = u.z;
    result(2, 0) = -f.x;
    result(2, 1) = -f.y;
    result(2, 2) = -f.z;
    result(0, 3) = -dot(s, pos);
    result(1, 3) = -dot(u, pos);
    result(2, 3) = dot(f, pos);
    return result;
}

}