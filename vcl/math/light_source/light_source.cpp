//
// Created by igor on 23/04/2020.
//

#include "light_source.hpp"

namespace vcl {

mat4 light_source_matrix() {
    float near_plane = 1.0f, far_plane = 7.5f;
    vcl::mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    vcl::mat4 lightView = lookAt(vec3(-2.0f, 4.0f, -1.0f),
                                 vec3(0.0f, 0.0f, 0.0f),
                                 vec3(0.0f, 1.0f, 0.0f));
    return lightProjection * lightView;
}

mat4 ortho(float const &left, float const &right, float const &bottom, float const &top, float const &zNear,
           float const &zFar) {
    mat4 result;
    result(0, 0) = 2.0f / (right - left);
    result(1, 1) = 2.0f / (top - bottom);
    result(2, 2) = -2.0f / (zFar - zNear);
    result(3, 3) = 1.0f;
    result(3, 0) = -(right + left) / (right - left);
    result(3, 1) = -(top + bottom) / (top - bottom);
    result(3, 2) = -(zFar + zNear) / (zFar - zNear);
    return result;
}

mat4 lookAt(vec3 const &eye, vec3 const &center, vec3 const &up) {
    vec3 const f((center - eye).normalized());
    vec3 const s(cross(f, up).normalized());
    vec3 const u(cross(s, f));

    mat4 result;
    result(0, 0) = s.x;
    result(1, 0) = s.y;
    result(2, 0) = s.z;
    result(0, 1) = u.x;
    result(1, 1) = u.y;
    result(2, 1) = u.z;
    result(0, 2) = -f.x;
    result(1, 2) = -f.y;
    result(2, 2) = -f.z;
    result(3, 0) = -dot(s, eye);
    result(3, 1) = -dot(u, eye);
    result(3, 2) = dot(f, eye);
    result(3, 3) = 1.0f;
    return result;
}

}