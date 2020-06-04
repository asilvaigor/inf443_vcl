//
// Created by igor on 02/06/2020.
//

#include "Snow.h"
#include "scene/Scene.h"

vcl::rand_generator Snow::generator(Scene::deterministic);

Snow::Snow(Shaders &shaders, int nParticles) :
        Object(true, true), nParticles(nParticles) {
    camera = nullptr;
    cameraDistance = 0.5f;
    minFlakeSize = 0.0005f;
    flakeSizeVar = 0.001f;
    dampingMassRatio = 1.0f;
    accelerationNoise = 0.03f;

    vcl::mesh m;
    m.position.push_back({0.0f, 1.0f, 0.0f});
    m.position.push_back({-0.866f, -0.5f, 0.0f});
    m.position.push_back({0.866f, 0.5f, 0.0f});
    m.connectivity.push_back({0, 1, 2});

    particles.resize(nParticles);
    timeLastNoise.resize(nParticles);
    noise.resize(nParticles);
    for (int i = 0; i < nParticles; i++) {
        particles[i] = vcl::mesh_drawable(m);
        particles[i].shader = shaders["snow"];
        particles[i].uniform.color = {1, 1, 1};
        timeLastNoise[i] = generator.rand(0.0f, 1.0f);
        noise[i] = {generator.rand(-accelerationNoise, accelerationNoise),
                    generator.rand(-accelerationNoise, accelerationNoise)};
    }

    velocities.resize(nParticles);
    accelerations.resize(nParticles);
    for (int i = 0; i < nParticles; i++) {
        velocities[i].z = -0.03;
        auto &p = particles[i].uniform.transform.translation;
        p.x = generator.rand(-cameraDistance, cameraDistance);
        p.y = generator.rand(-cameraDistance, cameraDistance);
        p.z = generator.rand(-cameraDistance, cameraDistance);
    }
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
    vcl::vec3 camDelta = cam - lastCameraPosition;

    for (int i = 0; i < nParticles; i++) {
        auto &p = particles[i].uniform.transform.translation;
        auto &v = velocities[i];
        auto &a = accelerations[i];
        p += 0.9f * camDelta;
        auto delta = p - cam;

        // Reset particle if it escaped the cube
        if (fabs(delta.x) > cameraDistance || fabs(delta.y) > cameraDistance || fabs(delta.z) > cameraDistance) {
            p.x = cam.x + generator.rand(-cameraDistance, cameraDistance);
            p.y = cam.y + generator.rand(-cameraDistance, cameraDistance);
            p.z = cam.z + generator.rand(-cameraDistance, cameraDistance);
        } else {
            // Physical equations: gravity and air forces, plus a noise
            if (time - timeLastNoise[i] > 1.0f) {
                noise[i] = vcl::vec2(generator.rand(-accelerationNoise, accelerationNoise),
                                     generator.rand(-accelerationNoise, accelerationNoise));
                timeLastNoise[i] = time;
            }
            a.x = -dampingMassRatio * v.x + noise[i].x;
            a.y = -dampingMassRatio * v.y + noise[i].y;
            v += a * dt;
            p += v * dt;
        }

        particles[i].uniform.transform.scaling = minFlakeSize + flakeSizeVar * (delta.norm() / cameraDistance);
    }

    lastCameraPosition = cam;
    lastTime = time;
}
