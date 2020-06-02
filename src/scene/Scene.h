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
#include "SceneGui.h"
#include "CascadeShadow.h"

/**
 * Singleton class which contains a scene with its gui and objects such as shader, light etc.
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
     * Gets the GUI
     * @return GUI
     */
    std::shared_ptr<SceneGui> getGui();

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

    /**
     * This will make the camera follow a given object.
     * @param object
     */
    void haveCameraFollow(std::shared_ptr<Object> &object);

    static bool deterministic;

private:
    std::shared_ptr<SceneGui> gui;
    std::vector<std::shared_ptr<Object>> stillObjects;
    std::vector<std::shared_ptr<Object>> movableObjects;
    std::shared_ptr<Shaders> shaders;
    std::shared_ptr<Texture> whiteTexture;
    std::shared_ptr<Grid> grid;
    std::shared_ptr<CascadeShadow> cascadeShadow;

    static bool exists; // Singleton bool

    /**
     * Will initialize gui, camera and shaders.
     * @param windowTitle
     */
    explicit Scene(std::string &windowTitle);

    /**
     * Updates the objects in the scene using a depth map.
     */
    void updateScene();
};


#endif //PGM_SCENE_H
