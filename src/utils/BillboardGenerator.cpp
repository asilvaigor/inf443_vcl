//
// Created by igor on 04/05/2020.
//

#include "BillboardGenerator.h"
#include "objects/Object.h"
#include "utils/Constants.h"

BillboardGenerator::BillboardGenerator() {
    isEmpty = true;
    fbos = nullptr;
    textures = nullptr;
    depths = nullptr;
    reference = nullptr;
}

BillboardGenerator::BillboardGenerator(Shaders &shaders, Object *object) {
    isEmpty = false;
    reference = nullptr;
    this->shaders = &shaders;

    auto corners = object->getBoundingBox().getCorners();
    calculateVirtualCameras(corners);
    generateQuads(corners);
    generateBuffers();

    // Drawing first texture
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
    glViewport(0, 0, texture1Width, texture1Height);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    object->drawMesh(virtualCamera1);

    // Drawing second texture
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
    glViewport(0, 0, texture2Width, texture2Height);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    object->drawMesh(virtualCamera2);

    // Unbinding
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

BillboardGenerator::BillboardGenerator(BillboardGenerator *billboard, vcl::vec3 &translation) :
        reference(billboard), translation(translation) {
    isEmpty = billboard->empty();
}

BillboardGenerator::~BillboardGenerator() {
    if (!empty()) {
        // FIXME: Opengl throwing multiple errors when deleting these, not sure why.
//        glDeleteTextures(2, textures);
//        glDeleteRenderbuffers(2, depths);
//        glDeleteFramebuffers(2, fbos);
//        delete[]textures;
//        delete[]depths;
//        delete[]fbos;
    }
}

void BillboardGenerator::draw(vcl::camera_scene &camera, std::shared_ptr<vcl::light_source> &light) {
    if (reference != nullptr) {
        reference->quad1.uniform.transform.translation = translation;
        reference->quad2.uniform.transform.translation = translation;
        reference->draw(camera, light);
        return;
    }

    // Depth mask has to be disabled to use alpha channel correctly
    if (!shaders->isOverridden())
        glDepthMask(false);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    quad1.uniform.lights = {light};
    vcl::draw(quad1, camera);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    quad2.uniform.lights = {light};
    vcl::draw(quad2, camera);

    if (!shaders->isOverridden())
        glDepthMask(true);
}

bool BillboardGenerator::empty() const {
    return isEmpty;
}

void BillboardGenerator::calculateVirtualCameras(std::vector<vcl::vec3> &corners) {
    calculateVirtualCamera(virtualCamera1, corners[0], corners[1], corners[2], true);
    calculateVirtualCamera(virtualCamera2, corners[0], corners[1], corners[6], false);
}

void BillboardGenerator::calculateVirtualCamera(vcl::camera_scene &camera, vcl::vec3 &p1, vcl::vec3 &p2, vcl::vec3 &p3,
                                                bool xIsConstant) {
    float width;
    if (xIsConstant)
        width = p3.y - p1.y;
    else width = p3.x - p1.x;
    float height = p2.z - p1.z;

    // Putting the camera at a distance so the height is equivalent to the BILLBOARD_RATIO_THRESHOLD
    float dist = height / (Constants::BILLBOARD_RATIO_THRESHOLD * std::tan(Constants::CAMERA_ANGLE / 2));
    // Picking the angle to cover perfectly the bounding box face at the required distance
    float angle = 2 * std::atan(height / (2 * dist));

    camera.scale = 1.0f;
    camera.perspective = vcl::perspective_structure(angle, width / height, Constants::CAMERA_Z_NEAR,
                                                    Constants::CAMERA_Z_FAR);
    vcl::mat3 r;
    if (xIsConstant) {
        r.rotate_y(M_PI_2);
        r.rotate_x(M_PI_2);
    } else {
        r.rotate_x(M_PI_2);
    }
    camera.orientation = r;
    if (xIsConstant) {
        camera.translation.x = -p1.x + camera.orientation.xz + dist;
        camera.translation.y = -0.5f * (p1.y + p3.y) + camera.orientation.yz;
    } else {
        camera.translation.x = -0.5f * (p1.x + p3.x) + camera.orientation.xz;
        camera.translation.y = -p1.y + camera.orientation.yz + dist;
    }
    camera.translation.z = -0.5f * (p1.z + p2.z) + camera.orientation.zz;

    // Setting the texture sizes
    if (xIsConstant) {
        texture1Width = roundf(width * Constants::BILLBOARD_TEXTURE_SIZE_RATIO);
        texture1Height = roundf(height * Constants::BILLBOARD_TEXTURE_SIZE_RATIO);
    } else {
        texture2Width = roundf(width * Constants::BILLBOARD_TEXTURE_SIZE_RATIO);
        texture2Height = roundf(height * Constants::BILLBOARD_TEXTURE_SIZE_RATIO);
    }
}

void BillboardGenerator::generateQuads(std::vector<vcl::vec3> &corners) {
    auto q1 = (corners[0] + corners[4]) / 2.0f, q2 = (corners[1] + corners[5]) / 2.0f,
            q3 = (corners[2] + corners[6]) / 2.0f, q4 = (corners[3] + corners[7]) / 2.0f;
    vcl::mesh mesh = vcl::mesh_primitive_quad(q3, q1, q2, q4);
    quad1 = vcl::mesh_drawable(mesh);
    quad1.shader = shaders->operator[]("mesh");
    quad1.uniform.shading = {0.8f, 0.3f, 0.0, 32};

    q1 = (corners[0] + corners[2]) / 2.0f;
    q2 = (corners[1] + corners[3]) / 2.0f;
    q3 = (corners[4] + corners[6]) / 2.0f;
    q4 = (corners[5] + corners[7]) / 2.0f;
    mesh = vcl::mesh_primitive_quad(q1, q3, q4, q2);
    quad2 = vcl::mesh_drawable(mesh);
    quad2.shader = shaders->operator[]("mesh");
    quad2.uniform.shading = quad1.uniform.shading;
}

void BillboardGenerator::generateBuffers() {
    fbos = new GLuint[2];
    textures = new GLuint[2];
    depths = new GLuint[2];
    glGenFramebuffers(2, fbos);
    glGenTextures(2, textures);
    glGenRenderbuffers(2, depths);

    // Texture 1
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture1Width, texture1Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[0], 0);

    // Depth buffer 1
    glBindRenderbuffer(GL_RENDERBUFFER, depths[0]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texture1Width, texture1Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depths[0]);

    // Texture 2
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture2Width, texture2Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[1], 0);

    // Depth buffer 2
    glBindRenderbuffer(GL_RENDERBUFFER, depths[1]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texture2Width, texture2Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depths[1]);

    // Enabling stuff
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
