//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

void Tree::setupData(std::map<std::string, GLuint> &shaders, TreeSpecies &species, vcl::vec3 position) {
    TurtleGraphics turtle(position);
    Branch trunk(species, turtle);
    auto branchesMesh = trunk.toMesh();
    branchesDrawable = vcl::mesh_drawable(branchesMesh);
    branchesDrawable.shader = shaders["mesh"];
    auto leavesMesh = trunk.toLeavesMesh();
    leavesDrawable = vcl::mesh_drawable(leavesMesh);
    leavesDrawable.shader = shaders["mesh"];

    std::cout << "Number of vertices: branches " << branchesMesh.position.size() << " leaves " <<
              leavesMesh.position.size() << std::endl;

    textureId = create_texture_gpu(vcl::image_load_png("scenes/3D_graphics/00_default/wood_texture.png"),
                                   GL_REPEAT, GL_REPEAT);
    const vcl::image_raw white{1, 1,
                               vcl::image_color_type::rgba, {255, 255, 255, 255}};
    textureWhiteId = create_texture_gpu(white);
}

void Tree::draw(const vcl::camera_scene &camera) {
    vcl::draw(leafDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, textureId);
    vcl::draw(branchesDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, textureWhiteId);
    vcl::draw(leavesDrawable, camera);
}
