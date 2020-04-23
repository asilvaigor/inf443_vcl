//
// Created by igor on 15/04/20.
//

#include "vcl.hpp"
#include "Branch.h"

vcl::rand_generator Branch::rand(false);

Branch::Branch(TreeSpecies &s, TurtleGraphics turtle, int depth, int startIdx, Branch *parent, float treeScale,
               float nBranchesFactor, float splitAngleCorrection, float cloneProb, float offsetInTrunk,
               float radiusLimit) : species(s) {
    this->depth = depth;
    this->startIdx = startIdx;
    this->parent = parent;
    this->treeScale = treeScale;
    this->nBranchesFactor = nBranchesFactor;
    this->splitAngleCorrection = splitAngleCorrection;
    this->offsetInTrunk = offsetInTrunk;
    this->radiusLimit = radiusLimit;
    this->turtle = turtle;

    this->cloneProb = cloneProb;
    if (depth > 0)
        this->cloneProb *= 1 - 0.8f * offsetInTrunk / parent->length;

    if (treeScale < FLT_EPSILON)
        this->treeScale = species.scale + rand.rand() * species.scaleVar;
    maxLengthChild = species.length[depth + 1] + rand.rand() * species.lengthVar[depth + 1];
    length = calculateLength();
    radius = calculateRadius();

    generate();
}

vcl::mesh Branch::toMesh() {
    auto mesh = spline.toMesh();
    for (auto &s : branches) {
        auto m = s.toMesh();
        mesh.add(m);
    }
    return mesh;
}

vcl::mesh Branch::toLeavesMesh() {
    vcl::mesh mesh;
    for (auto &l : leaves) {
        auto m = l.toMesh();
        mesh.add(m);
    }

    for (auto &s : branches) {
        auto m = s.toLeavesMesh();
        mesh.add(m);
    }
    return mesh;
}

vcl::mesh Branch::toSnowyLeavesMesh() {
    vcl::mesh mesh;
    for (auto &l : snowyLeaves) {
        auto m = l.toMesh();
        mesh.add(m);
    }

    for (auto &s : branches) {
        auto m = s.toSnowyLeavesMesh();
        mesh.add(m);
    }
    return mesh;
}

void Branch::generate() {
    int curveResolution = species.curveResolution[depth];
    int segSplits = species.segSplits[depth];
    float segLength = length / curveResolution;
    int nakedSegmentIdx = (int) ceil(species.nakedRatio[depth] * curveResolution);

    int nLeaves = 0;
    float leavesOnSeg;
    int nBranches = 0;
    float branchesOnSeg;
    if (depth == species.levels - 1 && species.leafBlosNum > 0) {
        nLeaves = calculateNLeaves();
        nLeaves *= 1 - startIdx / curveResolution;
        leavesOnSeg = (float) nLeaves / curveResolution;
    } else {
        nBranches = calculateNBranches();
        nBranches *= 1 - startIdx / curveResolution;
        nBranches *= nBranchesFactor;
        branchesOnSeg = (float) nBranches / curveResolution;
    }

    // Starting with rand.random rotation
    float rotationAngle = 0.0f;
    if (species.rotateAngle[depth + 1] > FLT_EPSILON)
        rotationAngle = rand.rand(0, (float) (2 * M_PI));

    for (int segIdx = startIdx; segIdx <= curveResolution; segIdx++) {
        vcl::vec3 leftHandle = turtle.getPosition() - turtle.getDirection() * length / (curveResolution * 3);
        vcl::vec3 rightHandle = turtle.getPosition() + turtle.getDirection() * length / (curveResolution * 3);
        float r = calculateRadiusAtOffset(1.0f * segIdx / curveResolution);
        spline.addPoint(turtle.getPosition(), r, leftHandle, rightHandle);

        if (segIdx > startIdx) {
            float nSplits = 0;
            if (species.nakedSplits > 0 && depth == 0 && segIdx == nakedSegmentIdx) {
                nSplits = species.nakedSplits;
            } else if (segSplits > 0 && (depth > 0 || segIdx > nakedSegmentIdx) && segIdx < curveResolution - 1)
                if (rand.rand(0, 1) <= cloneProb) {
                    nSplits = segSplits;

                    cloneProb /= (int) nSplits + 1;
                    nBranchesFactor /= nSplits + 1;
                    nBranchesFactor = std::max(0.8f, nBranchesFactor);

                    nBranches = (int) round(nBranches * nBranchesFactor);
                    branchesOnSeg = 1.0f * nBranches / curveResolution;
                }

            auto state = rand.getState();

            if (nBranches > 0 && depth < species.levels - 1 && branchesOnSeg > 0)
                makeBranches(segIdx, branchesOnSeg, rotationAngle);
            else if (nLeaves > 0 && leavesOnSeg > 0)
                makeLeaves(segIdx, leavesOnSeg, rotationAngle);

            rand.setState(state);

            if (segIdx > startIdx) {
                if (nSplits > 0) {
                    float curveAngle = species.splitAngle[depth] + rand.rand() * species.splitAngleVar[depth];
                    splitAngleCorrection = 0;

                    auto newState = rand.getState();
                    makeClones(segIdx, nSplits, curveAngle);
                    rand.setState(newState);

                    if (nSplits == 1)
                        turtle.turnRight(curveAngle);
                } else {
                    turtle.turnLeft(rand.rand() * species.bendVar[depth] / curveResolution);
                    float curveAngle = calculateCurveAngle();
                    turtle.pitchDown(curveAngle - splitAngleCorrection);
                }
            }
        }

        turtle.move(segLength);
    }
}

void Branch::makeBranches(int segIdx, float branchesOnSeg, float prevRotationAngle) {
    float nakedLength = length * species.nakedRatio[depth];
    float branchDist = species.branchDist[depth + 1];
    float curveResolution = species.curveResolution[depth];

    int nWhorls = (int) (branchesOnSeg / (branchDist + 1.0f));
    float branchesPerWhorl = branchDist + 1.0f;
    float branchWhorlError = 0.0f;

    for (int w = 0; w < nWhorls; w++) {
        float offset = 1.0f * w / nWhorls;
        float offsetInParent = (((segIdx - 1) + offset) / curveResolution) * length;

        if (offsetInParent > nakedLength) {
            int nBranchesThisWhorl = (int) (branchesPerWhorl + branchWhorlError);
            branchWhorlError -= nBranchesThisWhorl - branchesPerWhorl;

            for (int i = 0; i < nBranchesThisWhorl; i++)
                makeBranch(i, offset, offsetInParent, prevRotationAngle, nBranchesThisWhorl);
        }

        prevRotationAngle += species.rotateAngle[depth + 1];
    }
}

void Branch::makeBranch(int branchIdx, float offset, float offsetInParent, float prevRotationAngle, int nBranchesInGroup) {
    vcl::vec3 pos = spline.position(spline.getNPoints() - 2, offset);
    vcl::vec3 dir = spline.tangent(spline.getNPoints() - 2, offset);
    vcl::vec3 right = vcl::cross(turtle.getRight(), dir);
    TurtleGraphics newTurtle(pos, dir, right);

    float rollAngle = prevRotationAngle + (float) (2 * M_PI * branchIdx / nBranchesInGroup) +
                      rand.rand() * species.rotateAngleVar[depth + 1];
    float r = calculateRadiusAtOffset(offsetInParent / length);
    newTurtle.rollCw(rollAngle);
    newTurtle.pitchDown(calculateDownAngle(offsetInParent));
    newTurtle.move(r);

    branches.emplace_back(Branch(species, newTurtle, depth + 1, 0, this, treeScale,
                                 1, 0, 1, offsetInParent, r));
}

void Branch::makeLeaves(int segIdx, float leavesOnSeg, float prevRotationAngle) {
    float nakedLength = length * species.nakedRatio[depth];
    float leavesDist = species.leavesDist;
    float curveResolution = species.curveResolution[depth];

    for (int i = 0; i < leavesOnSeg; i++) {
        float offset;
        if (segIdx % 2 == 0)
            offset = 1.0f * i / leavesOnSeg;
        else offset = 1.0f * (i - leavesDist) / leavesOnSeg;
        float offsetInParent = (((segIdx - 1) + offset) / curveResolution) * length;

        if (offsetInParent > nakedLength)
            makeLeaf(offset, offsetInParent, prevRotationAngle);
        prevRotationAngle += species.leavesRotateAngle;
    }
}

void Branch::makeLeaf(float offset, float offsetInParent, float prevRotationAngle) {
    vcl::vec3 pos = spline.position(spline.getNPoints() - 2, offset);
    vcl::vec3 dir = spline.tangent(spline.getNPoints() - 2, offset);
    vcl::vec3 right = vcl::cross(turtle.getRight(), dir);
    TurtleGraphics newTurtle(pos, dir, right);

    float rollAngle = calculateRotateAngle(prevRotationAngle);
    float r = calculateRadiusAtOffset(offsetInParent / length);
    newTurtle.rollCw(rollAngle);
    newTurtle.pitchDown(calculateDownAngle(offsetInParent));
    newTurtle.move(r);

    float scale = treeScale * (1.0f - 0.6f * offsetInTrunk / parent->length - 0.2f * offsetInParent / length);

    vcl::vec3 zAxis(0, 0, 1);
    if (newTurtle.getDirection().angle(zAxis) > M_PI * 0.4)
        leaves.emplace_back(Leaf(species, newTurtle, scale));
    else snowyLeaves.emplace_back(Leaf(species, newTurtle, scale));
}

void Branch::makeClones(int segIdx, float nSplits, float curveAngle) {
    for (int splitIdx = 0; splitIdx < (int) nSplits; splitIdx++) {
        float effCurveAngle;
        if (splitIdx == 0)
            effCurveAngle = curveAngle / 2;
        else effCurveAngle = -curveAngle / 2;

        auto newTurtle = turtle;
        newTurtle.turnLeft(effCurveAngle);

        branches.emplace_back(Branch(species, newTurtle, depth, segIdx, parent, treeScale, nBranchesFactor,
                                     splitAngleCorrection, cloneProb, offsetInTrunk, radiusLimit));
    }
}

float Branch::calculateLength() {
    float l;
    if (depth == 0) {
        l = treeScale * (species.length[0] + rand.rand() * species.lengthVar[0]);
    } else if (depth == 1) {
        l = parent->length * parent->maxLengthChild *
            (0.2f + 0.8f * (parent->length - offsetInTrunk) / (parent->length - species.nakedRatio[0]));
    } else l = parent->maxLengthChild * (parent->length - 0.7f * offsetInTrunk);

    l = std::max(0.0f, l);
    return l;
}

float Branch::calculateRadius() {
    float r;
    if (depth == 0) {
        r = length * species.ratio;
    } else {
        r = parent->radius * pow((length / parent->length), species.ratioPow);
        r = std::max(0.005f, r);
        r = std::min(r, radiusLimit);
    }
    return r;
}

float Branch::calculateRadiusAtOffset(float offset) const {
    float taper = radius * (1 - offset);
    float remaining = (1 - offset) * length;

    float r;
    if (remaining >= taper)
        r = taper;
    else r = (float) (sqrt(pow(taper, 2) - pow((remaining - taper), 2)));
    if (depth == 0) {
        float y_val = std::max(0.0f, 1.0f - 8.0f * offset);
        float flare = species.flare * ((pow(100.0f, y_val) - 1) / 100.0f) + 1.0f;
        r *= flare;
    }
    return r;
}

float Branch::calculateCurveAngle() {
    float curveAngle = species.curve[depth] / species.curveResolution[depth];
    curveAngle += rand.rand() * (species.curveVar[depth] / species.curveResolution[depth]);
    return curveAngle;
}

float Branch::calculateDownAngle(float stemOffset) {
    int nextDepth = depth + 1;
    float angle;
    if (species.downAngleVar[nextDepth] >= FLT_EPSILON) {
        angle = species.downAngle[nextDepth] +
                rand.rand() * species.downAngleVar[nextDepth];
    } else {
        angle = species.downAngle[nextDepth] + species.downAngleVar[nextDepth] *
                                               (1 - 2 * (0.2f + 0.8f * (length - stemOffset) /
                                                                (length * (1 - species.nakedRatio[depth]))));
    }
    angle += rand.rand() * abs(angle * 0.1f);
    return angle;
}

float Branch::calculateRotateAngle(float prevAngle) {
    float rotateAngle;
    if (species.rotateAngle[depth] >= FLT_EPSILON) {
        rotateAngle = prevAngle + species.rotateAngle[depth] + rand.rand() * species.rotateAngleVar[depth];
    } else {
        rotateAngle = prevAngle * ((float) M_PI + species.rotateAngleVar[depth] +
                                   rand.rand() * species.rotateAngleVar[depth]);
    }

    while (rotateAngle > (float) (2 * M_PI))
        rotateAngle -= 2 * M_PI;
    while (rotateAngle < 0)
        rotateAngle += 2 * M_PI;

    return rotateAngle;
}

int Branch::calculateNBranches() const {
    int nextDepth = depth + 1;
    if (nextDepth >= species.levels)
        return 0;

    int nBranches;
    if (depth == 0) {
        nBranches = species.nBranches[nextDepth] +
                    (int) round(rand.rand() * species.nBranchesVar[nextDepth]);
    } else {
        if (depth == 1) {
            nBranches = (int) round(species.nBranches[nextDepth] * (0.2 + 0.8 * (
                    length / parent->length) / parent->maxLengthChild));
        } else {
            nBranches = (int) round(species.nBranches[nextDepth] *
                                    (1.0 - 0.5 * offsetInTrunk / parent->length));
        }
    }
    return nBranches;
}

int Branch::calculateNLeaves() const {
    float nLeaves = (float) species.leafBlosNum * treeScale / species.scale;
    nLeaves *= length / (parent->maxLengthChild * parent->length);
    return (int) nLeaves;
}
