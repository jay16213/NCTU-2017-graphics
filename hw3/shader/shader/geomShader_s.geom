#version 150 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices=85) out;

uniform int level;
uniform float radius;
uniform vec4 Centroid;

in Vertex {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec2 _texcoord;
} vertex[];

out vec3 vertexEyeSpace;
out vec3 normal;
out vec2 tex;
out vec4 gl_Position;

struct Vert {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec2 _texcoord;
    vec4 position;
};

void outputVert(struct Vert v)
{
    //vertexEyeSpace = v.vertexEyeSpace;
    //normal = v.normal;
    tex = v._texcoord;
    gl_Position = gl_ProjectionMatrix * (radius * v.position);
    EmitVertex();
    return;
}

void main()
{
    switch(level)
    {
        case 0:
        {
            for(int i = 0; i < gl_in.length(); i++)
            {
                vertexEyeSpace = vertex[i].vertexEyeSpace;
                normal = vertex[i].normal;
                tex = vertex[i]._texcoord;
                gl_Position = gl_ProjectionMatrix * gl_in[i].gl_Position;
                EmitVertex();
            }
            EndPrimitive();
            break;
        }

        case 1:
        {
            for(int i = 0; i < gl_in.length(); i++)
            {
                //init
                struct Vert v1, v2, v3;
                v1.vertexEyeSpace = vertex[i].vertexEyeSpace;
                v1.normal = vertex[i].normal;
                v1._texcoord = vertex[i]._texcoord;
                v1.position = gl_in[i].gl_Position;

                v2.vertexEyeSpace = vertex[i + 1].vertexEyeSpace;
                v2.normal = vertex[i + 1].normal;
                v2._texcoord = vertex[i + 1]._texcoord;
                v2.position = gl_in[i + 1].gl_Position;

                v3.vertexEyeSpace = vertex[i + 2].vertexEyeSpace;
                v3.normal = vertex[i + 2].normal;
                v3._texcoord = vertex[i + 2]._texcoord;
                v3.position = gl_in[i + 2].gl_Position;
                //----------------------------------------------------

                struct Vert v12, v23, v13;
                v12.vertexEyeSpace = 0.5f * (v1.vertexEyeSpace + v2.vertexEyeSpace);
                v12.vertexEyeSpace = normalize(v12.vertexEyeSpace);
                //v12.normal = normalize(0.5f * (v1.normal + v2.normal));
                v12._texcoord = 0.5f * (v1._texcoord + v2._texcoord);
                v12.position = 0.5f * (v1.position + v2.position);
                v12.normal = normalize(v12.position - Centroid).xyz;

                v23.vertexEyeSpace = 0.5f * (v2.vertexEyeSpace + v3.vertexEyeSpace);
                v23.vertexEyeSpace = normalize(v23.vertexEyeSpace);
                //v23.normal = normalize(0.5f * (v2.normal + v3.normal));
                v23._texcoord = 0.5f * (v2._texcoord + v3._texcoord);
                v23.position = 0.5f * (v2.position + v3.position);
                v23.normal = normalize(v23.position - Centroid).xyz;

                v13.vertexEyeSpace = 0.5f * (v1.vertexEyeSpace + v3.vertexEyeSpace);
                v13.vertexEyeSpace = normalize(v13.vertexEyeSpace);
                //v13.normal = normalize(0.5f * (v1.normal + v3.normal));
                v13._texcoord = 0.5f * (v1._texcoord + v3._texcoord);
                v13.position = 0.5f * (v1.position + v3.position);
                v13.normal = normalize(v13.position - Centroid).xyz;

                //-------------------------------------------------------------------

                outputVert(v12);
                outputVert(v2);
                outputVert(v23);
                EndPrimitive();

                outputVert(v1);
                outputVert(v12);
                outputVert(v13);
                outputVert(v23);
                outputVert(v3);
                EndPrimitive();
            }
            break;
        }

        case 2:
        {
            break;
        }
    }

    return;
}
