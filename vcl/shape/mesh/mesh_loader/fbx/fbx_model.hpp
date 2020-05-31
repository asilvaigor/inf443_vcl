//
// Created by igor on 15/05/2020.
//

#ifndef PGM_FBX_MODEL_HPP
#define PGM_FBX_MODEL_HPP

#include <map>
#include "../../mesh_drawable/mesh_skinned_drawable.hpp"
#include "third_party/assimp/include/mesh.h"
#include "third_party/assimp/include/scene.h"
#include "third_party/assimp/include/Importer.hpp"

namespace vcl {

class fbx_model {
public:
    fbx_model(const std::string &filename, GLuint shader, vec3 &pos);

    float draw(vcl::camera_scene &camera, float time);

    void set_light(std::shared_ptr<vcl::light_source> &light);

    void set_animation(const std::string &animation);

    void transform(vcl::mat4 &m);

    vcl::vec3 get_position() const;

    std::vector<mesh_skinned> meshes;
    std::vector<mesh_skinned_drawable> drawables;
    std::vector<GLuint> textures;
    std::vector<bone_info> bones;

private:
    Assimp::Importer importer;
    const aiScene *scene;
    aiMatrix4x4 global_inverse_transform;
    aiMatrix4x4 start_transform;
    std::map<std::string, GLuint> bone_map;
    std::map<std::string, const aiNodeAnim *> animation_node_map;
    std::map<std::string, const aiAnimation *> animation_map;
    std::string cur_animation;
    GLuint shader;
    std::shared_ptr<vcl::light_source> light;
    float last_time;
    float animation_time;

    void load_mesh(aiMesh *assimpMesh);

    void updateBones(const aiNode *pNode, const aiMatrix4x4 &parent_transform);

    void updateDrawables(vcl::camera_scene &camera);

    void updateAnimationTime(float &time);

    aiVector3D calc_interpolated_scaling(const aiNodeAnim *node_anim) const;

    aiQuaternion calc_interpolated_rotation(const aiNodeAnim *node_anim) const;

    aiVector3D calc_interpolated_position(const aiNodeAnim *node_anim) const;

};

}

#endif //PGM_FBX_MODEL_HPP
