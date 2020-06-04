//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#ifndef PGM_BEARFILMINGCOMPANION_H
#define PGM_BEARFILMINGCOMPANION_H

#include <src/objects/companion/ObjectFollowerCompanion.h>
#include "objects/companion/ObjectFollowerCompanion.h"

class BearFilmingCompanion : public ObjectFollowerCompanion {
public:
    BearFilmingCompanion(Shaders& shaders, std::shared_ptr<Object>& obj, bool debug = true);

    void update(float time) override;

private:
    // TODO change constants place
    const float floatingMargin = 15.0f;
    const float floatingRadius = 20.0f;
    const float timeFactor = 0.2f;
};


#endif //PGM_BEARFILMINGCOMPANION_H
