//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

Tree::Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species) :
        Object(false), species(species), branchTexture("wood"), leafTexture("leaf"), snowTexture("snow") {
    TurtleGraphics turtle(position);
    Branch trunk(species, turtle);

    auto branchesMesh = trunk.toBranchMesh();
    branchesDrawable = vcl::mesh_drawable(branchesMesh);
    branchesDrawable.shader = shaders["mesh"];
    branchesDrawable.uniform.shading = species.branchShading;
    branchesDrawable.uniform.color = {species.branchColor.x, species.branchColor.y, species.branchColor.z};
    branchesDrawable.uniform.color_alpha = species.branchColor.w;

    auto leavesMesh = trunk.toLeavesMesh();
    if (!leavesMesh.position.empty()) {
        leavesDrawable = vcl::mesh_drawable(leavesMesh);
        leavesDrawable.shader = shaders["mesh"];
        leavesDrawable.uniform.shading = species.leafShading;
        leavesDrawable.uniform.color = {species.leafColor.x, species.leafColor.y, species.leafColor.z};
        leavesDrawable.uniform.color_alpha = species.leafColor.w;
        hasLeaves = true;
    } else hasLeaves = false;

    auto snowyLeavesMesh = trunk.toSnowyLeavesMesh();
    if (!snowyLeavesMesh.position.empty()) {
        snowyLeavesDrawable = vcl::mesh_drawable(snowyLeavesMesh);
        snowyLeavesDrawable.shader = shaders["mesh"];
        snowyLeavesDrawable.uniform.shading = species.snowyLeafShading;
        snowyLeavesDrawable.uniform.color = {species.snowyLeafColor.x, species.snowyLeafColor.y,
                                             species.snowyLeafColor.z};
        snowyLeavesDrawable.uniform.color_alpha = species.snowyLeafColor.w;
        hasSnow = true;
    } else hasSnow = false;

//    std::cout << "Number of vertices: branches " << branchesMesh.position.size() << " leaves " <<
//              leavesMesh.position.size() << " snowyLeaves " << snowyLeavesMesh.position.size() << std::endl;
}

void Tree::draw(const vcl::camera_scene &camera) {
    branchesDrawable.uniform.light = *light;
    branchTexture.bind();
    vcl::draw(branchesDrawable, camera);
    if (hasLeaves) {
        leavesDrawable.uniform.light = *light;
        leafTexture.bind();
        vcl::draw(leavesDrawable, camera);
    }
    if (hasSnow) {
        snowyLeavesDrawable.uniform.light = *light;
        snowTexture.bind();
        vcl::draw(snowyLeavesDrawable, camera);
    }
}
