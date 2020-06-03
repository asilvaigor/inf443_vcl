#include "camera.hpp"

#include "../../math/transformation/transformation.hpp"

namespace vcl {

perspective_structure::perspective_structure() : perspective_structure(50 * M_PI / 180, 1, 0, 1) {}

perspective_structure::perspective_structure(float angle_of_view_arg, float image_aspect_arg, float z_near_arg,
                                             float z_far_arg)
        : angle_of_view(angle_of_view_arg), image_aspect(image_aspect_arg), z_near(z_near_arg), z_far(z_far_arg) {
    const float fy = 1 / std::tan(angle_of_view / 2);
    const float fx = fy / image_aspect;
    const float L = z_near - z_far;

    const float C = (z_far + z_near) / L;
    const float D = (2 * z_far * z_near) / L;

    matrix = {fx, 0, 0, 0,
              0, fy, 0, 0,
              0, 0, C, D,
              0, 0, -1, 0};

    matrix_inverse = {1 / fx, 0, 0, 0,
                      0, 1 / fy, 0, 0,
                      0, 0, 0, -1,
                      0, 0, 1 / D, C / D};
}

camera_scene::camera_scene() : scale0(3.0f) {
    scale = scale0;
    camera_type = camera_control_trackball;
    translation = {0, 0, 0};
    orientation = mat3::identity();
    view_m = mat4::identity();
    camera_m = mat4::identity();
    perspective_view_m = mat4::identity();
    position = {0, 0, 0};
    direction = {1, 0, 0};
}

void camera_scene::apply_translation_in_screen_plane(float tr_x, float tr_y) {
    const float alpha = scale / scale0;
    translation += (alpha + 0.5f) * orientation * vec3{tr_x, tr_y, 0.0f};
    update();
}

void camera_scene::apply_translation_orthogonal_to_screen_plane(float tr) {
    const float alpha = scale / scale0;
    translation += (alpha + 0.5f) * orientation * vec3{0.0f, 0.0f, tr};
    update();
}


void camera_scene::apply_rotation(float x0, float y0, float x1, float y1) {

    if (camera_type == camera_control_spherical_coordinates) {
        // Assume z-coordinate is up-direction
        const float dtheta = x1 - x0;
        const float dphi = y1 - y0;

        spherical_coordinates.x -= dtheta;
        spherical_coordinates.y += dphi;
        const float theta = spherical_coordinates.x;
        const float phi = spherical_coordinates.y;

        const mat3 Rx = {1, 0, 0,
                         0, std::cos(phi), -std::sin(phi),
                         0, std::sin(phi), std::cos(phi)};

        const mat3 Rz = {std::cos(theta), -std::sin(theta), 0,
                         std::sin(theta), std::cos(theta), 0,
                         0, 0, 1};

        orientation = Rz * Rx;
    } else if (camera_type == camera_control_trackball) {
        /** Trackball implementation from http://ws.iat.sfu.ca/papers/comparerotation.pdf */
        const float d = std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
        if (d < 1e-6f)
            return;

        const vec3 p0 = trackball_projection(x0, y0);
        const vec3 p1 = trackball_projection(x1, y1);

        const mat3 R = rotation_between_vector_mat3(p0, p1);
        orientation = orientation * transpose(R);
    }
    update();
}

void camera_scene::apply_scaling(float s) {
    scale *= (1.0f + s);
    scale = std::max(scale, 0.01f);
    update();
}

void camera_scene::set_scale(const float &s) {
    scale = s;
    update();
}

void camera_scene::set_perspective(const perspective_structure &p) {
    perspective = p;
    update();
}

void camera_scene::set_view(const mat4 &view) {
    view_m = view;
    update();
}

void camera_scene::set_translation(const vec3 &t) {
    translation = t;
    update();
}

void camera_scene::set_orientation(const mat3 &o) {
    orientation = o;
    update();
}

void camera_scene::set_image_aspect(const float &aspect) {
    perspective.image_aspect = aspect;
    update();
}

mat4 &camera_scene::get_view_matrix() {
    return view_m;
}

mat4 &camera_scene::get_camera_matrix() {
    return camera_m;
}

mat4 &camera_scene::get_perspective_view_matrix() {
    return perspective_view_m;
}

vec3 &camera_scene::get_position() {
    return position;
}

vec3 &camera_scene::get_direction() {
    return direction;
}

vec3 &camera_scene::get_translation() {
    return translation;
}

mat3 &camera_scene::get_orientation() {
    return orientation;
}

perspective_structure &camera_scene::get_perspective() {
    return perspective;
}

void camera_scene::set_z_near_far_list(std::vector<std::pair<float, float>> &list) {
    z_near_far = list;
    update();
}

std::vector<vec3> &camera_scene::get_frustum_corners(float z_near, float z_far) {
    assert (frustum_corners.find({z_near, z_far}) != frustum_corners.end());
    return frustum_corners[{z_near, z_far}];
}

std::vector<vec4> &camera_scene::get_frustum_planes() {
    return frustumPlanes;
}

bool camera_scene::is_inside_frustum(vec3 &pt) {
    bool inside = true;
    vec4 pt4(pt, 1.0f);
    for (auto &plane : frustumPlanes) {
        inside &= vcl::dot(plane, pt4) > 0;
    }
    return inside;
}

void camera_scene::update() {
    // Updating view matrix
    mat3 R = transpose(orientation);
    vec3 T = vec3{0, 0, -scale} + R * translation;
    view_m = {R(0, 0), R(0, 1), R(0, 2), T.x,
              R(1, 0), R(1, 1), R(1, 2), T.y,
              R(2, 0), R(2, 1), R(2, 2), T.z,
              0, 0, 0, 1};

    // Updating camera matrix
    R = orientation;
    T = R * vec3{0, 0, scale} - translation;
    camera_m = {R(0, 0), R(0, 1), R(0, 2), T.x,
                R(1, 0), R(1, 1), R(1, 2), T.y,
                R(2, 0), R(2, 1), R(2, 2), T.z,
                0, 0, 0, 1};

    // Updating view_perspective matrix
    perspective_view_m = perspective.matrix * view_m;

    // Updating position
    position = orientation * vec3{0, 0, scale} - translation;

    // Updating direction
    direction = vec3(-orientation.xz, -orientation.yz, -orientation.zz).normalized();

    // Updating frustum
    if (!z_near_far.empty()) {
        calculate_frustum_corners();
        calculate_frustum_planes();
    }
}

void camera_scene::calculate_frustum_corners() {
    for (auto p : z_near_far) {
        std::vector<vec3> corners;
        float z_near = p.first;
        float z_far = p.second;

        auto proj = transpose(calculate_perspective_matrix(
                perspective.angle_of_view, perspective.image_aspect, z_near, z_far) * view_m);

        std::vector<std::vector<int>> signs({{1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
                                             {-1, -1, -1, -1, 1,  1,  1,  1,  1,  1,  1,  1},
                                             {-1, -1, -1, -1, -1, -1, -1, -1, 1,  1,  1,  1},
                                             {1,  1,  1,  1,  -1, -1, -1, -1, 1,  1,  1,  1},
                                             {-1, -1, -1, -1, 1,  1,  1,  1,  -1, -1, -1, -1},
                                             {1,  1,  1,  1,  1,  1,  1,  1,  -1, -1, -1, -1},
                                             {1,  1,  1,  1,  -1, -1, -1, -1, -1, -1, -1, -1},
                                             {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}});
        for (int i = 0; i < 8; i++) {
            vec3 n1(proj.xw + signs[i][0] * proj.xx, proj.yw + signs[i][1] * proj.yx, proj.zw + signs[i][2] * proj.zx);
            vec3 n2(proj.xw + signs[i][4] * proj.xy, proj.yw + signs[i][5] * proj.yy, proj.zw + signs[i][6] * proj.zy);
            vec3 n3(proj.xw + signs[i][8] * proj.xz, proj.yw + signs[i][9] * proj.yz, proj.zw + signs[i][10] * proj.zz);
            vec3 d(proj.ww + signs[i][3] * proj.wx, proj.ww + signs[i][7] * proj.wy, proj.ww + signs[i][11] * proj.wz);

            vec3 c23 = cross(n2, n3);
            mat3 m(c23, cross(n3, n1), cross(n1, n2));
            float invDot = 1.0f / (n1.x * c23.x + n1.y * c23.y + n1.z * c23.z);
            vec3 point = -m * d * invDot;

            corners.push_back(point);
        }

        frustum_corners[p] = corners;
    }
}

void camera_scene::calculate_frustum_planes() {
    frustumPlanes.clear();
    vec3 center = position + direction * (perspective.z_near + perspective.z_far) / 2.0f;
    auto corners = get_frustum_corners(perspective.z_near, perspective.z_far);

    std::vector<std::vector<int>> planesIdx({{0, 1, 2},
                                             {0, 1, 4},
                                             {1, 2, 4},
                                             {2, 3, 6},
                                             {3, 0, 5},
                                             {4, 5, 6}});
    vec4 center4(center, 1.0f);
    for (int i = 0; i < 6; i++) {
        vec3 &a = corners[planesIdx[i][0]];
        vec3 ab = corners[planesIdx[i][1]] - a;
        vec3 ac = corners[planesIdx[i][2]] - a;
        vec3 cross = vcl::cross(ab, ac);
        vec4 plane(cross, -vcl::dot(cross, a));

        // Placing the planes with the correct orientation
        if (vcl::dot(plane, center4) < 0)
            frustumPlanes.push_back(-plane);
        else frustumPlanes.push_back(plane);
    }
}

mat4 camera_scene::calculate_perspective_matrix(float angle_of_view, float image_aspect, float z_near, float z_far) {
    const float fy = 1 / std::tan(angle_of_view / 2);
    const float fx = fy / image_aspect;
    const float L = z_near - z_far;

    const float C = (z_far + z_near) / L;
    const float D = (2 * z_far * z_near) / L;

    return {fx, 0, 0, 0,
            0, fy, 0, 0,
            0, 0, C, D,
            0, 0, -1, 0};
}

vec3 trackball_projection(float x, float y, float radius) {
    const float d = std::sqrt(x * x + y * y);
    float z = 0;
    if (d < radius / std::sqrt(2.0f))
        z = std::sqrt(radius * radius - d * d) / radius;
    else
        z = radius * radius / (2 * d);

    return {x, y, z};
}

}
