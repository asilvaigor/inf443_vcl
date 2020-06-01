//
// Created by igor on 14/05/2020.
//

#ifndef PGM_MESH_SKINNED_H
#define PGM_MESH_SKINNED_H

#include "vcl/math/math.hpp"
#include "vcl/wrapper/glad/glad.hpp"
#include "mesh.hpp"
#include "third_party/assimp/include/anim.h"

#define NUM_BONES_PER_VERTEX 4

namespace vcl {

struct bone_info {
    aiMatrix4x4 offset;
    aiMatrix4x4 transform;
};

struct vertex_bone_data {
    GLuint idx[NUM_BONES_PER_VERTEX]{};
    float weights[NUM_BONES_PER_VERTEX]{};
    int cnt;

    vertex_bone_data();

    void insert(GLuint id, float w);
};

class mesh_skinned : public mesh {
public:
    buffer<vertex_bone_data> bones;

    void fill_empty_fields() override;
};

}

#endif //PGM_MESH_SKINNED_H
