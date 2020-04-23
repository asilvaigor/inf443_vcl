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

class Tree : public Object {
public:
    Tree(Shaders &shaders, vcl::vec3 &position, TreeSpecies &species);

    void draw(const vcl::camera_scene &camera) override;

private:
    TreeSpecies &species;
    vcl::mesh_drawable branchesDrawable;
    vcl::mesh_drawable leavesDrawable;
    vcl::mesh_drawable snowyLeavesDrawable;
    const Texture branchTexture;
    const Texture leafTexture;
    const Texture snowTexture;
};


#endif //PGM_TREE_H
