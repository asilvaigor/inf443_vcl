#pragma once

#include <map>
#include "../../math/vec/vec3/vec3.hpp"
#include "../../math/mat/mat4/mat4.hpp"
#include "../../math/mat/mat3/mat3.hpp"

namespace vcl {

struct perspective_structure {
    perspective_structure();

    perspective_structure(float angle_of_view, float image_aspect, float z_near, float z_far);

    float angle_of_view;
    float image_aspect;
    float z_near;
    float z_far;
    mat4 matrix;
    mat4 matrix_inverse;
};

enum camera_control_type {
    camera_control_trackball, camera_control_spherical_coordinates
};

class float_compare : public std::binary_function<std::pair<float, float>, std::pair<float, float>, bool> {
public:
    bool operator()(const std::pair<float, float> &a, const std::pair<float, float> &b) const {
        return b.first - a.first > 1e-5 || (fabs(a.first - b.first) < 1e-5 && b.second - a.second > 1e-5);
    }
};

/** Structure handling a camera.
    The camera handle internally
    - a scale parameter (zoom in/out) - float
    - a translation (camera position) - vec3
    - an orientation - mat3 */
class camera_scene {
public:
    camera_scene();

    // Apply transformation to camera
    void apply_translation_in_screen_plane(float tr_x, float tr_y);

    void apply_translation_orthogonal_to_screen_plane(float tr);

    void apply_rotation(float p0x, float p0y, float p1x, float p1y);

    void apply_scaling(float s);

    void set_scale(const float &s);

    void set_perspective(const perspective_structure &perspective);

    void set_view(const mat4 &view);

    void set_translation(const vec3 &translation);

    void set_orientation(const mat3 &orientation);

    void set_image_aspect(const float &aspect);

    /** Compute the view matrix to be multiplied to vertices */
    mat4 &get_view_matrix();

    mat4 &get_camera_matrix();

    mat4 &get_perspective_view_matrix();

    vec3 &get_position();

    vec3 &get_direction();

    vec3 &get_translation();

    mat3 &get_orientation();

    perspective_structure &get_perspective();

    void set_z_near_far_list(std::vector<std::pair<float, float>> &z_near_far);

    std::vector<vec3> &get_frustum_corners(float z_near, float z_far);

    std::vector<vec4> &get_frustum_planes();

    /** Checks if a point is inside the frustum */
    bool is_inside_frustum(vec3 &pt);

private:
    float scale0;
    float scale;
    camera_control_type camera_type;
    vec2 spherical_coordinates;
    vec3 translation;
    mat3 orientation;
    perspective_structure perspective;
    mat4 view_m;
    mat4 camera_m;
    mat4 perspective_view_m;
    vec3 position;
    vec3 direction;
    std::vector<vec4> frustumPlanes;
    std::map<std::pair<float, float>, std::vector<vec3>, float_compare> frustum_corners;
    std::vector<std::pair<float, float>> z_near_far;

    void update();

    void calculate_frustum_corners();

    /** Calculates the frustum planes using the camera's z planes. */
    void calculate_frustum_planes();

    /** Compute a 4x4 perspective matrix */
    mat4 calculate_perspective_matrix(float angle_of_view, float image_aspect, float z_near, float z_far);
};

vec3 trackball_projection(float x, float y, float radius = 1.0f);

}
