//
// Created by igor on 14/05/2020.
//

#ifndef PGM_MESH_SKINNED_DRAWABLE_HPP
#define PGM_MESH_SKINNED_DRAWABLE_HPP

#include "mesh_drawable.hpp"
#include "mesh_drawable_gpu_data/mesh_skinned_drawable_gpu_data.hpp"

namespace vcl {

class mesh_skinned_drawable : public mesh_drawable {
public:
    mesh_skinned_drawable();
    mesh_skinned_drawable(const mesh_skinned& mesh_cpu, GLuint shader = 0, GLuint texture_id = 0);

    void draw(camera_scene& camera);

    mesh_skinned_drawable_gpu_data data;
};

}

#endif //PGM_MESH_SKINNED_DRAWABLE_HPP
