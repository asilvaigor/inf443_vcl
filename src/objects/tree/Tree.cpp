//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

Tree::Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species, float snowCoverage, bool verbose) :
        Object(false), species(species), branchTexture("wood"), leafTexture("leaf"), snowTexture("snow") {
    TurtleGraphics turtle(position);
    boundingBox = BoundingBox(position.x, position.x, position.y, position.y, position.z, position.z);
    Branch trunk(species, turtle, boundingBox, snowCoverage);

    auto branchesMesh = trunk.toBranchMesh();
    branchesDrawable = vcl::mesh_drawable(branchesMesh);
    branchesDrawable.shader = shaders["mesh"];
    branchesDrawable.uniform.shading = species.branchShading;
    branchesDrawable.uniform.color = {species.branchColor.x, species.branchColor.y, species.branchColor.z};
    branchesDrawable.uniform.color_alpha = species.branchColor.w;

    auto snowyBranchesMesh = trunk.toSnowyBranchMesh();
    if (!snowyBranchesMesh.empty()) {
        snowyBranchesDrawable = vcl::mesh_drawable(snowyBranchesMesh);
        snowyBranchesDrawable.shader = shaders["mesh"];
        snowyBranchesDrawable.uniform.shading = species.snowyBranchShading;
        snowyBranchesDrawable.uniform.color = {species.snowyBranchColor.x, species.snowyBranchColor.y,
                                               species.snowyBranchColor.z};
        snowyBranchesDrawable.uniform.color_alpha = species.snowyBranchColor.w;
        hasSnowyBranches = true;
    } else hasSnowyBranches = false;

    auto leavesMesh = trunk.toLeavesMesh();
    if (!leavesMesh.empty()) {
        leavesDrawable = vcl::mesh_drawable(leavesMesh);
        leavesDrawable.shader = shaders["mesh"];
        leavesDrawable.uniform.shading = species.leafShading;
        leavesDrawable.uniform.color = {species.leafColor.x, species.leafColor.y, species.leafColor.z};
        leavesDrawable.uniform.color_alpha = species.leafColor.w;
        hasLeaves = true;
    } else hasLeaves = false;

    auto snowyLeavesMesh = trunk.toSnowyLeavesMesh();
    if (!snowyLeavesMesh.empty()) {
        snowyLeavesDrawable = vcl::mesh_drawable(snowyLeavesMesh);
        snowyLeavesDrawable.shader = shaders["mesh"];
        snowyLeavesDrawable.uniform.shading = species.snowyLeafShading;
        snowyLeavesDrawable.uniform.color = {species.snowyLeafColor.x, species.snowyLeafColor.y,
                                             species.snowyLeafColor.z};
        snowyLeavesDrawable.uniform.color_alpha = species.snowyLeafColor.w;
        hasSnowyLeaves = true;
    } else hasSnowyLeaves = false;

    if (verbose) {
        std::cout << "Number of vertices: branches " << branchesMesh.position.size();
        if ((int) leavesMesh.position.size() > 0)
            std::cout << " leaves " << leavesMesh.position.size();
        if ((int) snowyLeavesMesh.position.size() > 0)
            std::cout << " snowyLeaves " << snowyLeavesMesh.position.size();
        std::cout << std::endl;
    }
}

void Tree::draw(vcl::camera_scene &camera) {
    if (boundingBox.isInCameraFrustum(camera)) {
        branchesDrawable.uniform.light = light;
        branchTexture.bind();
        vcl::draw(branchesDrawable, camera);
        if (hasSnowyBranches) {
            snowyBranchesDrawable.uniform.light = light;
            snowTexture.bind();
            vcl::draw(snowyBranchesDrawable, camera);
        }
        if (hasLeaves) {
            leavesDrawable.uniform.light = light;
            leafTexture.bind();
            vcl::draw(leavesDrawable, camera);
        }
        if (hasSnowyLeaves) {
            snowyLeavesDrawable.uniform.light = light;
            snowTexture.bind();
            vcl::draw(snowyLeavesDrawable, camera);
        }
    }
}
