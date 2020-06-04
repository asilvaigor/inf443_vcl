//
// Created by igor on 22/04/2020.
//

#include <chrono>
#include <thread>
#include "Scene.h"
#include "objects/forest/Forest.h"
#include "objects/boid/Boid.h"
#include "utils/SingletonException.h"
#include "utils/Constants.h"

bool Scene::exists = false;
bool Scene::deterministic = true;

Scene &Scene::getInstance(std::string windowTitle) {
    if (exists)
        throw SingletonException("Scene");
    exists = true;
    static Scene instance(windowTitle);
    return instance;
}

Scene::Scene(std::string &windowTitle) {
    gui = SceneGui::getInstance(windowTitle);

    std::cout << "Initializing scene... " << std::flush;

    shaders = Shaders::getInstance();

    // Creating rendering stuff
    whiteTexture = std::make_shared<Texture>(255, 255, 255);
    grid = std::make_shared<Grid>(*shaders);
    simulatedTime = 0;

    vcl::light_source light({100, 0, 100}, {-1, 0, -1},
            gui->getCamera().get_perspective().z_near, gui->getCamera().get_perspective().z_far);
    cascadeShadow = std::make_shared<CascadeShadow>(light, 2048);
    gui->getCamera().set_z_near_far_list(cascadeShadow->getZNearFarList());

    std::cout << "Finished" << std::endl;
}

Shaders &Scene::getShaders() {
    return *shaders;
}

std::shared_ptr<SceneGui> Scene::getGui() {
    return gui;
}

void Scene::addObject(std::shared_ptr<Object> &object) {
    if (object->isMovable())
        movableObjects.push_back(object);
    else stillObjects.push_back(object);
}

void Scene::display() {
    gui->open();
    while (gui->isRunning()) {
        gui->update();
        updateScene();
        gui->render();
    }
}

void Scene::haveCameraFollow(std::shared_ptr<Object> &object) {
    gui->haveCameraFollow(object);
}

void Scene::updateScene() {
    auto initialTime = (float) glfwGetTime();
    simulatedTime += 1.0f / Constants::FPS;

    for (auto &obj : movableObjects)
        obj->update(simulatedTime);

    cascadeShadow->update(movableObjects, stillObjects, gui, shaders, simulatedTime);
    shaders->override("wireframe", gui->showVertices());
    if (gui->showGrid())
        grid->draw(gui->getCamera());

    whiteTexture->bind();
    for (auto &obj : stillObjects) {
        // Forest should be decomposed in its objects
        obj->draw(gui->getCamera());
        whiteTexture->bind();
    }
//    std::cout << glfwGetTime() - time << std::endl;
    for (auto &obj : movableObjects) {
        // Boid should be decomposed in its objects
        auto *b = dynamic_cast<Boid *>(obj.get());
        if (b != nullptr)
            for (auto &o : b->getObjects())
                o->draw(gui->getCamera());
        else obj->draw(gui->getCamera());
        whiteTexture->bind();
    }

    // Limiting fps
    auto finalTime = (float) glfwGetTime();
    std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / Constants::FPS - (finalTime - initialTime)));
}
