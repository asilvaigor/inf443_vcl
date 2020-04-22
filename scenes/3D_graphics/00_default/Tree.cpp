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
    branchesDrawable.uniform.shading = {0.3, 0.6, 0.0, 128};
    branchesDrawable.uniform.light = {10, 10, 10};

    auto leavesMesh = trunk.toLeavesMesh();
    leavesDrawable = vcl::mesh_drawable(leavesMesh);
    leavesDrawable.shader = shaders["mesh"];
    leavesDrawable.uniform.shading = {0.3, 0.8, 0.0, 128};
    leavesDrawable.uniform.light = {10, 10, 10};
    leavesDrawable.uniform.color = {0.04, 0.4, 0.14};

    auto snowyLeavesMesh = trunk.toSnowyLeavesMesh();
    snowyLeavesDrawable = vcl::mesh_drawable(snowyLeavesMesh);
    snowyLeavesDrawable.shader = shaders["mesh"];
    snowyLeavesDrawable.uniform.shading = {0.4, 0.6, 0.0, 128};
    snowyLeavesDrawable.uniform.light = {10, 10, 0};
    snowyLeavesDrawable.uniform.color = {1, 1, 1};

    std::cout << "Number of vertices: branches " << branchesMesh.position.size() << " leaves " <<
              leavesMesh.position.size() << " " << snowyLeavesMesh.position.size() << std::endl;

    branchTextureId = create_texture_gpu(vcl::image_load_png("scenes/3D_graphics/00_default/wood_texture.png"),
            GL_REPEAT, GL_REPEAT);
    leafTextureId = create_texture_gpu(vcl::image_load_png("scenes/3D_graphics/00_default/leaf_texture.png"),
            GL_REPEAT, GL_REPEAT);
    snowTextureId = create_texture_gpu(vcl::image_load_png("scenes/3D_graphics/00_default/snow_texture.png"),
                                       GL_REPEAT, GL_REPEAT);
    const vcl::image_raw white{1, 1,
                               vcl::image_color_type::rgba, {255, 255, 255, 255}};
    whiteTextureId = create_texture_gpu(white);
}

void Tree::draw(const vcl::camera_scene &camera) {
    vcl::draw(leafDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, branchTextureId);
    vcl::draw(branchesDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, leafTextureId);
    vcl::draw(leavesDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, snowTextureId);
    vcl::draw(snowyLeavesDrawable, camera);
    glBindTexture(GL_TEXTURE_2D, whiteTextureId);
}
