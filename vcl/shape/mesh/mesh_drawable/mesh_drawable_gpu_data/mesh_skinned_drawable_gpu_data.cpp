//
// Created by igor on 14/05/2020.
//

#include "opengl/debug/opengl_debug.hpp"
#include "mesh_skinned_drawable_gpu_data.hpp"

namespace vcl {

mesh_skinned_drawable_gpu_data::mesh_skinned_drawable_gpu_data() : mesh_drawable_gpu_data(), vbo_skeleton(0) {}

mesh_skinned_drawable_gpu_data::mesh_skinned_drawable_gpu_data(const mesh_skinned &mesh_cpu_arg) :
        mesh_drawable_gpu_data(mesh_cpu_arg) {
    // temp copy of the mesh to fill all empty fields
    mesh_skinned mesh_cpu = mesh_cpu_arg;
    mesh_cpu.fill_empty_fields();

    // Fill VBO for skeleton
    glGenBuffers(1, &vbo_skeleton);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skeleton);
    if (!mesh_cpu.bones.empty())
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_bone_data) * mesh_cpu.bones.size(),
                     &mesh_cpu.bones[0], GL_STATIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);

    // Bones ids at layout 4
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skeleton);
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 4, GL_INT, sizeof(vertex_bone_data), (const GLvoid *) nullptr);

    // Bones weights at layout 5
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skeleton);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_bone_data), (const GLvoid *) 16);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}