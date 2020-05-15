//
// Created by igor on 15/05/2020.
//

#include "fbx_model.hpp"
#include "third_party/assimp/include/Importer.hpp"
#include "third_party/assimp/include/postprocess.h"

namespace vcl {

fbx_model::fbx_model(const std::string &filename) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename.c_str(), (unsigned int) aiProcess_Triangulate |
                                                               aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                               aiProcess_FindDegenerates);
    if (!scene)
        throw std::invalid_argument("Could not find model " + filename);

    global_inverse_transform = scene->mRootNode->mTransformation;
    global_inverse_transform.Inverse();
    for (int i = 0; i < (int) scene->mNumMeshes; i++)
        load_mesh(scene->mMeshes[i]);
}

void fbx_model::load_mesh(aiMesh *assimpMesh) {
    mesh_skinned mesh;
    const aiVector3D origin(0.0f, 0.0f, 0.0f);

    // Filling up position, normals and texture_uv
    for (uint i = 0; i < assimpMesh->mNumVertices; i++) {
        const aiVector3D *pos = &(assimpMesh->mVertices[i]);
        const aiVector3D *normal = &(assimpMesh->mNormals[i]);
        const aiVector3D *tex = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][i]) : &origin;

        mesh.position.push_back(vec3(pos->x, pos->y, pos->z));
        mesh.normal.push_back(vec3(normal->x, normal->y, normal->z));
        mesh.texture_uv.push_back(vec2(tex->x, tex->y));
    }

    // Filling up connectivity
    for (uint i = 0; i < assimpMesh->mNumFaces; i++) {
        const aiFace &face = assimpMesh->mFaces[i];
        mesh.connectivity.push_back({face.mIndices[0], face.mIndices[1], face.mIndices[2]});
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
            bones[bone_idx].offset = mat4::from_assimp(assimpMesh->mBones[i]->mOffsetMatrix);
            bone_map[bone_name] = bone_idx;
        } else bone_idx = bone_map[bone_name];

        // Adding the bone info to each vertex
        for (int j = 0; j < (int) assimpMesh->mBones[i]->mNumWeights; j++) {
            int vertex_id = assimpMesh->mBones[i]->mWeights[j].mVertexId;
            float w = assimpMesh->mBones[i]->mWeights[j].mWeight;
            mesh.bones[vertex_id].insert(bone_idx, w);
        }
    }

    // TODO: criar o drawable, bones para uniform

    meshes.push_back(mesh);
}

}