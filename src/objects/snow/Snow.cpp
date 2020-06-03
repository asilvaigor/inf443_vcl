//
// Created by igor on 02/06/2020.
//

#include "Snow.h"
#include "scene/Scene.h"

vcl::rand_generator Snow::generator(Scene::deterministic);

Snow::Snow(Shaders &shaders, int nParticles) :
        Object(true, true), nParticles(nParticles) {
    camera = nullptr;

    vcl::mesh m;
    m.position.push_back({0.0f, 1.0f, 0.0f});
    m.position.push_back({-0.866f, -0.5f, 0.0f});
    m.position.push_back({0.866f, 0.5f, 0.0f});
    m.connectivity.push_back({0, 1, 2});

    particles.resize(nParticles);
    for (int i = 0; i < nParticles; i++) {
        particles[i] = vcl::mesh_drawable(m);
        particles[i].shader = shaders["snow"];
        particles[i].uniform.color = {1, 1, 1};
    }

    cameraDistance = 0.5f;
    minFlakeSize = 0.0005f;
    flakeSizeVar = 0.001f;
    airDamping = 0.0005f;
    accelerationNoise = 0.00015f;
    flakeMass = 0.000003f;
    gravity = 9.81f;
    velocities.resize(nParticles);
    accelerations.resize(nParticles);
    for (int i = 0; i < nParticles; i++)
        velocities[i].z = -gravity * flakeMass / airDamping;
}

void Snow::drawMesh(vcl::camera_scene &camera) {
    this->camera = &camera;
    for (int i = 0; i < nParticles; i++)
        vcl::draw_snow(particles[i], camera);
}

void Snow::update(float time) {
    float dt = time - lastTime;
    if (camera == nullptr)
        return;
    vcl::vec3 cam = camera->get_position();

    for (int i = 0; i < nParticles; i++) {
        auto &p = particles[i].uniform.transform.translation;
        auto &v = velocities[i];
        auto &a = accelerations[i];
        auto delta = p - cam;
        particles[i].uniform.transform.scaling = minFlakeSize + flakeSizeVar * (delta.norm() / cameraDistance);

        // Reset particle if it escaped the cube
        if (fabs(delta.x) > cameraDistance || fabs(delta.y) > cameraDistance || fabs(delta.z) > cameraDistance) {
            p.x = cam.x + generator.rand(-cameraDistance, cameraDistance);
            p.y = cam.y + generator.rand(-cameraDistance, cameraDistance);
            p.z = cam.z + generator.rand(-cameraDistance, cameraDistance);
        } else {
            // Physical equations: gravity and air forces, plus a noise
            a.x += -airDamping * v.x + generator.rand(-accelerationNoise, accelerationNoise);
            a.y += -airDamping * v.y + generator.rand(-accelerationNoise, accelerationNoise);
            a.z += -gravity * flakeMass - airDamping * v.z + generator.rand(-accelerationNoise, accelerationNoise);
            v += a * dt;
            p += v * dt;
        }
    }

    lastTime = time;
}
