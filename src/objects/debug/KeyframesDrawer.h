//
// Created by igor on 02/06/2020.
//

#ifndef PGM_KEYFRAMESDRAWER_H
#define PGM_KEYFRAMESDRAWER_H

#include "objects/Object.h"

class KeyframesDrawer : public Object {
public:
    KeyframesDrawer(Shaders &shaders, std::vector<vcl::vec3> &keyframes);

    void drawMesh(vcl::camera_scene &camera) override;

public:
    std::vector<vcl::mesh_drawable> drawables;
    std::vector<vcl::vec3> &keyframes;
};


#endif //PGM_KEYFRAMESDRAWER_H
