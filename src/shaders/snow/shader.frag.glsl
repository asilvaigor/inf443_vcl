#version 330 core

in struct fragment_data
{
    vec3 color;
} fragment;

out vec4 FragColor;


void main()
{
    FragColor = vec4(fragment.color, 1);
}
