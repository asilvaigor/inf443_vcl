//
// Created by igor on 22/04/2020.
//

#include "Scene.h"
#include "utils/SingletonException.h"

bool Scene::exists = false;

Scene &Scene::getInstance(std::string windowTitle) {
    if (exists)
        throw SingletonException("Scene");
    exists = true;
    static Scene instance(windowTitle);
    return instance;
}

Scene::Scene(std::string &windowTitle) {
    gui = SceneGui::getInstance(windowTitle);
    shaders = Shaders::getInstance();

    // Creating rendering stuff
    whiteTexture = std::make_shared<Texture>();
    grid = std::make_shared<Grid>(*shaders);

    vcl::light_source light({100, 0, 100}, {-1, 0, -1},
            gui->getCamera().perspective.z_near, gui->getCamera().perspective.z_far);
    cascadeShadow = std::make_shared<CascadeShadow>(light, 3072);
}

Shaders &Scene::getShaders() {
    return *shaders;
}

void Scene::addObject(std::shared_ptr<Object> &object) {
    if (object->isMovable())
        movableObjects.push_back(object);
    else stillObjects.push_back(object);
}

void Scene::display() {
    glViewport(0, 0, gui->getWindowWidth(), gui->getWindowHeight());
    while (gui->isRunning()) {
        gui->update();
        updateScene();
        gui->render();
    }
}

void Scene::updateScene() {
    float time = glfwGetTime();

    cascadeShadow->update(movableObjects, stillObjects, gui, shaders, time);
    shaders->override("wireframe", gui->showVertices());
    if (gui->showGrid())
        grid->draw(gui->getCamera());

    whiteTexture->bind();
    for (auto &obj : stillObjects) {
        obj->draw(gui->getCamera());
        whiteTexture->bind();
    }
    for (auto &obj : movableObjects) {
        obj->draw(gui->getCamera(), time);
        whiteTexture->bind();
    }
}
