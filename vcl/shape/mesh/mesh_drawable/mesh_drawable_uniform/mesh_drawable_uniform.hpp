#pragma once

#include <memory>
#include "vcl/math/math.hpp"
#include "vcl/shape/mesh/mesh_structure/mesh_skinned.hpp"

namespace vcl
{

struct shading_mesh {
    shading_mesh(float ambiant=0.2f, float diffuse=0.8f, float specular=0.5f, int specular_exponent=128);

    float ambiant;
    float diffuse;
    float specular;
    int specular_exponent;
};

struct mesh_drawable_uniform {

    mesh_drawable_uniform();


    affine_transform transform;
    vec3 color;
    shading_mesh shading;
    float color_alpha;
    std::vector<std::shared_ptr<light_source>> lights;
    std::vector<mat4> bones;
    int current_light;

};

}
