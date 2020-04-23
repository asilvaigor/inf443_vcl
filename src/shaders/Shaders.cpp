//
// Created by igor on 22/04/2020.
//

#include <cstdio>
#include <dirent.h>
#include "Shaders.h"
#include "utils/SingletonException.h"

std::shared_ptr<Shaders> Shaders::ptr;

std::shared_ptr<Shaders> & Shaders::getInstance() {
    if (ptr != nullptr)
        throw SingletonException("Shaders");
    ptr.reset(new Shaders());
    return ptr;
}

Shaders::Shaders() {
    // Iterating through all dirs in the shaders dir to create meshes.
    const std::string path = "../src/shaders/";
    struct dirent *entry;
    DIR *dp = opendir(path.c_str());
    while ((entry = readdir(dp))) {
        std::string dir(entry->d_name);
        if (dir != ".." && dir != "." && dir != "Shaders.cpp" && dir != "Shaders.h") {
            std::string vertFile = path + dir + "/shader.vert.glsl";
            std::string geomFile = path + dir + "/shader.geom.glsl";
            std::string fragFile = path + dir + "/shader.frag.glsl";
            if (fopen((geomFile).c_str(), "r"))
                shaders[dir] = vcl::create_shader_program(vertFile, geomFile, fragFile);
            else shaders[dir] = vcl::create_shader_program(vertFile, fragFile);
        }
    }
    closedir(dp);
}

void Shaders::showWireframe(bool show) {
    if (show)
        vcl::mesh_drawable::overrideShader(shaders["wireframe"]);
    else vcl::mesh_drawable::overrideShader(0);
}

GLuint &Shaders::operator[](const std::string& s) {
    return shaders[s];
}

