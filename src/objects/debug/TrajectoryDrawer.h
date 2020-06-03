//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#ifndef PGM_TRAJECTORYDRAWER_H
#define PGM_TRAJECTORYDRAWER_H

#include "objects/Object.h"

class TrajectoryDrawer : public Object {
public:
    TrajectoryDrawer(Shaders& shaders, std::vector<vcl::vec3>& points);

    void drawMesh(vcl::camera_scene &camera) override;
private:
    std::vector<vcl::vec3>& points;
    Shaders& shaders;
    vcl::segment_drawable_immediate_mode segmentDrawer;
};


#endif //PGM_TRAJECTORYDRAWER_H
