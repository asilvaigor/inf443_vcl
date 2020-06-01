//
// Created by igor on 14/05/2020.
//

#include "mesh_skinned.hpp"

namespace vcl {

vertex_bone_data::vertex_bone_data() {
    for (int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
        idx[i] = 0;
        weights[i] = 0;
    }
    cnt = 0;
}

void vertex_bone_data::insert(GLuint id, float w) {
    if (cnt < 4) {
        idx[cnt] = id;
        weights[cnt++] = w;
    }
}

void mesh_skinned::fill_empty_fields() {
    mesh::fill_empty_fields();

    const size_t N = position.size();
    if (bones.size() < N) {
        bones.resize(N);
        for (size_t k = 0; k < N; ++k)
            bones[k] = vertex_bone_data();
    }
}

}