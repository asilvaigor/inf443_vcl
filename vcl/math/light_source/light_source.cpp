//
// Created by igor on 23/04/2020.
//

#include "light_source.hpp"

namespace vcl {

light_source::light_source() = default;

light_source::light_source(vec3 pos, vec3 dir, float camera_z_near, float camera_z_far, int shadow_map_id) {
    this->pos = pos;
    this->dir = dir.normalized();
    this->camera_z_near = camera_z_near;
    this->camera_z_far = camera_z_far;
    this->shadow_map_id = shadow_map_id;
}

void light_source::update(camera_scene &camera) {
    auto corners = camera.calculate_frustum_corners(camera_z_near, camera_z_far);
    vec3 centroid;
    float min_z = std::numeric_limits<float>::max();
    float max_z = std::numeric_limits<float>::min();
    for (auto &c : corners) {
        centroid += c;
        min_z = std::min(min_z, c.z);
        max_z = std::max(max_z, c.z);
    }
    centroid /= 8.0f;

    vec3 virtual_pos = centroid + dir * (max_z - min_z);

    mat4 view = calculate_view_matrix(virtual_pos);
    mat4 ortho = calculate_ortho_matrix(corners, view);

    matrix = ortho * view;
}

mat4 light_source::get_matrix() const {
    return matrix;
}

vec3 light_source::get_pos() const {
    return pos;
}

vec3 light_source::get_dir() const {
    return dir;
}

float light_source::get_z_near() const {
    return camera_z_near;
}

float light_source::get_z_far() const {
    return camera_z_far;
}

int light_source::get_shadow_map_id() const {
    return shadow_map_id;
}

mat4 light_source::calculate_view_matrix(vec3 const &virtual_pos) const {
    vec3 xaxis = cross({0, 0, 1}, dir).normalized();
    vec3 yaxis = cross(dir, xaxis).normalized();

    mat4 m(xaxis.x, xaxis.y, xaxis.z, -virtual_pos.x,
           yaxis.x, yaxis.y, yaxis.z, -virtual_pos.y,
           dir.x, dir.y, dir.z, -virtual_pos.z,
           0, 0, 0, 1);

    return m;
}

mat4 light_source::calculate_ortho_matrix(std::vector<vec3> &corners, mat4 view) {
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::min();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::min();
    float min_z = std::numeric_limits<float>::max();
    float max_z = std::numeric_limits<float>::min();

    for (int i = 0; i < 8; i++) {
        vec4 tmp = view * vec4(corners[i], 1);
        min_x = std::min(tmp.x, min_x);
        max_x = std::max(tmp.x, max_x);
        min_y = std::min(tmp.y, min_y);
        max_y = std::max(tmp.y, max_y);
        min_z = std::min(tmp.z, min_z);
        max_z = std::max(tmp.z, max_z);
    }

    return ortho(min_x, max_x, min_y, max_y, 0, max_z - min_z);
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

}