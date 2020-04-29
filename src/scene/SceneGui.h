//
// Created by igor on 29/04/2020.
//

#ifndef PGM_SCENEGUI_H
#define PGM_SCENEGUI_H

#include <memory>
#include "vcl.hpp"
#include <GLFW/glfw3.h>

/**
 * Gui for the scene, which has camera and window objects.
 */
class SceneGui {
public:
    /**
     * Gets the instance of the singleton class, or throws an exception.
     * @param windowTitle Title of the gui window.
     * @return Instance of SceneGui class.
     */
    static std::shared_ptr<SceneGui> &getInstance(std::string windowTitle);

    /**
     * Destroys window.
     */
    ~SceneGui();

    /**
     * Clears screen and buffers and updates checkboxes.
     */
    void update();

    /**
     * Renders the scene in the screen.
     */
    void render();

    /**
     * Checks if the window wasn't closed.
     * @return If the gui is still running.
     */
    bool isRunning();

    /**
     * Wireframe checkbox in the gui.
     */
    bool showWireframe();

    vcl::camera_scene &getCamera();

    int getWindowWidth();

    int getWindowHeight();

private:
    std::string windowTitle;
    vcl::glfw_fps_counter fpsCounter;
    bool wireframeOn;

    static std::shared_ptr<SceneGui> gui;
    static GLFWwindow *window;
    static int windowWidth;
    static int windowHeight;
    static vcl::camera_control_glfw cameraControl;
    static vcl::camera_scene camera;

    /**
     * Initializes opengl and the gui.
     * @param windowTitle
     */
    explicit SceneGui(std::string &windowTitle);

    /**
     * Called in display loop, updates the fps text in the gui.
     */
    void updateFps();

    /**
     * These functions will be passed to opengl for interactions with the scene.
     */
    static void windowSizeCallback(GLFWwindow *window, int width, int height);

    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

    static void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);

    static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    static void keyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};


#endif //PGM_SCENEGUI_H
