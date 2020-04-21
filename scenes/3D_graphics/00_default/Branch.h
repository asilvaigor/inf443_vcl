//
// Created by igor on 15/04/20.
//

#ifndef PGM_STEM_H
#define PGM_STEM_H

#include <memory>
#include "TreeSpecies.h"
#include "TurtleGraphics.h"
#include "BezierSpline.h"


class Branch {
public:
    explicit Branch(TreeSpecies& species, int depth = 0, int startIdx = 0, TurtleGraphics turtle = TurtleGraphics(),
            Branch* parent = nullptr, float nBranchesFactor = 1, float splitAngleCorrection = 0, float cloneProb = 1,
            float offsetInParent = 0, float radiusLimit = FLT_MAX);

    vcl::mesh toMesh();

private:
    TreeSpecies &species;
    TurtleGraphics turtle;
    BezierSpline spline;
    int depth;
    int startIdx;
    std::vector<Branch> children;
    Branch* parent;
    float length;
    float radius;
    float nBranchesFactor;
    float splitAngleCorrection;
    float cloneProb;
    float offsetInParent;
    float maxLengthChild;
    float radiusLimit;

    static vcl::rand_generator rand;

    void generate();

    void makeBranches(int segIdx, float branchesOnSeg, float prevRotationAngle);

    void makeBranch(int branchIdx, int branchMode, float offset, float stemOffset, float prevRotationAngle,
            int nBranchesInGroup = 0);

    void makeClones(int segIdx, float nSplits, float curveAngle);

    float calculateLength();

    float calculateRadius();

    float calculateRadiusAtOffset(float offset) const;

    float calculateCurveAngle();

    float calculateDownAngle(float stemOffset);

    float calculateRotateAngle(float prevAngle);

    int calculateNBranches();

};


#endif //PGM_STEM_H
