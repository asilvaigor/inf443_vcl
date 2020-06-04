#version 330 core

in struct fragment_data
{
    vec3 color;
    vec2 texture_uv;
} fragment;

out vec4 FragColor;

uniform sampler2D texture_sampler;

void main()
{
    FragColor = vec4(fragment.color, texture(texture_sampler, fragment.texture_uv)[3]);
}
