//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#ifndef PGM_COORDINATEMARKER_H
#define PGM_COORDINATEMARKER_H


#include <src/objects/Object.h>

class CoordinateMarker : public Object {
public:
    /**
     * Initializes a coordinate marker at the origin
     * @param shaders
     */
    CoordinateMarker(Shaders& shaders);

    void drawMesh(vcl::camera_scene &camera) override;

private:
    vcl::mesh_drawable marker;
};


#endif //PGM_COORDINATEMARKER_H
