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
    explicit CoordinateMarker(Shaders& shaders, vcl::vec3 position = {0.0f, 0.0f, 0.0f});

    void drawMesh(vcl::camera_scene &camera) override;

    void setPosition(vcl::vec3 position);

private:
    vcl::mesh_drawable marker;
    vcl::vec3 position;
};


#endif //PGM_COORDINATEMARKER_H
