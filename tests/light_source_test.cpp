//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/box/Box.h"
#include "objects/terrain/Terrain.h"

int main() {
    vcl::vec3 lightPos(2, 0, 2);
    vcl::vec3 lightDir(-1, 0, -1);
    vcl::light_source light;
    vcl::camera_scene camera;
    camera.perspective = vcl::perspective_structure(
            40 * 3.14f / 180, 1.28, 0.01f, 500.0f);
    light.update(camera, lightPos, lightDir);

    std::cout << light.get_matrix() * vcl::vec4(0, 0, 0, 1) << std::endl;

    return 0;
}
