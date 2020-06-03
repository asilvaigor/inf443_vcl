//
// Created by igor on 04/05/2020.
//

#ifndef PGM_BILLBOARDGENERATOR_H
#define PGM_BILLBOARDGENERATOR_H

#include <memory>
#include "vcl.hpp"
#include "shaders/Shaders.h"
#include "utils/BoundingBox.h"

class Object;

/**
 * Generates and draws billboards from objects in the scene.
 * It will draw two perpendicular billboards which correspond to the object seen from that angle.
 */
class BillboardGenerator {
public:
    /**
     * Does nothing. The billboards will be empty.
     */
    BillboardGenerator();

    /**
     * Generates the buffers and draws both billboards in textures.
     * @param shaders
     * @param object
     */
    BillboardGenerator(Shaders &shaders, Object *object);

    /**
     * Stores a pointer to another billboard, and will draw it with a translation.
     * @param billboard
     * @param translation
     */
    BillboardGenerator(BillboardGenerator *billboard, vcl::vec3 &translation);

    /**
     * Deletes buffers.
     */
    ~BillboardGenerator();

    /**
     * Draws the two perpendicular billboards.
     * @param camera
     * @param light
     */
    void draw(vcl::camera_scene &camera, std::shared_ptr<vcl::light_source> &light);

    /**
     * Check if the billboards exist.
     * @return
     */
    bool empty() const;

    vcl::mesh_drawable quad1;
    vcl::mesh_drawable quad2;

private:
    Shaders *shaders;
    bool isEmpty;
    GLuint *fbos;
    GLuint *textures;
    GLuint *depths;
    int texture1Width, texture1Height;
    int texture2Width, texture2Height;
    vcl::camera_scene virtualCamera1;
    vcl::camera_scene virtualCamera2;
    BillboardGenerator *reference;
    vcl::vec3 translation;

    /**
     * Calculates two camera angles that cover two faces of the bounding box.
     * @param corners Corners of the bounding box.
     */
    void calculateVirtualCameras(std::vector<vcl::vec3> &corners);

    void calculateVirtualCamera(vcl::camera_scene &camera, vcl::vec3 &p1, vcl::vec3 &p2, vcl::vec3 &p3,
            bool xIsConstant);

    /**
     * Generates two rectangle meshes to draw the textures in.
     * @param corners Corners of the bounding box.
     */
    void generateQuads(std::vector<vcl::vec3> &corners);

    /**
     * Generates the framebuffers, textures and depth buffers that are used.
     */
    void generateBuffers();
};


#endif //PGM_BILLBOARDGENERATOR_H
