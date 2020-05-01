//
// Created by igor on 23/04/2020.
//

#include "vcl/opengl/texture/image/image.hpp"
#include "vcl/wrapper/glad/glad.hpp"
#include "depth_maps.hpp"

namespace vcl {

depth_maps::depth_maps(int n, GLuint resolution) : resolution(resolution) {
    // Frame buffer object
    glGenFramebuffers(1, &fbo);

    // Layouts
    for (int i = 0; i < n; i++)
        layouts.push_back(i + 1);

    // Creating textures
    texture_ids = new GLuint[n];
    glGenTextures(n, texture_ids);
    for (int i = 0; i < n; i++) {
        glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // Attaching it to the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_ids[0], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Auxiliar variables
    nBinded = 0;
    binded.assign(n, false);
}

depth_maps::~depth_maps() {
    delete[] texture_ids;
}

void depth_maps::bind(int i) {
    if (binded[i]) return;
    if (nBinded == 0) {
        glViewport(0, 0, resolution, resolution);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glCullFace(GL_FRONT);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_ids[i], 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    nBinded++;
    binded[i] = true;
}

void depth_maps::unbind_all(int windowWidth, int windowHeight) {
    for (int i = 0; i < (int) layouts.size(); i++)
        unbind(i, windowWidth, windowHeight);
}

void depth_maps::unbind(int i, int windowWidth, int windowHeight) {
    if (!binded[i]) return;
    binded[i] = false;
    nBinded--;

    // Binding the texture to be used
    glActiveTexture(GL_TEXTURE0 + layouts[i]);
    glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
    glActiveTexture(GL_TEXTURE0);

    if (nBinded == 0) {
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

}