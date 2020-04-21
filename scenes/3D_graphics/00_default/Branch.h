//
// Created by igor on 15/04/20.
//

#ifndef PGM_STEM_H
#define PGM_STEM_H

#include <memory>
#include "TreeSpecies.h"
#include "TurtleGraphics.h"
#include "BezierSpline.h"
#include "Leaf.h"


class Branch {
public:
    explicit Branch(TreeSpecies& species, TurtleGraphics turtle = TurtleGraphics(), int depth = 0, int startIdx = 0,
                    Branch* parent = nullptr, float treeScale = 0, float nBranchesFactor = 1,
                    float splitAngleCorrection = 0, float cloneProb = 1, float offsetInTrunk = 0,
                    float radiusLimit = FLT_MAX);

    vcl::mesh toMesh();

    vcl::mesh toLeavesMesh();

private:
    TreeSpecies &species;
    TurtleGraphics turtle;
    BezierSpline spline;
    int depth;
    int startIdx;
    std::vector<Branch> branches;
    std::vector<Leaf> leaves;
    Branch* parent;
    float length;
    float radius;
    float treeScale;
    float nBranchesFactor;
    float splitAngleCorrection;
    float cloneProb;
    float offsetInTrunk;
    float maxLengthChild;
    float radiusLimit;

    static vcl::rand_generator rand;

    void generate();

    void makeBranches(int segIdx, float branchesOnSeg, float prevRotationAngle);

    void makeBranch(int branchIdx, float offset, float offsetInParent, float prevRotationAngle, int nBranchesInGroup = 0);

    void makeLeaves(int segIdx, float leavesOnSeg, float prevRotationAngle);

    void makeLeaf(float offset, float offsetInParent, float prevRotationAngle);

    void makeClones(int segIdx, float nSplits, float curveAngle);

    float calculateLength();

    float calculateRadius();

    float calculateRadiusAtOffset(float offset) const;

    float calculateCurveAngle();

    float calculateDownAngle(float stemOffset);

    float calculateRotateAngle(float prevAngle);

    int calculateNBranches() const;

    int calculateNLeaves() const;
};


#endif //PGM_STEM_H
