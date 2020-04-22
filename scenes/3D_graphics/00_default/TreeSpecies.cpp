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
    pine.leafBlosNum = 50;
    pine.leavesDist = 0.0f;
    pine.leavesRotateAngle = 2.44f;
    pine.leafHeight = 0.09f;
    pine.leafBase = 0.03f;

    pine.nBranches = {1, 70};
    pine.nBranchesVar = {0, 7};
    pine.nakedRatio = {0.05f, 0.02f};
    pine.length = {1.0f, 0.35f};
    pine.lengthVar = {0.0f, 0.05f};
    pine.downAngle = {-0.0f, 0.52f, 0.52f};
    pine.downAngleVar = {-0.0f, -1.05f, 0.175f};
    pine.rotateAngle = {0.0f, 2.44f};
    pine.rotateAngleVar = {0.0f, 0.52f};
    pine.curve = {0.0f, -0.35f};
    pine.curveVar = {0.175f, 1.57f};
    pine.curveResolution = {4, 4};
    pine.bendVar = {0.0f, 1.22f};
    pine.segSplits = {0, 2};
    pine.splitAngle = {0.0f, 1.4f};
    pine.splitAngleVar = {0.0f, 0.52f};
    pine.branchDist = {0.0f, 3.5f, 0.0f};

    pine.leafColor = {0.55f, 0.71f, 0.15f, 1.0f};
    pine.leafColorVar = {0.08f, 0.1f, 0.02f, 0.2f};

    return pine;
}