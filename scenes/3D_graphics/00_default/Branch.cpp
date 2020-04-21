//
// Created by igor on 15/04/20.
//

#include "vcl/vcl.hpp"
#include "Branch.h"

vcl::rand_generator Branch::rand;

Branch::Branch(TreeSpecies &s, int depth, int startIdx, TurtleGraphics turtle, Branch *parent, float nBranchesFactor,
               float splitAngleCorrection, float cloneProb, float offsetInParent, float radiusLimit) : species(s) {
    this->depth = depth;
    this->startIdx = startIdx;
    this->parent = parent;
    this->nBranchesFactor = nBranchesFactor;
    this->splitAngleCorrection = splitAngleCorrection;
    this->cloneProb = cloneProb;
    this->offsetInParent = offsetInParent;
    this->radiusLimit = radiusLimit;
    this->turtle = turtle;

    maxLengthChild = species.length[depth + 1] + rand.rand() * species.lengthVar[depth + 1];
    length = calculateLength();
    radius = calculateRadius();

    generate();
}

vcl::mesh Branch::toMesh() {
    auto mesh = spline.toMesh();
    for (auto &s : children) {
        auto m = s.toMesh();
        if (m.position.size() > 0)
            mesh.add(m);
    }
    return mesh;
}

void Branch::generate() {
    int curveResolution = species.curveResolution[depth];
    int segSplits = species.segSplits[depth];
    float segLength = length / curveResolution;
    int nakedSegmentIdx = (int) ceil(species.nakedRatio[depth] * curveResolution);
    int nBranches = calculateNBranches();
    nBranches *= 1 - startIdx / curveResolution;
    nBranches *= nBranchesFactor;
    float branchesOnSeg = (float) nBranches / curveResolution;

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

            if (nBranches > 0 && depth < species.levels - 1)
                if (branchesOnSeg > 0)
                    makeBranches(segIdx, branchesOnSeg, rotationAngle);

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

    // If whorled branching
    if (branchDist > 1) {
        int nWhorls = (int) (branchesOnSeg / (branchDist + 1.0f));
        float branchesPerWhorl = branchDist + 1.0f;
        float branchWhorlError = 0.0f;

        for (int w = 0; w < nWhorls; w++) {
            float offset = 1.0f * w / nWhorls;
            float stemOffset = (((segIdx - 1) + offset) / curveResolution) * length;

            if (stemOffset > nakedLength) {
                int nBranchesThisWhorl = (int) (branchesPerWhorl + branchWhorlError);
                branchWhorlError -= nBranchesThisWhorl - branchesPerWhorl;

                for (int i = 0; i < nBranchesThisWhorl; i++)
                    makeBranch(i, 2, offset, stemOffset, prevRotationAngle, nBranchesThisWhorl);
            }

            prevRotationAngle += species.rotateAngle[depth + 1];
        }
    } else {
        for (int i = 0; i < branchesOnSeg; i++) {
            float offset;
            if (segIdx % 2 == 0)
                offset = 1.0f * i / branchesOnSeg;
            else offset = 1.0f * (i - branchDist) / branchesOnSeg;
            float stemOffset = (((segIdx - 1) + offset) / curveResolution) * length;

            if (stemOffset > nakedLength)
                makeBranch(i, 1, offset, stemOffset, prevRotationAngle);
        }
    }
}

void Branch::makeBranch(int branchIdx, int branchMode, float offset, float stemOffset, float prevRotationAngle,
                        int nBranchesInGroup) {
    vcl::vec3 pos = spline.position(spline.getNPoints() - 2, offset);
    vcl::vec3 dir = spline.tangent(spline.getNPoints() - 2, offset);
    vcl::vec3 right = vcl::cross(turtle.getRight(), dir);
    TurtleGraphics newTurtle(pos, dir, right);

    float rollAngle;
    if (branchMode == 1) { // Opposite
        rollAngle = calculateRotateAngle(prevRotationAngle);
    } else { // Whorled
        rollAngle = prevRotationAngle + (float) (2 * M_PI * branchIdx / nBranchesInGroup) +
                    rand.rand() * species.rotateAngleVar[depth + 1];
    }
    float r = calculateRadiusAtOffset(stemOffset / length);
    newTurtle.rollCw(rollAngle);
    newTurtle.pitchDown(calculateDownAngle(stemOffset));
    newTurtle.move(r);

    children.emplace_back(Branch(species, depth + 1, 0, newTurtle, this, 1,
                                 0, 1, stemOffset, r));
}

void Branch::makeClones(int segIdx, float nSplits, float curveAngle) {
    for (int splitIdx = 0; splitIdx < (int) nSplits; splitIdx++) {
        float effCurveAngle;
        if (splitIdx == 0)
            effCurveAngle = curveAngle / 2;
        else effCurveAngle = -curveAngle / 2;

        auto newTurtle = turtle;
        newTurtle.turnLeft(effCurveAngle);

        children.emplace_back(Branch(species, depth, segIdx, newTurtle, parent, nBranchesFactor,
                                     splitAngleCorrection, cloneProb, offsetInParent, radiusLimit));
    }
}

float Branch::calculateLength() {
    float l;
    if (depth == 0) {
        l = species.scale * (species.length[0] + rand.rand() * species.lengthVar[0]);
    } else if (depth == 1) {
        l = parent->length * parent->maxLengthChild *
            (0.2f + 0.8f * (parent->length - offsetInParent) / (parent->length - species.nakedRatio[0]));
    } else l = parent->maxLengthChild * (parent->length - 0.7f * offsetInParent);

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

int Branch::calculateNBranches() {
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
                                    (1.0 - 0.5 * offsetInParent / parent->length));
        }
    }
    return nBranches;
}
