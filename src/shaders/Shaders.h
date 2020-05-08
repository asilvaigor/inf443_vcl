//
// Created by igor on 22/04/2020.
//

#ifndef PGM_SHADERS_H
#define PGM_SHADERS_H

#include <memory>
#include <unordered_map>
#include "vcl.hpp"

/**
 * Singleton class, Stores all shaders in a map for easy access.
 * WARNING: THIS CLASS CANNOT BE INSTANTIATED BEFORE Scene, OR IT WILL PROVOKE A SEGFAULT.
 */
class Shaders {
public:
    /**
     * Gets the instance of the singleton class, or throws an exception.
     * @return Shared pointer to Shaders.
     */
    static std::shared_ptr<Shaders> &getInstance();

    /**
     * Will activate the shader for all objects on the scene, overriding the current shaders.
     * @param shader Name of the shader
     * @param override
     */
    void override(std::string shader, bool override);

    /**
     * Checks if the shaders are being overridden by another.
     * @return
     */
    bool isOverridden();

    /**
     * Works as map.
     * @param s String with the name of the folder the shader is located.
     * @return Id of the shader.
     */
    GLuint& operator[](const std::string& s);

private:
    std::unordered_map<std::string, GLuint> shaders;
    static std::shared_ptr<Shaders> ptr;

    /**
     * Processes all shaders located in the src/shaders folders and stores them in the map.
     * SEGFAULT when trying to compile the mesh files.
     */
    Shaders();
};


#endif //PGM_SHADERS_H
