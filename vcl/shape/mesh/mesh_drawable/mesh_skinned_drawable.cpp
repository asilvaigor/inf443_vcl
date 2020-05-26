//
// Created by igor on 14/05/2020.
//

#include "mesh_skinned_drawable.hpp"
#include "vcl/opengl/opengl.hpp"

namespace vcl {

mesh_skinned_drawable::mesh_skinned_drawable() : data() {
    shader = 0;
    texture_id = 0;
}

mesh_skinned_drawable::mesh_skinned_drawable(const vcl::mesh_skinned &mesh_cpu, GLuint shader_arg,
                                                  GLuint texture_id_arg) : data(mesh_cpu) {
    shader = shader_arg;
    texture_id = texture_id_arg;
}

void mesh_skinned_drawable::draw(const camera_scene& camera) {
    load_data(*this, camera, shader, texture_id);
    vcl::draw(data); opengl_debug();
}



}