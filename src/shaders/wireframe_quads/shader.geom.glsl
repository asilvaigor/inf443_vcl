
#version 330

layout (triangles) in;
layout (line_strip, max_vertices=3) out;

in struct vertex_data
{
    vec4 position;
    vec4 normal;
} vertex[];



// view transform
uniform mat4 perspective_view;


void main(void)
{

    gl_Position = perspective_view*vertex[0].position;
    EmitVertex();
    gl_Position = perspective_view*vertex[1].position;
    EmitVertex();
    gl_Position = perspective_view*vertex[2].position;
    EmitVertex();

    EndPrimitive();
}
