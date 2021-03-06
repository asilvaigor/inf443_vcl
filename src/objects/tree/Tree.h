//
// Created by igor on 14/04/20.
//

#ifndef PGM_TREE_H
#define PGM_TREE_H

#include "vcl.hpp"
#include "shaders/Shaders.h"
#include "objects/Object.h"
#include "Branch.h"
#include "utils/Texture.h"

/**
 * Procedurally generated tree object.
 * Code is a simplified version of the blender addon https://github.com/friggog/tree-gen, made by Charlie Hewitt.
 * The way it is currently tuned, the whole tree will have from 15k to 20k vertices.
 */
class Tree : public Object {
public:
    /**
     * Generates a random tree at the given position.
     * @param shaders Shaders instance.
     * @param position Position of the base of the tree.
     * @param species TreeSpecies.
     * @param snowCoverage Controls how much snow will be in the tree.
     * @param verbose It will data while generating tree.
     */
    Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species, float snowCoverage = 0.4,
            bool verbose = false);

    /**
     * Draws the tree on opengl.
     * @param camera
     * @param time
     */
    void drawMesh(vcl::camera_scene &camera) override;

    float &getTrunkRadius();

    float &getBranchRadius();

    bool hasBranches();

    bool hasSnowyBranches();

    bool hasLeaves();

    bool hasSnowyLeaves();

    vcl::mesh_drawable branchesDrawable;
    vcl::mesh_drawable snowyBranchesDrawable;
    vcl::mesh_drawable leavesDrawable;
    vcl::mesh_drawable snowyLeavesDrawable;
    static std::shared_ptr<Texture> branchTexture;
    static std::shared_ptr<Texture> leafTexture;
    static std::shared_ptr<Texture> snowTexture;

private:
    TreeSpecies &species;
    vcl::mesh branchesMesh;
    vcl::mesh snowyBranchesMesh;
    vcl::mesh leavesMesh;
    vcl::mesh snowyLeavesMesh;
    bool verbose;
    float trunkRadius;
    float branchRadius;

    void createMeshes(float &snowCoverage);

    void createDrawables(Shaders &shaders);

    float normalizeSnowCoverage(float &snowCoverage);
};


#endif //PGM_TREE_H
