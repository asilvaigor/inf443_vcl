//
// Created by igor on 02/06/2020.
//

#ifndef PGM_TRANSLATEDTREE_H
#define PGM_TRANSLATEDTREE_H

#include "objects/tree/Tree.h"

/**
 * Stores a pointer to a tree, and will draw this tree translated.
 */
class TranslatedTree : public Object {
public:
    /**
     * Transforms the bounding sphere and billboard.
     * @param tree
     * @param translation
     */
    TranslatedTree(std::shared_ptr<Tree> &tree, vcl::vec3 &translation);

    /**
     * Updates the shader uniforms and draws the tree.
     * @param camera
     * @param time
     */
    void drawMesh(vcl::camera_scene &camera) override;

    const float &getBranchRadius();

    const float &getTrunkRadius();

private:
    std::shared_ptr<Tree> tree;
    vcl::vec3 translation;
};


#endif //PGM_TRANSLATEDTREE_H
