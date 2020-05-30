//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include "Water.h"

Water::Water(Shaders &shaders, float xSize, float ySize) : Object(false), xSize(xSize), ySize(ySize) {
    uDimensionSize = 100;
    vDimensionSize = 100;

    // Initializing water mesh
    initialize_mesh();

    // Initializing oscillator
    oscillator = {0, 0, 0};
    oscilator_mesh = vcl::mesh_primitive_sphere(1);
    oscilator_mesh.shader = shaders["mesh"];
    oscilator_mesh.uniform.color = {1, 0, 0};

    // Adding shader
    waterMeshDrawable.shader = shaders["mesh"];

    // Setting current time
    timer = std::chrono::system_clock::now();
    timeBegin = std::chrono::system_clock::now();

    waterMeshDrawable.uniform.color = {0, 0.5, 1};
    waterMeshDrawable.uniform.color_alpha = 0.7;
}

void Water::drawMesh(vcl::camera_scene &camera) {
    // TODO fix this mess

    // Enable use of alpha component as color blending for transparent elements
    //  new color = previous color + (1-alpha) current color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furthest to nearest elements
    glDepthMask(false);

    waterMeshDrawable.uniform.light = light;
    // Updating animation
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - timer;
    // Chance animation rate

    //TODO add parameter for this
    if (duration.count() > 1/60){
//        std::cout << "here\n";

        update_oscillators();
        update_mesh();
        timer = std::chrono::system_clock::now();
    }

    // TODO remove drawing this marker
    vcl::draw(waterMeshDrawable, camera);
    glDepthMask(true);

    oscilator_mesh.uniform.light = light;
    oscilator_mesh.uniform.transform.translation = oscillator;
    vcl::draw(oscilator_mesh, camera);
}

void Water::initialize_mesh() {
    waterMesh.position.resize(uDimensionSize*vDimensionSize);
    waterPositions.resize(uDimensionSize);
    waterVerticalSpeeds.resize(uDimensionSize);
    waterUpdatedVerticalPositions.resize(uDimensionSize);

    // Fill terrain geometry
    for(size_t ku=0; ku<uDimensionSize; ++ku)
    {
        waterPositions[ku].resize(vDimensionSize);
        waterVerticalSpeeds[ku].resize(vDimensionSize);
        waterUpdatedVerticalPositions[ku].resize(vDimensionSize);
        for(size_t kv=0; kv<vDimensionSize; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/((float)uDimensionSize-1.0f);
            const float v = kv/((float)vDimensionSize-1.0f);

            // Compute coordinates
            const float x = xSize*(u-0.5f);
            const float y = ySize*(v-0.5f);
            waterMesh.position[kv+uDimensionSize*ku] = {x, y, 0};

            // Initializing height matrix
            waterPositions[ku][kv] = {x, y, 0};
            waterVerticalSpeeds[ku][kv] = 0.0f;
            waterUpdatedVerticalPositions[ku][kv] = 0.0f;
        }
    }

    // Generate triangle organization
    // Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(unsigned int ku=0; ku<(unsigned int) uDimensionSize-1; ++ku)
    {
        for(unsigned int kv=0; kv<(unsigned int) vDimensionSize-1; ++kv)
        {
            const unsigned int idx = kv + ((unsigned int)uDimensionSize)*ku; // current vertex offset

            const vcl::uint3 triangle_1 = {idx, idx+1+(unsigned int)vDimensionSize, idx+1};
            const vcl::uint3 triangle_2 = {idx, idx+(unsigned int)vDimensionSize, idx+1+(unsigned int)vDimensionSize};

            waterMesh.connectivity.push_back(triangle_1);
            waterMesh.connectivity.push_back(triangle_2);
        }
    }

    waterMeshDrawable = vcl::mesh_drawable(waterMesh);

    update_mesh();
}

void Water::update_mesh() {
    vcl::buffer<vcl::vec3> position(uDimensionSize*vDimensionSize);
    update_heights();

    // Fill terrain geometry
    for(size_t ku=0; ku<uDimensionSize; ++ku)
    {
        for(size_t kv=0; kv<vDimensionSize; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/((float)uDimensionSize-1.0f);
            const float v = kv/((float)vDimensionSize-1.0f);

            // Compute coordinates
            const float x = xSize*(u-0.5f);
            const float y = ySize*(v-0.5f);
            std::chrono::duration<double> elapsed = std::chrono::system_clock::now()-timeBegin;
            float time = sin(elapsed.count()+30*u);
            position[kv+uDimensionSize*ku] = waterPositions[ku][kv];
        }
    }
    waterMeshDrawable.update_position(position);
}

void Water::update_oscillators() {
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now()-timeBegin;
    oscillator.z = sin(2*elapsed.count());
}

void Water::update_heights() {
    for (size_t u = 0; u < uDimensionSize; ++u){
        for (size_t v = 0; v < vDimensionSize; ++v){
            vcl::vec3 pos = waterPositions[u][v];

            // Calculating electrostatic force
            float m = 1;
            float q = 1;
            float d = vcl::norm(pos-oscillator);
            vcl::vec3 force = q/(pow(d,3))*(oscillator-pos);

            waterVerticalSpeeds[u][v] += force.z/m;

            if (u > 0 && v > 0 && u < uDimensionSize-1 && v < vDimensionSize-1){
                float north = waterPositions[u][v+1].z;
                float south = waterPositions[u][v-1].z;
                float east = waterPositions[u+1][v].z;
                float west = waterPositions[u-1][v].z;

                waterVerticalSpeeds[u][v] += 0.002*((north+south+east+west)/4-waterPositions[u][v].z);
            }

            waterVerticalSpeeds[u][v] *= 0.99;

            waterPositions[u][v].z += waterVerticalSpeeds[u][v];
        }
    }
}


