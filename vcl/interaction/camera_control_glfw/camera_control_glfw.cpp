#include "camera_control_glfw.hpp"
#include "math/transformation/special_transform/special_transform.hpp"


namespace vcl {


enum class camera_action_state {
    none, translation, rotation, scale, translation_depth
};

void camera_control_glfw::update_mouse_move(camera_scene &camera, GLFWwindow *window, float x1, float y1) {
    assert(window != nullptr);

    // Get type of click (left, right), is CTRL pressed
    const bool mouse_click_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    const bool mouse_click_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    const bool mouse_click_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    const bool key_ctrl = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL));
    const bool key_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));

    if (!update || key_shift) {
        x0 = x1;
        y0 = y1;
        return;
    }

    // Get window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    const auto w = static_cast<float>(width);
    const auto h = static_cast<float>(height);

    // Set action state accordingly
    camera_action_state state = camera_action_state::none;
    if (mouse_click_left && !key_ctrl)
        state = camera_action_state::rotation;
    else if (mouse_click_left || mouse_click_middle)
        state = camera_action_state::translation;
    else if (mouse_click_right && !key_ctrl)
        state = camera_action_state::scale;
    else if (mouse_click_right)
        state = camera_action_state::translation_depth;

    // ************************************************* //
    // Compute transformation to apply on the camera
    // ************************************************* //
    if (state == camera_action_state::translation) {
        const float tx = (x1 - x0) / w;
        const float ty = -(y1 - y0) / h;
        camera.apply_translation_in_screen_plane(tx, ty);
    } else if (state == camera_action_state::rotation) {
        // relative position on screen
        const float ux0 = 2 * x0 / float(w) - 1;
        const float uy0 = 1 - 2 * y0 / float(h);
        const float ux1 = 2 * x1 / float(w) - 1;
        const float uy1 = 1 - 2 * y1 / float(h);

        // apply rotation
        camera.apply_rotation(ux0, uy0, ux1, uy1);
    } else if (state == camera_action_state::scale) {
        const float scale_magnitude = (y0 - y1) / h;
        camera.apply_scaling(scale_magnitude);
    } else if (state == camera_action_state::translation_depth) {
        const float translation_magnitude = (y1 - y0) / h;
        camera.apply_translation_orthogonal_to_screen_plane(translation_magnitude);
    }

    // Update previous click position
    x0 = x1;
    y0 = y1;
}

void camera_control_glfw::update_mouse_move(camera_scene &camera, mat3 &follow_orientation, GLFWwindow *window, float x1, float y1) {
    assert(window != nullptr);

    // Get type of click (left, right), is CTRL pressed
    const bool mouse_click_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    const bool mouse_click_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    const bool mouse_click_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    const bool key_ctrl = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL));
    const bool key_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));

    if (!update || key_shift) {
        x0 = x1;
        y0 = y1;
        return;
    }

    // Get window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    const auto w = static_cast<float>(width);
    const auto h = static_cast<float>(height);

    // Set action state accordingly
    camera_action_state state = camera_action_state::none;
    if (mouse_click_left && !key_ctrl)
        state = camera_action_state::rotation;
    else if (mouse_click_right && !key_ctrl)
        state = camera_action_state::scale;

    // ************************************************* //
    // Compute transformation to apply on the camera
    // ************************************************* //
    if (state == camera_action_state::rotation) {
        // relative position on screen
        const float ux0 = 2 * x0 / float(w) - 1;
        const float uy0 = 1 - 2 * y0 / float(h);
        const float ux1 = 2 * x1 / float(w) - 1;
        const float uy1 = 1 - 2 * y1 / float(h);

        const vec3 p0 = trackball_projection(ux0, uy0);
        const vec3 p1 = trackball_projection(ux1, uy1);

        follow_orientation = follow_orientation * transpose(rotation_between_vector_mat3(p0, p1));
    } else if (state == camera_action_state::scale) {
        const float scale_magnitude = (y0 - y1) / h;
        camera.apply_scaling(scale_magnitude);
    }

    // Update previous click position
    x0 = x1;
    y0 = y1;
}


void camera_control_glfw::update_mouse_click(camera_scene &, GLFWwindow *, int, int, int) {

}

void camera_control_glfw::update_mouse_scroll(camera_scene &camera, GLFWwindow *window, double, double yoffset) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    const auto h = static_cast<float>(height);
    const float scale_magnitude = -30.0f * (float) yoffset / h;
    camera.apply_scaling(scale_magnitude);
}


}
