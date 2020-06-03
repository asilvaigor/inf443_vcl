//
// Created by igor on 14/04/20.
//

#include "Tree.h"
#include "TreeSpecies.h"

std::shared_ptr<Texture> Tree::branchTexture = nullptr;
std::shared_ptr<Texture> Tree::leafTexture = nullptr;
std::shared_ptr<Texture> Tree::snowTexture = nullptr;

Tree::Tree(Shaders &shaders, vcl::vec3 &pos, TreeSpecies &species, float snowCoverage, bool verbose) :
        Object(false, species.shadowless), species(species) {
    if (Tree::branchTexture == nullptr)
        branchTexture = std::make_shared<Texture>("wood");
    if (Tree::leafTexture == nullptr)
        leafTexture = std::make_shared<Texture>("leaf");
    if (Tree::snowTexture == nullptr)
        snowTexture = std::make_shared<Texture>("snow");

    position = pos;
    this->verbose = verbose;
    snowCoverage = normalizeSnowCoverage(snowCoverage);
    createMeshes(snowCoverage);
    createDrawables(shaders);
    billboard = BillboardGenerator(shaders, this);

    branchRadius = 0.5 * std::max(boundingBox.maxX - boundingBox.minX, boundingBox.maxY - boundingBox.minY);
    if (species.nBranches[0] == 1)
        trunkRadius = (species.length[0] + species.lengthVar[0]) * species.ratio;
    else trunkRadius = branchRadius;
}

void Tree::drawMesh(vcl::camera_scene &camera) {
    if (!branchesMesh.empty()) {
        branchesDrawable.uniform.lights = lights;
        branchTexture->bind();
        vcl::draw(branchesDrawable, camera);
    }
    if (!leavesMesh.empty()) {
        leavesDrawable.uniform.lights = lights;
        leafTexture->bind();
        vcl::draw(leavesDrawable, camera);
    }

    snowTexture->bind();
    if (!snowyLeavesMesh.empty()) {
        snowyLeavesDrawable.uniform.lights = lights;
        vcl::draw(snowyLeavesDrawable, camera);
    }
    if (!snowyBranchesMesh.empty()) {
        snowyBranchesDrawable.uniform.lights = lights;
        vcl::draw(snowyBranchesDrawable, camera);
    }
}

float &Tree::getTrunkRadius() {
    return trunkRadius;
}

float &Tree::getBranchRadius() {
    return branchRadius;
}

bool Tree::hasBranches() {
    return !branchesMesh.empty();
}

bool Tree::hasSnowyBranches() {
    return !snowyBranchesMesh.empty();
}

bool Tree::hasLeaves() {
    return !leavesMesh.empty();
}

bool Tree::hasSnowyLeaves() {
    return !snowyLeavesMesh.empty();
}

void Tree::createMeshes(float &snowCoverage) {
    std::vector<vcl::vec3 *> vertices;
    int nBranches = species.nBranches[0] + (int) (Branch::rand.rand() * species.nBranchesVar[0]);
    for (int i = 0; i < nBranches; i++) {
        vcl::vec3 p = position;
        if (species.nBranches[0] > 1) {
            // Placing stems uniformly in a circle
            float r = Branch::rand.rand(0.0f, species.stemSpacing);
            float ang = Branch::rand.rand(0.0f, 2 * M_PI);
            p += vcl::vec3(r * std::cos(ang), r * std::sin(ang), 0);
        }
        TurtleGraphics turtle(p);
        Branch trunk(species, turtle, snowCoverage);

        auto m = trunk.toBranchMesh();
        branchesMesh.add(m);

        m = trunk.toSnowyBranchMesh();
        snowyBranchesMesh.add(m);

        m = trunk.toLeavesMesh();
        leavesMesh.add(m);

        m = trunk.toSnowyLeavesMesh();
        snowyLeavesMesh.add(m);
    }

    // Creating bounding sphere and box
    int nB = (int) branchesMesh.position.size();
    int nSB = (int) snowyBranchesMesh.position.size();
    int nL = (int) leavesMesh.position.size();
    int nSL = (int) snowyLeavesMesh.position.size();
    int nVertices = nB + nSB + nL + nSL;
    vertices.reserve(nVertices);

    for (auto &pt : branchesMesh.position) {
        vertices.push_back(&pt);
        boundingBox.update(pt);
    }
    for (auto &pt : snowyBranchesMesh.position) {
        vertices.push_back(&pt);
        boundingBox.update(pt);
    }
    for (auto &pt : leavesMesh.position) {
        vertices.push_back(&pt);
        boundingBox.update(pt);
    }
    for (auto &pt : snowyLeavesMesh.position) {
        vertices.push_back(&pt);
        boundingBox.update(pt);
    }
    boundingSphere = BoundingSphere(vertices);

    if (verbose) {
        std::cout << "Number of vertices: total " << nVertices;
        if (nB > 0) std::cout << " branches " << nB;
        if (nSB > 0) std::cout << " snowyBranches " << nSB;
        if (nL > 0) std::cout << " leaves " << nL;
        if (nSL > 0) std::cout << " snowyLeaves " << nSL;
        std::cout << std::endl;
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

float Tree::normalizeSnowCoverage(float &snowCoverage) {
    if (snowCoverage > 0.99f)
        return 1.0f;
    return snowCoverage * 0.6f;
}
