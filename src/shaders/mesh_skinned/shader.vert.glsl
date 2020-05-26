#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texture_uv;
layout (location = 4) in ivec4 bone_ids;
layout (location = 5) in vec4 bone_weights;

out struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
    vec4 light_ref_pos;
} fragment;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

// model transformation
uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0); // user defined rotation
uniform float scaling = 1.0;                                         // user defined scaling
uniform vec3 scaling_axis = vec3(1.0,1.0,1.0);                       // user defined scaling


// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;

uniform mat4 light_matrix;



void main()
{
    mat4 bone_transform = bones[bone_ids[0]] * bone_weights[0];
    bone_transform     += bones[bone_ids[1]] * bone_weights[1];
    bone_transform     += bones[bone_ids[2]] * bone_weights[2];
    bone_transform     += bones[bone_ids[3]] * bone_weights[3];

    vec4 true_normal = normal; //bone_transform * normal;
    vec4 true_pos = position; //bone_transform * position;

    // scaling matrix
    mat4 S = mat4(scaling*scaling_axis.x,0.0,0.0,0.0, 0.0,scaling*scaling_axis.y,0.0,0.0, 0.0,0.0,scaling*scaling_axis.z,0.0, 0.0,0.0,0.0,1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation,0.0);

    fragment.color = color;
    fragment.texture_uv = texture_uv;

    fragment.normal = R*true_normal;
    vec4 position_transformed = R*S*true_pos + T;

    fragment.light_ref_pos = light_matrix * position_transformed;
    fragment.position = position_transformed;
    gl_Position = perspective * view * position_transformed;
}
