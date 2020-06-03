//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#ifndef PGM_ONEWAYCOMPANION_H
#define PGM_ONEWAYCOMPANION_H

#include "ActivatableCompanion.h"

class OneWayCompanion : public ActivatableCompanion {
public:
    OneWayCompanion(Shaders& shaders, std::shared_ptr<vcl::CardinalSpline> &trajectory, float initialS,
            bool active = false, bool debug = true);

    void update(float time) override;
private:
    const float movementRate = 0.005;
};


#endif //PGM_ONEWAYCOMPANION_H
