//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

void Tree::setupData(std::map<std::string, GLuint> &shaders, TreeSpecies &species) {
    Branch trunk(species);
    auto m = trunk.toMesh();
    std::cout << "Number of vertices: " << m.position.size() << std::endl;
    drawable = vcl::mesh_drawable(trunk.toMesh());
    drawable.shader = shaders["mesh"];

    textureId = create_texture_gpu(vcl::image_load_png("scenes/3D_graphics/00_default/wood_texture.png"),
            GL_REPEAT, GL_REPEAT);
}

void Tree::draw(const vcl::camera_scene &camera) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    vcl::draw(drawable, camera);
}
