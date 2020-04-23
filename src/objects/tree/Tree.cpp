//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

Tree::Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species) :
        species(species), branchTexture("wood"), leafTexture("leaf"), snowTexture("snow") {

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
}

void Tree::draw(const vcl::camera_scene &camera) {
    vcl::draw(leavesDrawable, camera);
    branchTexture.bind();
    vcl::draw(branchesDrawable, camera);
    leafTexture.bind();
    vcl::draw(leavesDrawable, camera);
    snowTexture.bind();
    vcl::draw(snowyLeavesDrawable, camera);
}
