//
// Created by igor on 15/04/20.
//

#ifndef PGM_TREESPECIES_H
#define PGM_TREESPECIES_H

#include <vector>
#include "vcl.hpp"

/**
 * Stores parameters related to tree generation.
 * Check out the end of the article on https://github.com/friggog/tree-gen for a more thorough description of each
 * parameter.
 */
class TreeSpecies {
public:
    TreeSpecies();

    static TreeSpecies pine();

    static TreeSpecies blackTupelo();

    static TreeSpecies bush1();

    static TreeSpecies bush2();

    int levels{}; // >=1, Number of levels in the branches recursion.
    // 1 is only trunk, 2 is trunk with branches on it, 3 these branches will have branches etc.
    // Determines the size of all the std::vector variables below.
    float scale{}; // >=0, Average scale of the tree.
    float scaleVar{}; // >=0, Variance in the height of the tree.
    float flare{}; // >=0, How much the radius of the trunk decreases.
    float ratio{}; // >=0, Ratio radius / height of branches.
    float ratioPow{}; // >=0, How drastically the branch radius is reduced between levels.
    float stemSpacing{}; // >=0, The maximum spacing of a stem from the center.
    int leafBlosNum{}; // >=0, Number of leaves on each of the deepest level of branches.
    float leavesRotateAngle{}; // >=0 Rad, Angle leaves will be rotated between each other.
    float leafHeight{}; // >=0, Height of the leaf triangle.
    float leafHeightVar{}; // >=0, Leaf height variance.
    float leafBase{}; // >=0, Base of the leaf triangle.
    float leafBaseVar{}; // >=0, Leaf base variance.

    vcl::shading_mesh branchShading; // Shading for the branch mesh.
    vcl::shading_mesh snowyBranchShading; // Shading for the snowy branch mesh.
    vcl::shading_mesh leafShading; // Shading for the leaf mesh.
    vcl::shading_mesh snowyLeafShading; // Shading for the snowy leaf mesh.
    vcl::vec4 branchColor; // Color for the branch mesh.
    vcl::vec4 snowyBranchColor; // Color for the snowy branch mesh.
    vcl::vec4 leafColor; // Color for the leaf mesh.
    vcl::vec4 snowyLeafColor; // Color for the snowy leaf mesh.

    // These variables are setted for each level in the branch recursion.
    // Their sizes are equal to the "levels" variable above.
    std::vector<int> nBranches; // >=0, Average number of branches.
    std::vector<int> nBranchesVar; // >=0, Variance in the number of branches.
    std::vector<int> bezierResolution; // >=2, In How many points the bezier spline will be defined.
    std::vector<int> segSplits; // >=0, How many splits a branch will split into.
    std::vector<float> nakedRatio; // >=0, Proportion of the branch in which no child branches/leaves are spawned.
    std::vector<float> length; // >=0, Average length of the branch.
    std::vector<float> lengthVar; // >=0, Length variance.
    std::vector<float> downAngle; // Rad, Angle of direction of a child branch/leaf.
    std::vector<float> downAngleVar; // Rad, Variance of down angle. If <0, this variance is distributed along the
    // branch.
    std::vector<float> rotateAngle; // Rad, Angle for rotation along the branch axis.
    std::vector<float> rotateAngleVar; // Rad, Rotate angle variance.
    std::vector<float> curve; // Rad, Angle the branch will end in relation to its start.
    std::vector<float> curveVar; // Rad, Curve variance.
    std::vector<float> bendVar; // Rad, Similar to curve, its an angle in the other axis.
    std::vector<float> splitAngle; // Rad, Angle of the clone splitting.
    std::vector<float> splitAngleVar; // Rad Split angle variance.
    std::vector<float> branchDist; // >=1, Separation distance between whorls.
};


#endif //PGM_TREESPECIES_H
