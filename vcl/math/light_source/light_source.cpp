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
    calculate_color();
}

void light_source::update(camera_scene &camera, vec3 &p, vec3 &d) {
    pos = p;
    dir = d;
    calculate_color();
    auto corners = camera.calculate_frustum_corners(camera_z_near, camera_z_far);
    vec3 centroid;
    float z_min = std::numeric_limits<float>::max();
    float z_max = std::numeric_limits<float>::lowest();
    for (auto &c : corners) {
        centroid += c;
        float z = dot(c, dir);
        z_min = std::min(z_min, z);
        z_max = std::max(z_max, z);
    }
    centroid /= 8.0f;

    vec3 virtual_pos = centroid - dir * (z_max - z_min);

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

vec3 light_source::get_color() const {
    return color;
}

mat4 light_source::calculate_view_matrix(vec3 const &virtual_pos) const {
    vec3 xaxis = cross({0, 0, 1}, dir).normalized();
    vec3 yaxis = cross(-dir, xaxis).normalized();

    mat4 m(xaxis.x, xaxis.y, xaxis.z, -virtual_pos.x,
           yaxis.x, yaxis.y, yaxis.z, -virtual_pos.y,
           -dir.x, -dir.y, -dir.z, -virtual_pos.z,
           0, 0, 0, 1);

    return m;
}

mat4 light_source::calculate_ortho_matrix(std::vector<vec3> &corners, mat4 view) {
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::lowest();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::lowest();
    float min_z = std::numeric_limits<float>::max();
    float max_z = std::numeric_limits<float>::lowest();

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

void light_source::calculate_color() {
    float angle = std::atan2(pos.z, hypotf(pos.x, pos.y));
    float sunAngle = std::fabs((float) M_PI_2 - angle);

    vec3 white(1, 1, 1);
    vec3 yellow(0.99, 0.88, 0.24);
    vec3 red(0.92, 0.32, 0.14);

    float angles[] = {M_PI / 4, 1.75 * M_PI / 4, M_PI / 2};

    // Moon color
    if (angle < 0)
        color = {1, 1, 1};
    // Sun in the sky color
    else if (sunAngle < angles[0])
        color = white;
    // Sunset color
    else if (sunAngle < angles[1]) {
        auto lambda = (float) ((sunAngle - angles[0]) / (angles[1] - angles[0]));
        color = (1 - lambda) * white + lambda * yellow;
    }
    else {
        auto lambda = (float) ((sunAngle - angles[1]) / (angles[2] - angles[1]));
        color = (1 - lambda) * yellow + lambda * red;
    }
}

}