#version 330 core

layout (location = 0) in vec4 position;
layout (location = 3) in vec2 texture_uv;

out struct fragment_data {
    vec3 color;
    vec2 texture_uv;
} fragment;

uniform mat4 perspective_view;
uniform vec3 camera_dir;
uniform vec3 translation = vec3(0.0, 0.0, 0.0);
uniform float scaling;
uniform vec3 color;

void main()
{
    vec3 proj = camera_dir * camera_dir.z; // dot({0, 0, 1}, dir)
    vec3 v1 = vec3(-proj.x, -proj.y, 1 - proj.z); // {0, 0, 1} - dir
    vec3 v2 = cross(camera_dir, v1);
    v1 = normalize(v1);
    v2 = normalize(v2);

    vec3 transformed_pos = scaling * position.x * v1 + scaling * position.y * v2 + translation;
    fragment.color = color;
    fragment.texture_uv = texture_uv;
    gl_Position = perspective_view * vec4(transformed_pos, 1);
}
