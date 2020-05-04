//
// Created by igor on 15/04/20.
//

#include "TreeSpecies.h"

TreeSpecies::TreeSpecies() = default;

TreeSpecies TreeSpecies::pine() {
    TreeSpecies s;
    s.levels = 2;
    s.scale = 6.0f;
    s.scaleVar = 0.5f;
    s.flare = 0.3f;
    s.ratio = 0.02f;
    s.ratioPow = 1.3f;
    s.leafBlosNum = 50;
    s.leavesRotateAngle = 2.44f;
    s.leafHeight = 0.09f;
    s.leafHeightVar= 0.009f;
    s.leafBase = 0.03f;
    s.leafBaseVar = 0.003f;

    s.branchShading = {0.3f, 0.6f, 0.0f, 128};
    s.leafShading = {0.3f, 0.8f, 0.0f, 128};
    s.snowyLeafShading = {0.4f, 0.6f, 0.0f, 128};
    s.snowyBranchShading = {0.3f, 0.6f, 0.0f, 128};
    s.branchColor = {1.0f, 1.0f, 1.0f, 1.0f};
    s.leafColor = {0.04f, 0.4f, 0.14f, 1.0f};
    s.snowyLeafColor = {1.0f, 1.0f, 1.0f, 1.0f};
    s.snowyBranchColor = {1.0f, 1.0f, 1.0f, 1.0f};

    s.nBranches = {1, 70};
    s.nBranchesVar = {0, 7};
    s.bezierResolution = {4, 4};
    s.segSplits = {0, 2};
    s.nakedRatio = {0.05f, 0.02f};
    s.length = {1.0f, 0.35f};
    s.lengthVar = {0.0f, 0.05f};
    s.downAngle = {0.52f, 0.52f};
    s.downAngleVar = {-1.05f, 0.175f};
    s.rotateAngle = {0.0f, 2.44f};
    s.rotateAngleVar = {0.0f, 0.52f};
    s.curve = {0.0f, 0.35f};
    s.curveVar = {0.175f, 1.57f};
    s.bendVar = {0.0f, 1.22f};
    s.splitAngle = {0.0f, 1.4f};
    s.splitAngleVar = {0.0f, 0.52f};
    s.branchDist = {3.5f, 0.0f};

    return s;
}

TreeSpecies TreeSpecies::blackTupelo() {
    TreeSpecies s;
    s.levels = 3; // Lowering to 3 to improve performance.
    s.scale = 6.0f;
    s.scaleVar = 1.0f;
    s.flare = 1.0f;
    s.ratio = 0.015f;
    s.ratioPow = 1.3f;
    s.leafBlosNum = 0;

    s.branchShading = {0.3f, 0.6f, 0.0f, 128};
    s.branchColor = {1.0f, 1.0f, 1.0f, 1.0f};
    s.snowyBranchShading = {0.3f, 0.6f, 0.0f, 128};
    s.snowyBranchColor = {1.0f, 1.0f, 1.0f, 1.0f};

    s.nBranches = {1, 50, 15, 10};
    s.nBranchesVar = {0, 7, 2, 1};
    s.bezierResolution = {4, 3, 1, 1};
    s.segSplits = {0, 0, 0, 0};
    s.nakedRatio = {0.2f, 0.02f, 0.02f, 0.02f};
    s.length = {1.0f, 0.35, 0.6f, 0.4f};
    s.lengthVar = {0.0f, 0.05f, 0.1f, 0.0f};
    s.downAngle = {1.05f, 0.7f, 0.79f, 0.79f};
    s.downAngleVar = {-0.79f, 0.175f, 0.175f, 0.175f};
    s.rotateAngle = {0.0f, 2.44f, 2.44f, 2.44f};
    s.rotateAngleVar = {0.0f, 1.05f, 0.87f, 0.0f};
    s.curve = {0.0f, 0.0f, 0.175f, 0.0f};
    s.curveVar = {0.4f, 1.57f, 2.62f, 0.0f};
    s.bendVar = {0.0f, 1.74f, 0.0f, 0.0f};
    s.splitAngle = {0.0f, 0.0f, 0.0f, 0.0f};
    s.splitAngleVar = {0.0f, 0.0f, 0.0f, 0.0f};
    s.branchDist = {0.0f, 0.0f, 0.0f, 0.0f};

    return s;
}

TreeSpecies TreeSpecies::bush() {
    TreeSpecies s;
    s.levels = 1;
    s.scale = 1.0f;
    s.scaleVar = 0.3f;
    s.flare = 0.0f;
    s.ratio = 0.015f;
    s.ratioPow = 1.3f;
    s.stemSpacing = 0.3f;
    s.leafBlosNum = 0;

    s.branchShading = {0.3f, 0.6f, 0.0f, 128};
    s.branchColor = {1.0f, 1.0f, 1.0f, 1.0f};
    s.snowyBranchShading = {0.3f, 0.6f, 0.0f, 128};
    s.snowyBranchColor = {1.0f, 1.0f, 1.0f, 1.0f};

    s.nBranches = {20};
    s.nBranchesVar = {2};
    s.bezierResolution = {5};
    s.segSplits = {0};
    s.nakedRatio = {0.2f};
    s.length = {1.0f};
    s.lengthVar = {0.0f};
    s.downAngle = {1.05f};
    s.downAngleVar = {-0.79f};
    s.rotateAngle = {0.0f};
    s.rotateAngleVar = {0.0f};
    s.curve = {1.57f};
    s.curveVar = {3.14f};
    s.bendVar = {3.14f};
    s.splitAngle = {0.0f};
    s.splitAngleVar = {0.0f};
    s.branchDist = {0.0f};

    return s;
}