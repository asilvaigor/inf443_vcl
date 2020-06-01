#version 330 core

layout (location = 0) in vec4 position;
layout (location = 4) in ivec4 bone_ids;
layout (location = 5) in vec4 bone_weights;

out struct vertex_data
{
    vec4 position;
} vertex;

// model transformation
uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0); // user defined rotation
uniform float scaling = 1.0;                                         // user defined scaling
uniform vec3 scaling_axis = vec3(1.0,1.0,1.0);                       // user defined scaling

uniform bool is_skinned;
uniform mat4 bones[50];

void main()
{
    vec4 true_pos;
    if (is_skinned) {
        mat4 bone_transform = bones[bone_ids[0]] * bone_weights[0];
        bone_transform     += bones[bone_ids[1]] * bone_weights[1];
        bone_transform     += bones[bone_ids[2]] * bone_weights[2];
        bone_transform     += bones[bone_ids[3]] * bone_weights[3];

        true_pos = bone_transform * position;
    } else {
        true_pos = position;
    }

    // scaling matrix
    mat4 S = mat4(scaling*scaling_axis.x,0.0,0.0,0.0, 0.0,scaling*scaling_axis.y,0.0,0.0, 0.0,0.0,scaling*scaling_axis.z,0.0, 0.0,0.0,0.0,1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation,0.0);

    vertex.position = R*S*true_pos+T;
    gl_Position = vertex.position;
}
