//
// Created by igor on 29/04/2020.
//

#include <unistd.h>
#include "SceneGui.h"
#include "utils/SingletonException.h"
#include "utils/Constants.h"

std::shared_ptr<SceneGui> SceneGui::gui;
GLFWwindow *SceneGui::window;
int SceneGui::windowWidth;
int SceneGui::windowHeight;
vcl::camera_control_glfw SceneGui::cameraControl;
vcl::camera_scene SceneGui::camera;

std::shared_ptr<SceneGui> &SceneGui::getInstance(std::string _windowTitle) {
    if (gui != nullptr)
        throw SingletonException("SceneGui");
    gui.reset(new SceneGui(_windowTitle));
    return gui;
}

SceneGui::SceneGui(std::string &windowTitle) : windowTitle(windowTitle) {
    vcl::glfw_init();

    const int openglVersionMajor = 3;
    const int openglVersionMinor = 3;
    windowWidth = 1280;
    windowHeight = 1000;
    window = vcl::glfw_create_window(windowWidth, windowHeight, windowTitle, openglVersionMajor,
                                     openglVersionMinor);

    vcl::glad_init();
    vcl::opengl_debug_print_version();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    vcl::imgui_init(window);

    sunAngle = 0.25 * M_PI;
    verticesOn = false;
    gridOn = false;

    // Initial camera parameters
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    float aspectRatio = (float) windowWidth / (float) windowHeight;
    camera.perspective = vcl::perspective_structure(Constants::CAMERA_ANGLE, aspectRatio, Constants::CAMERA_Z_NEAR,
            Constants::CAMERA_Z_FAR);
    camera.apply_scaling(4);
    camera.apply_rotation(0, 0, 0, 1.0);

    // Interaction callbacks
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseClickCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetKeyCallback(window, keyboardInputCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
}

SceneGui::~SceneGui() {
    // FIXME: There was a segfault here
//    vcl::imgui_cleanup();
//    glfwDestroyWindow(window);
//    glfwTerminate();
}

void SceneGui::open() {
    glViewport(0, 0, gui->getWindowWidth(), gui->getWindowHeight());
    glfwShowWindow(window);
}

void SceneGui::update() {
    opengl_debug();

    // Clears screen and buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    opengl_debug();

    // Creates frame and checkbox
    vcl::imgui_create_frame();
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Vertices", &verticesOn);
    ImGui::Checkbox("Grid", &gridOn);
    ImGui::SliderAngle("Sun Angle", &sunAngle, -180, 180);

    opengl_debug();
}

void SceneGui::render() {
    ImGui::End();
    cameraControl.update = !(ImGui::IsAnyWindowFocused());
    vcl::imgui_render_frame(window);
    updateFps();
    glfwSwapBuffers(window);
    glfwPollEvents();
    opengl_debug();
}

bool SceneGui::isRunning() {
    return !glfwWindowShouldClose(window);
}

bool SceneGui::showVertices() {
    return verticesOn;
}

bool SceneGui::showGrid() {
    return gridOn;
}

vcl::camera_scene &SceneGui::getCamera() {
    return camera;
}

int SceneGui::getWindowWidth() {
    return windowWidth;
}

int SceneGui::getWindowHeight() {
    return windowHeight;
}

float SceneGui::getSunAngle() {
    return sunAngle;
}

void SceneGui::updateFps() {
    if (fpsCounter.update()) {
        const std::string new_window_title = windowTitle + " (" + std::to_string(fpsCounter.fps()) + " fps)";
        glfwSetWindowTitle(window, new_window_title.c_str());
        fpsCounter.reset();
    }
}

void SceneGui::windowSizeCallback(GLFWwindow *, int, int) {
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    camera.perspective.image_aspect = (float) windowWidth / (float) windowHeight;
}

void SceneGui::cursorPositionCallback(GLFWwindow *w, double xpos, double ypos) {
    cameraControl.update_mouse_move(camera, w, float(xpos), float(ypos));
}

void SceneGui::mouseClickCallback(GLFWwindow *, int button, int action, int mods) {
    ImGui::SetWindowFocus(nullptr);
    cameraControl.update_mouse_click(camera, window, button, action, mods);
}

void SceneGui::mouseScrollCallback(GLFWwindow *, double xoffset, double yoffset) {
    cameraControl.update_mouse_scroll(camera, window, xoffset, yoffset);
}

void SceneGui::keyboardInputCallback(GLFWwindow *, int, int, int, int) {

}

void SceneGui::setShowVertices(bool verticesShowState) {
    verticesOn = verticesShowState;
}
