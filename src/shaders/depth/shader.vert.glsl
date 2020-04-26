#version 330 core

layout (location = 0) in vec4 position;

uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0, 0.0, 0.0,                          // user defined rotation
                             0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0);
uniform float scaling = 1.0;                                           // user defined scaling
uniform vec3 scaling_axis = vec3(1.0, 1.0, 1.0);                       // user defined scaling

uniform mat4 light_matrix;

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

    vec4 position_transformed = R * S * position + T;
    gl_Position = light_matrix * position_transformed;
}
