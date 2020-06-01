//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include "Water.h"

Water::Water(Shaders &shaders, WaterLimits& waterLimits, std::vector<WaterOscillator> &oscillators)
        : Object(true), waterLimits(waterLimits), oscillators(oscillators){
    uDimensionSize = 100;
    vDimensionSize = 100;
    xSize = fabs(waterLimits.getX2()-waterLimits.getX1());
    ySize = fabs(waterLimits.getY1()-waterLimits.getY2());

    // Initializing water mesh
    initialize_mesh();

    // Initializing oscillators
    for (auto& oscillator: oscillators){
        if (oscillator.getDebugState())
            oscillator.setShaders(shaders);
    }

    // Adding shader
    waterMeshDrawable.shader = shaders["mesh"];
    waterMeshDrawable.uniform.shading.specular = 0.8f;
    waterMeshDrawable.uniform.shading.diffuse = 0.3f;
    waterMeshDrawable.uniform.shading.ambiant = 0.1f;


    // Setting current time
    timer = std::chrono::system_clock::now();
    std::chrono::system_clock::now();

    waterMeshDrawable.uniform.color = {15.0f/255.0f, 94.0f/255.0f, 156.0f/255.0f};
    waterMeshDrawable.uniform.color_alpha = 0.9;
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
        for (auto& oscillator: oscillators){
            oscillator.step();
        }
        update_mesh();
        timer = std::chrono::system_clock::now();
    }
    vcl::draw(waterMeshDrawable, camera);

//    // TODO dont forget to erase this
    glDepthMask(true);

    // Drawing oscillators
    for (auto& oscillator: oscillators){
        if (oscillator.getDebugState()){
            oscillator.setLight(light);
            vcl::draw(oscillator.getMesh(), camera);
        }
    }
}

void Water::initialize_mesh() {
    waterMesh.position.resize(uDimensionSize*vDimensionSize);
    waterPositions.resize(uDimensionSize);
    waterVerticalSpeeds.resize(uDimensionSize);

    // Fill terrain geometry
    for(size_t ku=0; ku<uDimensionSize; ++ku)
    {
        waterPositions[ku].resize(vDimensionSize);
        waterVerticalSpeeds[ku].resize(vDimensionSize);
        for(size_t kv=0; kv<vDimensionSize; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/((float)uDimensionSize-1.0f);
            const float v = kv/((float)vDimensionSize-1.0f);

            // Compute coordinates
            const float x = xSize*(u) + waterLimits.getX1();
            const float y = ySize*(v) + waterLimits.getY1();
            waterMesh.position[kv+uDimensionSize*ku] = {x, y, waterLimits.getWaterLevel()};

            // Initializing height matrix
            waterPositions[ku][kv] = {x, y, waterLimits.getWaterLevel()};
            waterVerticalSpeeds[ku][kv] = 0.0f;
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

    // Update water positions
    for(size_t ku=0; ku<uDimensionSize; ++ku)
        for(size_t kv=0; kv<vDimensionSize; ++kv)
            position[kv+uDimensionSize*ku] = waterPositions[ku][kv];

    waterMeshDrawable.update_position(position);
}

void Water::update_heights() {
    for (size_t u = 0; u < uDimensionSize; ++u){
        for (size_t v = 0; v < vDimensionSize; ++v){
            vcl::vec3 pos = waterPositions[u][v];

            // Calculating electrostatic force
            // TODO put those constants somewhere
            float m = 1;
            float q = 1;
            float forceZ = 0;

            for (auto& oscillator: oscillators){
                float d = vcl::norm(pos-oscillator.getPosition());
                vcl::vec3 force = q*oscillator.getCharge()/(pow(d,3))*(oscillator.getPosition()-pos);

                forceZ += force.z;
            }

            waterVerticalSpeeds[u][v] += forceZ/m;

            if (u > 0 && v > 0 && u < uDimensionSize-1 && v < vDimensionSize-1){
                float north = waterPositions[u][v+1].z;
                float south = waterPositions[u][v-1].z;
                float east = waterPositions[u+1][v].z;
                float west = waterPositions[u-1][v].z;

                waterVerticalSpeeds[u][v] += 0.002*((north+south+east+west)/4-waterPositions[u][v].z);
            }

            waterVerticalSpeeds[u][v] *= 0.999;

            waterPositions[u][v].z += waterVerticalSpeeds[u][v];
        }
    }
}


