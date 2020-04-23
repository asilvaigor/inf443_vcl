#include "scene/Scene.h"

int main() {
    auto scene = Scene::getInstance("Winter Wonderland");
    scene.display();
    return 0;
}
