//
// Created by igor on 15/04/20.
//

#ifndef PGM_TREESPECIES_H
#define PGM_TREESPECIES_H

#include <vector>
#include "vcl/vcl.hpp"

class TreeSpecies {
public:
    TreeSpecies();

    static TreeSpecies pine();

    int levels;
    float scale;
    float scaleVar;
    float flare;
    float ratio;
    float ratioPow;
    float nakedSplits;

    std::vector<int> nBranches;
    std::vector<int> nBranchesVar;
    std::vector<float> nakedRatio;
    std::vector<float> length;
    std::vector<float> lengthVar;
    std::vector<float> downAngle; // Rad
    std::vector<float> downAngleVar; // Rad
    std::vector<float> rotateAngle; // Rad
    std::vector<float> rotateAngleVar; // Rad
    std::vector<float> curve; // Rad
    std::vector<float> curveVar; // Rad
    std::vector<int> curveResolution;
    std::vector<float> bendVar; // Rad
    std::vector<int> segSplits;
    std::vector<float> splitAngle; // Rad
    std::vector<float> splitAngleVar; // Rad
    std::vector<float> branchDist;
};


#endif //PGM_TREESPECIES_H
