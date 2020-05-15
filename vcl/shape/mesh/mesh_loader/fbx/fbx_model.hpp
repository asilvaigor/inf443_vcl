//
// Created by igor on 15/05/2020.
//

#ifndef PGM_FBX_MODEL_HPP
#define PGM_FBX_MODEL_HPP

#include <map>
#include "../../mesh_drawable/mesh_skinned_drawable.hpp"
#include "third_party/assimp/include/mesh.h"
#include "third_party/assimp/include/scene.h"

namespace vcl {

class fbx_model {
public:
    fbx_model(const std::string &filename);

    void draw(vcl::camera_scene &camera, float time);

    void set_shader(GLuint shader);

    void set_light(vcl::light_source &light);

    void set_animation(const std::string &anim);

    std::vector<mesh_skinned> meshes;
    std::vector<mesh_skinned_drawable> drawables;
    std::vector<GLuint> textures;

private:
    aiMatrix4x4 global_inverse_transform;
    std::map<std::string, GLuint> bone_map;
    std::vector<bone_info> bones;

    void load_mesh(aiMesh* assimpMesh);
};

}

#endif //PGM_FBX_MODEL_HPP
