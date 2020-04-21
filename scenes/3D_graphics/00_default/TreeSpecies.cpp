//
// Created by igor on 15/04/20.
//

#include "TreeSpecies.h"

TreeSpecies::TreeSpecies() = default;

TreeSpecies TreeSpecies::pine() {
    TreeSpecies pine;
    pine.levels = 2;
    pine.scale = 6.0f;
    pine.scaleVar = 0.5f;
    pine.flare = 0.3f;
    pine.ratio = 0.02f;
    pine.ratioPow = 1.3f;
    pine.nakedSplits = 0;

    pine.nBranches = {1, 70};
    pine.nBranchesVar = {0, 7};
    pine.nakedRatio = {0.05f, 0.02f};
    pine.length = {1.0f, 0.35f};
    pine.lengthVar = {0.0f, 0.05f};
    pine.downAngle = {-0.0f, 0.52f, 0.52f};
    pine.downAngleVar = {-0.0f, -1.05f, 0.175f};
    pine.rotateAngle = {0.0f, 2.44f, 2.44f};
    pine.rotateAngleVar = {0.0f, 0.52f, 0.35f};
    pine.curve = {0.0f, -0.35f};
    pine.curveVar = {0.175f, 1.57f};
    pine.curveResolution = {4, 4};
    pine.bendVar = {0.0f, 1.22f};
    pine.segSplits = {0, 2};
    pine.splitAngle = {0.0f, 1.4f};
    pine.splitAngleVar = {0.0f, 0.52f};
    pine.branchDist = {0.0f, 3.5f};
    return pine;
}