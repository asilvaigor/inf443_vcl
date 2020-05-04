//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

Tree::Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species, float snowCoverage, bool verbose) :
        Object(false), species(species), branchTexture("wood"), leafTexture("leaf"), snowTexture("snow") {
    boundingBox = BoundingBox(position.x, position.x, position.y, position.y, position.z, position.z);
    createMeshes(position, snowCoverage);
    createDrawables(shaders);

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
        if (!branchesMesh.empty()) {
            branchesDrawable.uniform.light = light;
            branchTexture.bind();
            vcl::draw(branchesDrawable, camera);
        }
        if (!snowyBranchesMesh.empty()) {
            snowyBranchesDrawable.uniform.light = light;
            snowTexture.bind();
            vcl::draw(snowyBranchesDrawable, camera);
        }
        if (!leavesMesh.empty()) {
            leavesDrawable.uniform.light = light;
            leafTexture.bind();
            vcl::draw(leavesDrawable, camera);
        }
        if (!snowyLeavesMesh.empty()) {
            snowyLeavesDrawable.uniform.light = light;
            snowTexture.bind();
            vcl::draw(snowyLeavesDrawable, camera);
        }
    }
}

void Tree::createMeshes(vcl::vec3 &position, float &snowCoverage) {
    for (int i = 0; i < species.nBranches[0]; i++) {
        vcl::vec3 p = position;
        if (species.nBranches[0] > 1) {
            // Placing stems uniformly in a circle
            float r = vcl::rand_interval(0.0f, species.stemSpacing);
            float ang = vcl::rand_interval(0.0f, 2 * M_PI);
            p += vcl::vec3(r * std::cos(ang), r * std::sin(ang), 0);
        }
        TurtleGraphics turtle(p);
        Branch trunk(species, turtle, boundingBox, snowCoverage);

        auto m = trunk.toBranchMesh();
        branchesMesh.add(m);

        m = trunk.toSnowyBranchMesh();
        snowyBranchesMesh.add(m);

        m = trunk.toLeavesMesh();
        leavesMesh.add(m);

        m = trunk.toSnowyLeavesMesh();
        snowyLeavesMesh.add(m);
    }
}

void Tree::createDrawables(Shaders &shaders) {
    branchesDrawable = vcl::mesh_drawable(branchesMesh);
    branchesDrawable.shader = shaders["mesh"];
    branchesDrawable.uniform.shading = species.branchShading;
    branchesDrawable.uniform.color = {species.branchColor.x, species.branchColor.y, species.branchColor.z};
    branchesDrawable.uniform.color_alpha = species.branchColor.w;

    if (!snowyBranchesMesh.empty()) {
        snowyBranchesDrawable = vcl::mesh_drawable(snowyBranchesMesh);
        snowyBranchesDrawable.shader = shaders["mesh"];
        snowyBranchesDrawable.uniform.shading = species.snowyBranchShading;
        snowyBranchesDrawable.uniform.color = {species.snowyBranchColor.x, species.snowyBranchColor.y,
                                               species.snowyBranchColor.z};
        snowyBranchesDrawable.uniform.color_alpha = species.snowyBranchColor.w;
    }

    if (!leavesMesh.empty()) {
        leavesDrawable = vcl::mesh_drawable(leavesMesh);
        leavesDrawable.shader = shaders["mesh"];
        leavesDrawable.uniform.shading = species.leafShading;
        leavesDrawable.uniform.color = {species.leafColor.x, species.leafColor.y, species.leafColor.z};
        leavesDrawable.uniform.color_alpha = species.leafColor.w;
    }

    if (!snowyLeavesMesh.empty()) {
        snowyLeavesDrawable = vcl::mesh_drawable(snowyLeavesMesh);
        snowyLeavesDrawable.shader = shaders["mesh"];
        snowyLeavesDrawable.uniform.shading = species.snowyLeafShading;
        snowyLeavesDrawable.uniform.color = {species.snowyLeafColor.x, species.snowyLeafColor.y,
                                             species.snowyLeafColor.z};
        snowyLeavesDrawable.uniform.color_alpha = species.snowyLeafColor.w;
    }
}
