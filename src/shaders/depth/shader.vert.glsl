#version 330 core

layout (location = 0) in vec4 position;
layout (location = 3) in vec2 texture_uv;

out struct fragment_data
{
    vec2 texture_uv;
} fragment;

uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0, 0.0, 0.0,                          // user defined rotation
                             0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0);
uniform float scaling = 1.0;                                           // user defined scaling
uniform vec3 scaling_axis = vec3(1.0, 1.0, 1.0);                       // user defined scaling

uniform int current_light;
uniform mat4 light_matrix;
uniform mat4 light_matrix_2;
uniform mat4 light_matrix_3;

void main()
{
    // scaling matrix
    mat4 S = mat4(scaling * scaling_axis.x, 0.0, 0.0, 0.0,
                  0.0, scaling * scaling_axis.y, 0.0, 0.0,
                  0.0, 0.0, scaling * scaling_axis.z, 0.0,
                  0.0, 0.0, 0.0, 1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation, 0.0);

    fragment.texture_uv = texture_uv;

    vec4 position_transformed = R * S * position + T;
    switch (current_light) {
        case 1:
            gl_Position = light_matrix * position_transformed;
            break;
        case 2:
            gl_Position = light_matrix_2 * position_transformed;
            break;
        default:
            gl_Position = light_matrix_3 * position_transformed;
            break;
    }
}
