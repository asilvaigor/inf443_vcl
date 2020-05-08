#version 330 core

in struct fragment_data
{
    vec2 texture_uv;
} fragment;

uniform sampler2D texture_sampler;

void main()
{
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    if (color_texture.w < 0.5)
        discard;
}
