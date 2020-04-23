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
    pine.leafBlosNum = 50;
    pine.leavesRotateAngle = 2.44f;
    pine.leafHeight = 0.09f;
    pine.leafHeightVar= 0.009f;
    pine.leafBase = 0.03f;
    pine.leafBaseVar = 0.003f;
    pine.snowyLeafMaxAngle = (float) (0.4 * M_PI);

    pine.branchShading = {0.3f, 0.6f, 0.0f, 128};
    pine.leafShading = {0.3f, 0.8f, 0.0f, 128};
    pine.snowyLeafShading = {0.4f, 0.6f, 0.0f, 128};
    pine.branchColor = {1.0f, 1.0f, 1.0f, 1.0f};
    pine.leafColor = {0.04f, 0.4f, 0.14f, 1.0f};
    pine.snowyLeafColor = {1.0f, 1.0f, 1.0f, 1.0f};

    pine.nBranches = {1, 70};
    pine.nBranchesVar = {0, 7};
    pine.bezierResolution = {4, 4};
    pine.segSplits = {0, 2};
    pine.nakedRatio = {0.05f, 0.02f};
    pine.length = {1.0f, 0.35f};
    pine.lengthVar = {0.0f, 0.05f};
    pine.downAngle = {0.52f, 0.52f};
    pine.downAngleVar = {-1.05f, 0.175f};
    pine.rotateAngle = {0.0f, 2.44f};
    pine.rotateAngleVar = {0.0f, 0.52f};
    pine.curve = {0.0f, 0.35f};
    pine.curveVar = {0.175f, 1.57f};
    pine.bendVar = {0.0f, 1.22f};
    pine.splitAngle = {0.0f, 1.4f};
    pine.splitAngleVar = {0.0f, 0.52f};
    pine.branchDist = {3.5f, 0.0f};

    return pine;
}