//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_COMPANIONOBJECT_H
#define PGM_COMPANIONOBJECT_H

#include <src/objects/Object.h>

class CompanionObject : public Object {
public:
    CompanionObject(Shaders& shaders, bool debug);

    void drawMesh(vcl::camera_scene &camera, float time) override;

    virtual vcl::vec2 getFieldAt(vcl::vec2 pos);

    virtual vcl::vec3 getFieldAt(vcl::vec3 pos);

protected:
    // Mesh related
    vcl::mesh_drawable mesh;
    vcl::mesh_drawable chargeMesh;

    vcl::vec3 dp;
    float currentTime;

    // Chargers variables
    vcl::vec3 pc1, pc2;

    // TODO make this global
    const float T = 1.0/60;
    const float frictionFactor = 0.01f;
    const float distanceDipole = 5.0f;
    const float dipoleOffset = 8.0f;
    const float charge = 15.0f;

    // Debug
    bool debug;

    /**
     * Updates position and speed, must be
     * implemented in each companion object
     * @param time
     */
    virtual void update(float time) = 0;

    void updateChargesPositions();
};


#endif //PGM_COMPANIONOBJECT_H
