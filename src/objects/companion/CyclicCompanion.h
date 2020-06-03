//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_CYCLICCOMPANION_H
#define PGM_CYCLICCOMPANION_H

#include "CompanionObject.h"
#include "vcl.hpp"

class CyclicCompanion : public CompanionObject {
    CyclicCompanion(Shaders &shaders, vcl::CyclicCardinalSpline &trajectory, float initialS, bool debug=true);

};


#endif //PGM_CYCLICCOMPANION_H
