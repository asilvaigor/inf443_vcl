#include <iostream>
#include "third_party/assimp/include/Importer.hpp"
#include "third_party/assimp/include/scene.h"
#include "third_party/assimp/include/postprocess.h"

int main() {
    Assimp::Importer Importer;
    std::string file("../src/assets/models/bear.fbx");

    const aiScene *pScene = Importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                            aiProcess_FlipUVs | aiProcess_FindDegenerates);

    if (pScene) {
        std::cout << "MESHES: " << pScene->mNumMeshes << std::endl;
        for (int i = 0; i < (int) pScene->mNumMeshes; i++)
            std::cout << "- name: " << pScene->mMeshes[i]->mName.data << ", nVertex: " <<
                      pScene->mMeshes[i]->mNumVertices << std::endl;

        std::cout << "ANIMATIONS: " << pScene->mNumAnimations << std::endl;
        for (int i = 0; i < (int) pScene->mNumAnimations; i++)
            std::cout << "- name: " << pScene->mAnimations[i]->mName.data << ", nChannels: " <<
                      pScene->mAnimations[i]->mNumChannels << std::endl;

        std::cout << "MATERIALS: " << pScene->mNumMaterials << std::endl;
        std::cout << "TEXTURES: " << pScene->mNumTextures << std::endl;
        std::cout << "CAMERAS: " << pScene->mNumCameras << std::endl;
        std::cout << "LIGHTS: " << pScene->mNumLights << std::endl;
    } else throw std::invalid_argument("Error parsing " + file + std::string(Importer.GetErrorString()));

    return 0;
}