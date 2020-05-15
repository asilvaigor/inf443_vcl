//
// Created by igor on 25/04/2020.
//

#include "Rock.h"

Rock::Rock(Shaders &shaders, vcl::vec3 base, float snowCoverage, vcl::vec3 ellisoidAxisSize, bool verbose) :
        Object(false), rockTexture("rock"), snowTexture("snow") {
    calculateMesh(base, ellisoidAxisSize);
    putSnow(snowCoverage);

    rock = vcl::mesh_drawable(mesh);
    rock.shader = shaders["mesh"];
    rock.uniform.shading = {0.2, 0.8, 0.2, 128};
    snow = vcl::mesh_drawable(snowMesh);
    snow.shader = shaders["mesh"];
    snow.uniform.shading = {0.2, 0.8, 0.1, 32};

    std::vector<vcl::vec3 *> points(mesh.position.size() + snowMesh.position.size());
    for (auto &p : mesh.position)
        points.push_back(&p);
    for (auto &p : snowMesh.position)
        points.push_back(&p);
    boundingSphere = BoundingSphere(mesh);

    for (auto &pt : mesh.position)
        boundingBox.update(pt);
    for (auto &pt : snowMesh.position)
        boundingBox.update(pt);
    billboard = BillboardGenerator(shaders, this);

    if (verbose) {
        std::cout << "Number of vertices: total " << mesh.position.size() + snowMesh.position.size();
        std::cout << " rock " << mesh.position.size();
        std::cout << " snow " << snowMesh.position.size();
        std::cout << std::endl;
    }
}

void Rock::drawMesh(vcl::camera_scene &camera, float) {
    rock.uniform.light = light;
    rockTexture.bind();
    vcl::draw(rock, camera);
    snow.uniform.light = light;
    snowTexture.bind();
    vcl::draw(snow, camera);
}

void Rock::calculateMesh(vcl::vec3 &base, vcl::vec3 &ellipsoidAxisSize) {
    mesh = vcl::mesh_primitive_semi_ellipsoid(ellipsoidAxisSize[0], ellipsoidAxisSize[1], ellipsoidAxisSize[2], base);

    for (auto &p : mesh.position) {
        const float scaling = 1.7;
        const int octave = 7;
        const float persistency = 0.2;
        const float height = 0.2;
        float noise;

        noise = vcl::perlin(scaling * p.x, scaling * p.y, octave, persistency);
        if (p.z - base.z > FLT_EPSILON)
            p.z += height * noise;
        noise = vcl::perlin(scaling * p.x, scaling * p.y, octave, persistency);
        p.x += (p.x - base.x) > 0 ? height * noise : -height * noise;
        noise = vcl::perlin(scaling * p.x, scaling * p.y, octave, persistency);
        p.y += (p.y - base.y) > 0 ? height * noise : -height * noise;
    }
}

void Rock::putSnow(float snowCoverage) {
    int n = mesh.position.size();
    std::vector<unsigned int> idxMap(n, n);
    int snowMeshSize = 0;
    vcl::vec3 zAxis(0, 0, 1);

    vcl::buffer<vcl::uint3> connectivity2;
    for (auto &c : mesh.connectivity) {
        vcl::vec3 normal = (mesh.normal[c[0]] + mesh.normal[c[1]] + mesh.normal[c[2]]) / 3.0f;
        // If it has a low angle with the z axis, it will be turn into snow
        if (normal.angle(zAxis) < snowCoverage * M_PI_2) {
            for (int i = 0; i < 3; i++) {
                if (idxMap[c[i]] == (unsigned int) n) {
                    snowMesh.position.push_back(mesh.position[c[i]]);
                    snowMesh.normal.push_back(mesh.normal[c[i]]);
                    snowMesh.texture_uv.push_back(mesh.texture_uv[c[i]]);
                    idxMap[c[i]] = snowMeshSize++;
                }
                snowMesh.connectivity.push_back({idxMap[c[0]], idxMap[c[1]], idxMap[c[2]]});
            }
        } else connectivity2.push_back(c);
    }
    mesh.connectivity = connectivity2;

    // Removing the snow exclusive points from the rock mesh
    vcl::buffer<vcl::vec3> position2;
    vcl::buffer<vcl::vec3> normal2;
    vcl::buffer<vcl::vec2> texture_uv2;
    for (int i = 0; i < n; i++)
        idxMap[i] = n;

    // Points that are still in the rock
    std::vector<bool> contains(n, false);
    for (auto &c : mesh.connectivity)
        for (int i = 0; i < 3; i++)
            contains[c[i]] = true;

    // Passing the values of position, normal and texture_uv to filtered vectors
    int idx = 0;
    for (int i = 0; i < n; i++)
        if (contains[i]) {
            idxMap[i] = idx++;
            position2.push_back(mesh.position[i]);
            normal2.push_back(mesh.normal[i]);
            texture_uv2.push_back(mesh.texture_uv[i]);
        }

    // Putting the correct values on connectivity
    for (auto &c : mesh.connectivity)
        for (int i = 0; i < 3; i++)
            c[i] = idxMap[c[i]];

    mesh.position = position2;
    mesh.normal = normal2;
    mesh.texture_uv = texture_uv2;
}