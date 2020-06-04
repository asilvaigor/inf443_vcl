//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#ifndef PGM_OBJECTFOLLOWERCOMPANION_H
#define PGM_OBJECTFOLLOWERCOMPANION_H

#include "DipoleCompanion.h"

class ObjectFollowerCompanion : public DipoleCompanion {
public:
    ObjectFollowerCompanion(Shaders& shaders, std::shared_ptr<Object>& obj, bool debug = true);

    void update(float time) override;

protected:
    std::shared_ptr<Object> obj;
};


#endif //PGM_OBJECTFOLLOWERCOMPANION_H
