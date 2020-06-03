//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_COMPANIONOBJECT_H
#define PGM_COMPANIONOBJECT_H

#include <src/objects/Object.h>

class CompanionObject : public Object {
public:
    CompanionObject(Shaders& shaders, bool debug);

    void drawMesh(vcl::camera_scene &camera) override;
protected:
    // Mesh
    vcl::mesh_drawable mesh;

    vcl::vec3 dp;
    float currentTime;

    // Debug
    bool debug;
};


#endif //PGM_COMPANIONOBJECT_H
