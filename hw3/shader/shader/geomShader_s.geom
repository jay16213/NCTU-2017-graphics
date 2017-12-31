#version 150 compatibility
/*
      v2
     /  \
    /    \
  v02----v12
  /  \  /  \
 /    \/    \
v0----v01---v1

              v2
            /    \
           /      \
         v022----v122
         /  \    /  \
        /    \  /    \
      v021---v002---v121
      /  \   /  \   /  \
     /    \ /    \ /    \
   v020---v000--v001---v120
   /  \   / \   /  \   /  \
  /    \ /   \ /    \ /    \
v0----v010---v011---v012----v1
*/
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
    vertexEyeSpace = v.vertexEyeSpace;
    normal = v.normal;
    tex = v._texcoord;
    gl_Position = gl_ProjectionMatrix * v.position;
    EmitVertex();
    return;
}

void main()
{
    vec4 c = gl_ModelViewMatrix * Centroid;
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
                struct Vert v0, v1, v2;
                vec4 p0, p1, p2;
                v0.vertexEyeSpace = vertex[i].vertexEyeSpace;
                v0.normal = vertex[i].normal;
                v0._texcoord = vertex[i]._texcoord;
                v0.position = gl_in[i].gl_Position;

                v1.vertexEyeSpace = vertex[i + 1].vertexEyeSpace;
                v1.normal = vertex[i + 1].normal;
                v1._texcoord = vertex[i + 1]._texcoord;
                v1.position = gl_in[i + 1].gl_Position;

                v2.vertexEyeSpace = vertex[i + 2].vertexEyeSpace;
                v2.normal = vertex[i + 2].normal;
                v2._texcoord = vertex[i + 2]._texcoord;
                v2.position = gl_in[i + 2].gl_Position;

                p0 = v0.position - c;
                p1 = v1.position - c;
                p2 = v2.position - c;
                //----------------------------------------------------
                struct Vert v01, v12, v02;

                //v01
                v01.position = normalize(p0 + p1) * radius + c;
                v01.normal = normalize(v01.position).xyz;
                v01.vertexEyeSpace = normalize((v0.vertexEyeSpace + v1.vertexEyeSpace) - c.xyz) * radius;
                v01._texcoord = (v0._texcoord + v1._texcoord) / 2.0;

                //v12
                v12.position = normalize(p1 + p2) * radius + c;
                v12.normal = normalize(v12.position).xyz;
                v12.vertexEyeSpace = normalize((v1.vertexEyeSpace + v2.vertexEyeSpace) - c.xyz) * radius;
                v12._texcoord = (v1._texcoord + v2._texcoord) / 2.0;

                //v02
                v02.position = normalize(p0 + p2) * radius + c;
                v02.normal = normalize(v02.position).xyz;
                v02.vertexEyeSpace = normalize((v0.vertexEyeSpace + v2.vertexEyeSpace) - c.xyz) * radius;
                v02._texcoord = (v0._texcoord + v2._texcoord) / 2.0;
                //-------------------------------------------------------------------

                outputVert(v02);
                outputVert(v2);
                outputVert(v12);
                EndPrimitive();

                outputVert(v0);
                outputVert(v02);
                outputVert(v01);
                outputVert(v12);
                outputVert(v1);
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
