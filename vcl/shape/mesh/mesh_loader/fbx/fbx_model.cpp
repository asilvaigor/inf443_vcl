//
// Created by igor on 15/05/2020.
//

#include "fbx_model.hpp"
#include "third_party/assimp/include/postprocess.h"

namespace vcl {

fbx_model::fbx_model(const std::string &filename, GLuint shader) : shader(shader) {
    auto flags = (unsigned int) aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                 aiProcess_FlipUVs | aiProcess_FindDegenerates;
    scene = importer.ReadFile(filename.c_str(), flags);
    if (!scene)
        throw std::invalid_argument("Could not find model " + filename);

    global_inverse_transform = scene->mRootNode->mTransformation;
    global_inverse_transform.Inverse();
    start_transform = global_inverse_transform;
    for (int i = 0; i < (int) scene->mNumMeshes; i++)
        load_mesh(scene->mMeshes[i]);

    last_time = 0;
    animation_time = 0;
    light = nullptr;
}

float fbx_model::draw(vcl::camera_scene &camera, float time) {
    if (!cur_animation.empty() && last_time > EPSILON) {
        updateBones(scene->mRootNode, global_inverse_transform);
        updateDrawables(camera);
        updateAnimationTime(time);
    }

//    std::cout << animation_map[cur_animation]->mDuration << "\n";
//    std::cout << animation_map[cur_animation]->mTicksPerSecond << "\n";
//    std::cout << animation_time << "\n";
//    std::cout << node_anim->mScalingKeys[scaling_idx].mTime;

    last_time = time;
    if (!cur_animation.empty())
        return (float) (animation_time / animation_map[cur_animation]->mDuration);
    else return 0.0f;
}

void fbx_model::set_light(std::shared_ptr<vcl::light_source> &l) {
    this->light = l;
}

void fbx_model::set_animation(const std::string &animationName) {
    if (animation_map.find(animationName) != animation_map.end()) {
        animation_time = 0;
        cur_animation = animationName;
        auto &animation = animation_map[animationName];

        // Updating animation node map
        animation_node_map.clear();
        for (int j = 0; j < (int) animation->mNumChannels; j++) {
            const aiNodeAnim *pNodeAnim = animation->mChannels[j];
            animation_node_map[pNodeAnim->mNodeName.C_Str()] = pNodeAnim;
        }

        // Updating transformations
        updateBones(scene->mRootNode, global_inverse_transform);

        return;
    }

    std::string error("Animation " + animationName + " does not exist. List of animations: [");
    for (auto &p : animation_map) {
        error.append(p.first);
        error.append(", ");
    }
    error.push_back(']');
    throw std::invalid_argument(error);
}

void fbx_model::transform(vcl::mat4 &m) {
    global_inverse_transform = m * start_transform;
}

void fbx_model::load_mesh(aiMesh *assimpMesh) {
    mesh_skinned mesh;
    const aiVector3D origin(0.0f, 0.0f, 0.0f);

    // Filling up position, normals and texture_uv
    for (uint i = 0; i < assimpMesh->mNumVertices; i++) {
        aiVector3D *pos = &(assimpMesh->mVertices[i]);
        aiVector3D *normal = &(assimpMesh->mNormals[i]);
        const aiVector3D *tex = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][i]) : &origin;

        *normal = global_inverse_transform * (*normal);
        *pos = global_inverse_transform * (*pos);

        mesh.position.push_back(vec3(pos->x, pos->y, pos->z));
        mesh.normal.push_back(vec3(normal->x, normal->y, normal->z));
        mesh.texture_uv.push_back(vec2(tex->x, tex->y));
    }

    // Filling up connectivity
    for (uint i = 0; i < assimpMesh->mNumFaces; i++) {
        const aiFace &face = assimpMesh->mFaces[i];
        mesh.connectivity.push_back({face.mIndices[0], face.mIndices[1],
                                     face.mIndices[2]});
    }

    // Filling up bones
    for (int i = 0; i < (int) assimpMesh->mNumBones; i++) {
        GLuint bone_idx;
        std::string bone_name(assimpMesh->mBones[i]->mName.data);

        // Saving the bones in a vector to pass to the shader
        if (bone_map.find(bone_name) == bone_map.end()) {
            bone_idx = bones.size();
            bone_info bi;
            bones.push_back(bi);
            bones[bone_idx].offset = assimpMesh->mBones[i]->mOffsetMatrix;
            bone_map[bone_name] = bone_idx;
        } else bone_idx = bone_map[bone_name];

        // Adding the bone info to each vertex
        mesh.bones.resize(mesh.position.size());
        for (GLuint j = 0; j < assimpMesh->mBones[i]->mNumWeights; j++) {
            auto vertex_id = (int) assimpMesh->mBones[i]->mWeights[j].mVertexId;
            float w = assimpMesh->mBones[i]->mWeights[j].mWeight;
            mesh.bones[vertex_id].insert(bone_idx, w);
        }
    }

    meshes.push_back(mesh);
    drawables.emplace_back(mesh_skinned_drawable(mesh, shader));
    drawables.back().uniform.bones.resize(bones.size());

    // Storing animations
    for (uint i = 0; i < scene->mNumAnimations; i++)
        animation_map[scene->mAnimations[i]->mName.C_Str()] = scene->mAnimations[i];
}

void fbx_model::updateBones(const aiNode *node, const aiMatrix4x4 &parent_transform) {
    std::string node_name(node->mName.C_Str());
    aiMatrix4x4 node_transformation;
    const aiNodeAnim *node_animation = animation_node_map[node_name];

    if (node_animation) {
        auto scaling = calc_interpolated_scaling(node_animation);
        auto rotation = calc_interpolated_rotation(node_animation);
        auto position = calc_interpolated_position(node_animation);
        node_transformation = aiMatrix4x4(scaling, rotation, position);
    }

    aiMatrix4x4 global_transformation = parent_transform * node_transformation;

    if (bone_map.find(node_name) != bone_map.end()) {
        GLuint bone_idx = bone_map[node_name];
        bones[bone_idx].transform = global_transformation * bones[bone_idx].offset;
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        updateBones(node->mChildren[i], global_transformation);
}

void fbx_model::updateDrawables(vcl::camera_scene &camera) {
    for (auto &drawable : drawables) {
        for (int i = 0; i < (int) bones.size(); i++)
            drawable.uniform.bones[i] = mat4::from_assimp(bones[i].transform);
        drawable.uniform.lights = {light};
        drawable.draw(camera);
    }
}

void fbx_model::updateAnimationTime(float &time) {
    auto ticks_per_sec = (float) (animation_map[cur_animation]->mTicksPerSecond != 0 ?
                                  animation_map[cur_animation]->mTicksPerSecond : 25.0f);
    animation_time += (time - last_time) * ticks_per_sec;
    animation_time = fmod(animation_time, (float) animation_map[cur_animation]->mDuration);
}

aiVector3D fbx_model::calc_interpolated_scaling(const aiNodeAnim *node_anim) const {

    if (node_anim->mNumScalingKeys == 1)
        return node_anim->mScalingKeys[0].mValue;

    uint scaling_idx = 0;
    for (uint i = 0; i < node_anim->mNumScalingKeys - 1; i++)
        if (animation_time < (float) node_anim->mScalingKeys[i + 1].mTime) {
            scaling_idx = i;
            break;
        }
    uint next_scaling_idx = (scaling_idx + 1);

    assert(next_scaling_idx < node_anim->mNumScalingKeys);
    auto dt = (float) (node_anim->mScalingKeys[next_scaling_idx].mTime -
                       node_anim->mScalingKeys[scaling_idx].mTime);
    float factor = (animation_time - (float) node_anim->mScalingKeys[scaling_idx].mTime) / dt;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D &start = node_anim->mScalingKeys[scaling_idx].mValue;
    const aiVector3D &end = node_anim->mScalingKeys[next_scaling_idx].mValue;
    aiVector3D delta = end - start;
    return start + factor * delta;
}

aiQuaternion fbx_model::calc_interpolated_rotation(const aiNodeAnim *node_anim) const {
    if (node_anim->mNumRotationKeys == 1)
        return node_anim->mRotationKeys[0].mValue;

    uint rot_idx = 0;
    for (uint i = 0; i < node_anim->mNumRotationKeys - 1; i++)
        if (animation_time < (float) node_anim->mRotationKeys[i + 1].mTime) {
            rot_idx = i;
            break;
        }
    uint next_rot_idx = (rot_idx + 1);

    assert(next_rot_idx < node_anim->mNumRotationKeys);
    auto dt = (float) (node_anim->mRotationKeys[next_rot_idx].mTime -
                       node_anim->mRotationKeys[rot_idx].mTime);
    float factor = (animation_time - (float) node_anim->mRotationKeys[rot_idx].mTime) / dt;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiQuaternion &startRot = node_anim->mRotationKeys[rot_idx].mValue;
    const aiQuaternion &endRot = node_anim->mRotationKeys[next_rot_idx].mValue;
    aiQuaternion rot;
    aiQuaternion::Interpolate(rot, startRot, endRot, factor);
    return rot.Normalize();
}

aiVector3D fbx_model::calc_interpolated_position(const aiNodeAnim *node_anim) const {
    if (node_anim->mNumPositionKeys == 1)
        return node_anim->mPositionKeys[0].mValue;

    uint position_idx = 0;
    for (uint i = 0; i < node_anim->mNumPositionKeys - 1; i++)
        if (animation_time < (float) node_anim->mPositionKeys[i + 1].mTime) {
            position_idx = i;
            break;
        }
    uint next_position_idx = (position_idx + 1);

    assert(next_position_idx < node_anim->mNumPositionKeys);
    auto dt = (float) (node_anim->mPositionKeys[next_position_idx].mTime -
                       node_anim->mPositionKeys[position_idx].mTime);
    float factor = (animation_time - (float) node_anim->mPositionKeys[position_idx].mTime) / dt;
    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D &start = node_anim->mPositionKeys[position_idx].mValue;
    const aiVector3D &end = node_anim->mPositionKeys[next_position_idx].mValue;
    aiVector3D delta = end - start;
    return start + factor * delta;
}

}