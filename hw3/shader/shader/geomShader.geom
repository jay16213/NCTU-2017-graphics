#version 150 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices=85) out;

in Vertex {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec2 _texcoord;
} vertex[];

out vec3 vertexEyeSpace;
out vec3 normal;
out vec2 tex;
out vec4 gl_Position;

void main()
{

}