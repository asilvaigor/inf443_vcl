//
// Created by igor on 14/04/20.
//

#ifndef PGM_TREE_H
#define PGM_TREE_H

#include <vcl/vcl.hpp>
#include "Branch.h"

class Tree {
public:
    void setupData(std::map<std::string, GLuint> &shaders, TreeSpecies &species);

    void draw(const vcl::camera_scene &camera);

private:
    vcl::mesh_drawable drawable;
    GLuint textureId;
};


#endif //PGM_TREE_H
