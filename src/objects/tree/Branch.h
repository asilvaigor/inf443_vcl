//
// Created by igor on 15/04/20.
//

#ifndef PGM_STEM_H
#define PGM_STEM_H

#include <memory>
#include "TreeSpecies.h"
#include "utils/BoundingSphere.h"
#include "utils/TurtleGraphics.h"
#include "utils/BezierSpline.h"
#include "Leaf.h"


/**
 * Branch recursive class for Tree. It will create a mesh for the branch and its children, as well as leaves.
 * Code is a simplified version of the blender addon https://github.com/friggog/tree-gen, made by Charlie Hewitt.
 */
class Branch {
public:
    /**
     * Will generate the branch and all its children, by setting up variables and calling generate().
     * @param species TreeSpecies
     * @param turtle Turtle for the origin of the branch.
     * @param snowCoverage Controls how much snow there is in the tree.
     * @param depth Current depth in the tree.
     * @param startIdx The start segment of this branch's parent's bezier spline, used for splitting.
     * @param parent Pointer to the parent branch.
     * @param treeScale Tree scale, calculated in the call to the trunk constructor.
     * @param nBranchesFactor Factor controlling the number of branches in each level.
     * @param splitAngleCorrection Correction for the split angle.
     * @param splitProb Probability of splitting this branch.
     * @param offsetInTrunk Branch's Offset in the trunk's height.
     * @param radiusLimit Maximum value for the branch's radius.
     */
    explicit Branch(TreeSpecies &species, TurtleGraphics turtle, float &snowCoverage,
                    int depth = 0, int startIdx = 0, Branch *parent = nullptr, float treeScale = 0,
                    float nBranchesFactor = 1, float splitAngleCorrection = 0, float splitProb = 1,
                    float offsetInTrunk = 0, float radiusLimit = FLT_MAX);

    /**
     * Calculates the mesh for the branch parts.
     * @return
     */
    vcl::mesh toBranchMesh();

    /**
     * Calculates the mesh for the branches covered with snow.
     * @return
     */
    vcl::mesh toSnowyBranchMesh();

    /**
     * Calculates the mesh for the leaves.
     * @return
     */
    vcl::mesh toLeavesMesh();

    /**
     * Calculates the mesh for the snowy leaves.
     * @return
     */
    vcl::mesh toSnowyLeavesMesh();

private:
    TreeSpecies &species;
    TurtleGraphics turtle;
    float &snowCoverage;
    BezierSpline spline;
    int depth;
    int startIdx;
    std::vector<Branch> branches;
    std::vector<Leaf> leaves;
    std::vector<Leaf> snowyLeaves;
    Branch *parent;
    float length;
    float radius;
    float treeScale;
    float nBranchesFactor;
    float splitAngleCorrection;
    float splitProb;
    float offsetInTrunk;
    float maxLengthChild;
    float radiusLimit;

    static vcl::rand_generator rand;

    /**
     * Generates the tree by using a TurtleGraphics and recursively creating child branches/leaves as necessary.
     * It will also split the current branch with a probability.
     */
    void generate();

    /**
     * Makes branches on the current segment.
     */
    void makeBranches(int segIdx, float branchesOnSeg, float &prevRotationAngle);

    /**
     * Makes a single branch.
     */
    void makeBranch(int branchIdx, float offset, float offsetInParent, float &prevRotationAngle,
            int nBranchesInGroup = 0);

    /**
     * Makes leaves on the current segment.
     */
    void makeLeaves(int segIdx, float leavesOnSeg, float &prevRotationAngle);

    /**
     * Makes a single leaf.
     */
    void makeLeaf(float offset, float offsetInParent, float &prevRotationAngle);

    /**
     * Splits the current branch.
     */
    void makeSplits(int segIdx, float nSplits, float curveAngle);

    float calculateLength();

    float calculateRadius();

    float calculateRadiusAtOffset(float offset) const;

    float calculateCurveAngle();

    float calculateDownAngle(float stemOffset);

    float calculateRotateAngle(float prevAngle, int depth = -1);

    float calculateNBranches() const;

    int calculateNLeaves() const;
};


#endif //PGM_STEM_H
