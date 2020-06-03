
#version 330

layout (triangles) in;
layout (line_strip, max_vertices=9) out;

in struct vertex_data
{
    vec4 position;
    vec4 normal;
} vertex[];



// view transform
uniform mat4 perspective_view;


void main(void)
{
    const float L = 0.03;

    gl_Position = perspective_view*vertex[0].position;
    EmitVertex();
    gl_Position = perspective_view*(vertex[0].position + vec4(L*vertex[0].normal.xyz,0));
    EmitVertex();
    gl_Position = perspective_view*vertex[0].position ;
    EmitVertex();

    EndPrimitive();
}
