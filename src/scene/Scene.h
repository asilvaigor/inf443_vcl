//
// Created by igor on 22/04/2020.
//

#ifndef PGM_SCENE_H
#define PGM_SCENE_H

#include <memory>
#include "vcl.hpp"
#include "objects/Object.h"
#include "shaders/Shaders.h"
#include "utils/Texture.h"
#include <GLFW/glfw3.h>


/**
 * Singleton class which contains a scene with its gui and objects such as camera, light etc.
 */
class Scene {
public:
    /**
     * Gets the instance of the singleton class, or throws an exception.
     * @param windowTitle Title of the gui window.
     * @return Instance of Scene class.
     */
    static Scene &getInstance(std::string windowTitle);

    /**
     * Gets the shaders from the scene.
     * @return
     */
    Shaders &getShaders();

    /**
     * Adds an Object to the scene. This object will then be drawn every loop.
     * @param object
     */
    void addObject(std::shared_ptr<Object> &object);

    /**
     * Activates the loop to display the scene in a window.
     * It will update the gui and draw all the objects in the scene.
     */
    void display();

private:
    static bool exists;
    std::vector<std::shared_ptr<Object>> objects;
    std::shared_ptr<Shaders> shaders;
    vcl::mat4 sunMatrix;
    std::pair<GLuint, GLuint> depthMap;

    static vcl::camera_scene camera;
    static vcl::camera_control_glfw cameraControl;
    bool showWireframe;

    static GLFWwindow *window;
    std::string windowTitle;
    int windowWidth;
    int windowHeight;
    vcl::glfw_fps_counter fpsCounter;
    std::shared_ptr<Texture> whiteTexture;

    /**
     * Will initialize opengl, the window and the scene's camera and shaders.
     * @param windowTitle
     */
    explicit Scene(std::string &windowTitle);

    /**
     * Called in the constructor, initializes opengl and the gui.
     */
    void initializeInterface();

    /**
     * Called in the constructor, initializes shaders and camera.
     */
    void setupScene();

    /**
     * Called in display loop, updates the fps text in the gui.
     */
    void updateFps();

    /**
     * Updates gui frame.
     */
    void updateGui();

    /**
     * Updates the objects in the scene using a depth map.
     */
    void updateScene();

    /**
     * Clears opengl buffers and texture.
     */
    void clearScreen();

    /**
     * These functions will be passed to opengl for interactions with the scene.
     */
    static void windowSizeCallback(GLFWwindow *window, int width, int height);

    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

    static void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);

    static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    static void keyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};


#endif //PGM_SCENE_H
