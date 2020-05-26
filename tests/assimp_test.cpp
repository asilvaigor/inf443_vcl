#include <iostream>
#include "third_party/assimp/include/Importer.hpp"
#include "third_party/assimp/include/scene.h"
#include "third_party/assimp/include/postprocess.h"

void printNodes(aiNode* n) {
    if (n->mNumChildren > 0)
        std::cout << "(";
    std::cout << n->mName.C_Str();
    for (int i = 0; i < (int) n->mNumChildren; i++) {
        std::cout << " ";
        printNodes(n->mChildren[i]);
    }
    if (n->mNumChildren > 0)
        std::cout << ")";
}

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
        for (int i = 0; i < (int) pScene->mNumAnimations; i++) {
            std::cout << "- name: " << pScene->mAnimations[i]->mName.data << ", nChannels: " <<
                      pScene->mAnimations[i]->mNumChannels << std::endl;
            for (int j = 0; j < (int) pScene->mAnimations[i]->mNumChannels; j++)
                std::cout << pScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str() << " ";
            std::cout << std::endl;
        }

        std::cout << "MATERIALS: " << pScene->mNumMaterials << std::endl;
        std::cout << "TEXTURES: " << pScene->mNumTextures << std::endl;
        std::cout << "CAMERAS: " << pScene->mNumCameras << std::endl;
        std::cout << "LIGHTS: " << pScene->mNumLights << std::endl;
        std::cout << "NODES: ";
        printNodes(pScene->mRootNode);
        std::cout << std::endl;
    } else throw std::invalid_argument("Error parsing " + file + std::string(Importer.GetErrorString()));

    return 0;
}