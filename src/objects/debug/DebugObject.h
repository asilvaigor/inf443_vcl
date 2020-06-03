//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_DEBUGOBJECT_H
#define PGM_DEBUGOBJECT_H

#include "objects/Object.h"

class DebugObject : public Object {
public:
    DebugObject(Shaders& shaders);

    void drawMesh(vcl::camera_scene &camera) override;

    void addMesh(vcl::mesh_drawable mesh);

private:
    std::vector<vcl::mesh_drawable> meshes;
    Shaders& shaders;
};


#endif //PGM_DEBUGOBJECT_H
