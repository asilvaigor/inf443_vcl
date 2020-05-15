//
// Created by igor on 14/05/2020.
//

#ifndef PGM_MESH_SKINNED_DRAWABLE_GPU_DATA_H
#define PGM_MESH_SKINNED_DRAWABLE_GPU_DATA_H

#include "../../mesh_structure/mesh_skinned.hpp"
#include "mesh_drawable_gpu_data.hpp"

namespace vcl {

class mesh_skinned_drawable_gpu_data : public mesh_drawable_gpu_data {
public:
    mesh_skinned_drawable_gpu_data();
    mesh_skinned_drawable_gpu_data(const mesh_skinned &mesh_cpu_arg);

    GLuint vbo_skeleton; // (id) bones info
};

}


#endif //PGM_MESH_SKINNED_DRAWABLE_GPU_DATA_H
