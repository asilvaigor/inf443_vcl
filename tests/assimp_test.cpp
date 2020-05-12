#include <iostream>
#include "third_party/assimp/include/Importer.hpp"
#include "third_party/assimp/include/scene.h"
#include "third_party/assimp/include/postprocess.h"

int main() {
    Assimp::Importer Importer;
    std::string file("../src/assets/models/polarbear.fbx");

    const aiScene* pScene = Importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_FindDegenerates);

    if (pScene) {
        for (int i = 0; i < (int) pScene->mNumMeshes; i++)
            std::cout << pScene->mMeshes[i]->mNumVertices << std::endl;
    } else throw std::invalid_argument("Error parsing " + file + std::string(Importer.GetErrorString()));

    return 0;
}