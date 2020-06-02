//
// Created by igor on 02/06/2020.
//

#include "TranslatedTree.h"

TranslatedTree::TranslatedTree(std::shared_ptr<Tree> &tree, vcl::vec3 &translation) :
        Object(false), tree(tree), translation(translation) {
    position = tree->getPosition() + translation;
    boundingSphere = BoundingSphere(tree->getBoundingSphere().getCenter() + translation,
                                    tree->getBoundingSphere().getRadius());
    boundingBox = BoundingBox(tree->getBoundingBox().minX + translation.x,
                              tree->getBoundingBox().maxX + translation.x,
                              tree->getBoundingBox().minY + translation.y,
                              tree->getBoundingBox().maxY + translation.y,
                              tree->getBoundingBox().minZ + translation.z,
                              tree->getBoundingBox().maxZ + translation.z);
    billboard = BillboardGenerator(&tree->getBillboard(), translation);
}

void TranslatedTree::drawMesh(vcl::camera_scene &camera) {
    tree->branchesDrawable.uniform.transform.translation = translation;
    tree->snowyBranchesDrawable.uniform.transform.translation = translation;
    tree->leavesDrawable.uniform.transform.translation = translation;
    tree->snowyLeavesDrawable.uniform.transform.translation = translation;
    tree->drawMesh(camera);
}

const float &TranslatedTree::getBranchRadius() {
    return tree->getBranchRadius();
}

const float &TranslatedTree::getTrunkRadius() {
    return tree->getTrunkRadius();
}
