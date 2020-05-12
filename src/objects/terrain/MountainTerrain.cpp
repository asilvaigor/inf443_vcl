//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "MountainTerrain.h"

MountainTerrain::MountainTerrain(Shaders &shaders, float xSize, float ySize) {
    // Initializing internal variables
    light2 = std::make_shared<vcl::light_source>();
    light3 = std::make_shared<vcl::light_source>();
    this->xSize = xSize;
    this->ySize = ySize;

    // Creating mesh
    evaluate_mesh();

    // Setting bounding sphere
    boundingSphere = BoundingSphere(terrainMesh);

    //Setting shader
    terrain.shader = shaders["terrain_mesh"];
}

void MountainTerrain::drawMesh(vcl::camera_scene &camera) {
    terrain.uniform.light = light;
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

float MountainTerrain::evaluate_base_terrain_outline(float u, float v) {
    const float d = norm(vcl::vec2(u,v)-vcl::vec2(0.2f, 0.2f))/0.15f;
    float z = -1*std::exp(-d*d);
    return z;
}

float MountainTerrain::evaluate_terrain_z(const float u, const float v) {
    vcl::NoiseParameters parameters;
    parameters.g = 0.6;

    // Adding slope erosion
    float eps = 0.00001;
    if (u > eps && u < 1-eps && v > eps && v < 1-eps){
        // Slope erosion
        float du = (evaluate_terrain_z_no_erosion(u + eps, v, parameters) - evaluate_terrain_z_no_erosion(u - eps, v,
                                                                                           parameters)) / (2 * eps);
        float dv = (evaluate_terrain_z_no_erosion(u, v + eps, parameters) - evaluate_terrain_z_no_erosion(u, v - eps,
                                                                                           parameters)) / (2 * eps);
        vcl::vec2 grad = {du, dv};
        float slopeErosion = 1/(1+vcl::dot(grad, grad));


        //Concavity erosion
        float d2u = (evaluate_terrain_z_no_erosion(u + eps, v, parameters) + evaluate_terrain_z_no_erosion(u - eps, v,
                                                                                             parameters) - 2 *
                                                                                                                evaluate_terrain_z_no_erosion(
                                                                                                                     u,
                                                                                                                     v,
                                                                                                                     parameters)) / (eps * eps);
        float d2v = (evaluate_terrain_z_no_erosion(u, v + eps, parameters) + evaluate_terrain_z_no_erosion(u, v - eps,
                                                                                             parameters) - 2 *
                                                                                                                evaluate_terrain_z_no_erosion(
                                                                                                                     u,
                                                                                                                     v,
                                                                                                                     parameters)) / (eps * eps);
        float C = (d2u+d2v)*0.5f;

//        std::cout << C << std::endl;

//        parameters.g = parameters.g+0.005*(parameters.g/(1+fabs(std::min(C, (float) 0.0)))-parameters.g);

//        return evaluate_terrain_z_no_erosion(u, v, parameters)+slopeErosion;
        return evaluate_terrain_z_no_erosion(u, v, parameters);
    } else return evaluate_terrain_z_no_erosion(u, v, parameters);
}

float MountainTerrain::evaluate_terrain_z_no_erosion(float u, float v, vcl::NoiseParameters &parameters) {
    float z = 0.0f;

    // Adding base terrain contribution
    z += evaluate_base_terrain_outline(u, v);

    // Adding perlin noise
//    vcl::NoiseParameters parameters;
    parameters.Ss = -0.5;
    parameters.octaves = 20;
    parameters.Ia = 0.2;
    parameters.If = 3;
//    parameters.g = 0.6;
    parameters.Sh = 10;
    parameters.Ss = 2;
    parameters.Ea = 2;
    parameters.sigma = 0.9;

    z += (float)(noiseGenerator.erosionFbmNoise(u, v, parameters));
//    z += (float)(noiseGenerator.fbmNoise(u, v, parameters));
    z = noiseGenerator.heightDependentNoise(z, parameters);

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
