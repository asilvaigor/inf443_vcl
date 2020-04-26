//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/box/Box.h"
#include "objects/terrain/Terrain.h"

int main() {
    // Check out <https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping> for the implementation
    vcl::light_source light({10, 10, 10});
    std::cout << light.matrix << std::endl;
    auto v = vcl::transpose(light.matrix) * vcl::vec4(0, 0, 0, 1);
    std::cout << v << std::endl;
    assert(fabs(v.x) < 0.0001 && fabs(v.y) < 0.0001 && fabs(v.z - 4.021695) < 0.0001);

    return 0;
}
