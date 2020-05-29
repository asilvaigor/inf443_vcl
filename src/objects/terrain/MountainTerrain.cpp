//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "MountainTerrain.h"
#include <cmath>

MountainTerrain::MountainTerrain(Shaders &shaders, float xSize, float ySize) {
    // Initializing internal variables
    light2 = std::make_shared<vcl::light_source>();
    light3 = std::make_shared<vcl::light_source>();
    this->xSize = xSize;
    this->ySize = ySize;

    // Initializing texture
    terrainTexture = std::make_shared<Texture>("terrain");

    // TODO remove all this parameter initialization
    parameters.Ss = -0.5;
    parameters.octaves = 20;
    parameters.Ia = 0.2;
    parameters.If = 3;
    parameters.g = 0.6;
    parameters.Sh = 10;
    parameters.Ss = 2;
    parameters.Ea = 2;
    parameters.sigma = 0.9;

    // TODO remove this mountain
    mountainHeight = 2.0f;
    addMountain({0.2f, 0.2f}, 0.3f, mountainHeight);

    // TODO remove this lake
    lakePos = {0.5f, 0.7f};
    lakeSig = 0.25f;
    addHighLevelFeature(lakePos, lakeSig, -30.0f);

    // Creating mesh
    evaluate_mesh();

    // Setting bounding sphere
    boundingSphere = BoundingSphere(terrainMesh);

    //Setting shader
    terrain.shader = shaders["terrain_mesh"];
}

void MountainTerrain::drawMesh(vcl::camera_scene &camera) {
    terrain.uniform.light = light;
    terrainTexture->bind();
    terrain.uniform.light2 = light2;
    terrain.uniform.light3 = light3;
    vcl::draw(terrain, camera);
}

void MountainTerrain::setLight(std::shared_ptr<vcl::light_source> &light, int idx) {
    switch (idx) {
        case 1:
            this->light = light;
            break;
        case 2:
            this->light2 = light;
            break;
        default:
            this->light3 = light;
            break;
    }
    terrain.uniform.current_light = idx;
}

float MountainTerrain::evaluate_terrain_z(const float u, const float v) {
    float z = 0.0f;

    z += altitudeErosionFbmNoise(u, v);
    z = noiseGenerator.heightDependentNoise(z, parameters);

    // Adding high level features
    for (auto & feature: features){
        if (!feature.isLowLevel())
            z+= feature.evaluate(u, v);
    }

    return z;
}

vcl::vec3 MountainTerrain::evaluate_terrain(const float u, const float v){
    const float x = xSize*(u-0.5f);
    const float y = ySize*(v-0.5f);
    const float z = evaluate_terrain_z(u, v);

    return {x, y, z};
}

void MountainTerrain::evaluate_mesh() {
    // TODO change to variable size
    size_t uDimensionSize = 200;
    size_t vDimensionSize = 200;

    terrainMesh.position.resize(uDimensionSize*vDimensionSize);
    terrainMesh.texture_uv.resize(uDimensionSize*vDimensionSize);

    // Fill terrain geometry
    for(size_t ku=0; ku<uDimensionSize; ++ku)
    {
        for(size_t kv=0; kv<vDimensionSize; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/((float)uDimensionSize-1.0f);
            const float v = kv/((float)vDimensionSize-1.0f);

            // Compute coordinates
            terrainMesh.position[kv+uDimensionSize*ku] = evaluate_terrain(u, v);

            // Compute texture coordinates
            terrainMesh.texture_uv[kv+uDimensionSize*ku] = {u, v};
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

            terrainMesh.connectivity.push_back(triangle_1);
            terrainMesh.connectivity.push_back(triangle_2);
        }
    }

    terrain = vcl::mesh_drawable(terrainMesh);
}

float MountainTerrain::altitudeErosionFbmNoise(double x, double y) {
    float val = 0;
    std::vector<float> gains(parameters.octaves);
    for (int i = 0; i < parameters.octaves; ++i){
        if (i == 0) gains[i] = parameters.Ia;
        else {
            gains[i] = gains[i-1]*vcl::lerp(parameters.g, parameters.sigma*parameters.g, parameters.Ea);
        }
    }

    for (int k = 0; k < parameters.octaves; ++k){
        float freq = parameters.If*pow(parameters.l, k);
        val += gains[k]*noiseGenerator.sharpnessAdjustedNoise(x*freq, y*freq, parameters.Ss);

        // Adding low frequency features
        if (k == 0){
            for (auto &feature : features){
                if (feature.isLowLevel())
                    val += feature.evaluate(x, y);
            }
        }
    }

    return val;
}

void MountainTerrain::addMountain(vcl::vec2 pos, float sig, float height) {
    features.emplace_back(pos, sig, height, true);
}

void MountainTerrain::addHighLevelFeature(vcl::vec2 pos, float sig, float height) {
    features.emplace_back(pos, sig, height, false);
}

float MountainTerrain::getTerrainHeight(float x, float y) {
    return evaluate_terrain_z(x / xSize + 0.5f, y / ySize + 0.5f);
}

float MountainTerrain::getMaxTerrainHeight() {
    return mountainHeight;
}

bool MountainTerrain::isInsideLake(float x, float y) {
    // FIXME: This isn't working
    x = x / xSize - lakePos[0];
    y = y / ySize - lakePos[1];
    return x * x + y * y < 3 * lakeSig;
}

float &MountainTerrain::getXSize() {
    return xSize;
}

float &MountainTerrain::getYSize() {
    return ySize;
}


