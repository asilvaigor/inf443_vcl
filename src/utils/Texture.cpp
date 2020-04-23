//
// Created by igor on 22/04/2020.
//

#include "Texture.h"


Texture::Texture(const std::string& name, GLint wrap_s, GLint wrap_t) {
    if (name.empty()) {
        id = vcl::create_texture_gpu(vcl::image_raw(1, 1,
                vcl::image_color_type::rgba, {255, 255, 255, 255}));
    } else {
        std::string path = "../src/assets/textures/";
        std::string extension = ".png";
        id = create_texture_gpu(vcl::image_load_png(path + name + extension), wrap_s, wrap_t);
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
