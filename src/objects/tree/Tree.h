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
     */
    Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species);

    /**
     * Draws the tree on opengl.
     * @param camera
     */
    void draw(const vcl::camera_scene &camera) override;

private:
    TreeSpecies &species;
    vcl::mesh_drawable branchesDrawable;
    vcl::mesh_drawable leavesDrawable;
    vcl::mesh_drawable snowyLeavesDrawable;
    bool hasLeaves;
    bool hasSnow;
    const Texture branchTexture;
    const Texture leafTexture;
    const Texture snowTexture;
};


#endif //PGM_TREE_H
