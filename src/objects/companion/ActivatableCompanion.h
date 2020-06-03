//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_ACTIVATABLECOMPANION_H
#define PGM_ACTIVATABLECOMPANION_H

#include "CardinalSplineCompanion.h"

class ActivatableCompanion : public CardinalSplineCompanion {
public:
    ActivatableCompanion(Shaders& shaders, std::shared_ptr<vcl::CardinalSpline> trajectory, float initialS,
            bool active = false, bool debug = true);

    bool getActivationState();

    void setActivationState(bool active);
protected:
    bool active;
};


#endif //PGM_ACTIVATABLECOMPANION_H
