//
// Created by igor on 22/04/2020.
//

#ifndef PGM_TEXTURE_H
#define PGM_TEXTURE_H

#include "vcl.hpp"

/**
 * Texture to bind in opengl.
 * WARNING: THIS CLASS CANNOT BE INSTANTIATED BEFORE Scene, OR IT WILL PROVOKE A SEGFAULT.
 */
class Texture {
public:
    /**
     * Creates a texture with a given color.
     * @param r
     * @param g
     * @param b
     */
    explicit Texture(int r, int g, int b);

    /**
     * Loads the texture's png file.
     * @param name Name of the png file located in std/assets/textures/, without the .png extension.
     * @param wrap_s Opengl id of the texture placement.
     * @param wrap_t Opengl id of the texture placement.
     */
    explicit Texture(const std::string& name, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);

    /**
     * Binds texture for it to be rendered.
     */
    void bind() const;

    GLuint getId() const;

private:
    GLuint id;
};


#endif //PGM_TEXTURE_H
