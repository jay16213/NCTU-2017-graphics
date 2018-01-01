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
                v01.position = normalize(0.5*p0 + 0.5*p1) * radius + c;
                v01.normal = normalize(v0.normal + v1.normal);
                v01.vertexEyeSpace = v01.position.xyz;
                v01._texcoord = 0.5*v0._texcoord + 0.5*v1._texcoord;

                //v12
                v12.position = normalize(0.5*p1 + 0.5*p2) * radius + c;
                v12.normal = normalize(v1.normal + v2.normal);
                v12.vertexEyeSpace = v12.position.xyz;
                v12._texcoord = 0.5*v1._texcoord + 0.5*v2._texcoord;

                //v02
                v02.position = normalize(0.5*p0 + 0.5*p2) * radius + c;
                v02.normal = normalize(v0.normal + v2.normal);
                v02.vertexEyeSpace = v02.position.xyz;
                v02._texcoord = 0.5*v0._texcoord + 0.5*v2._texcoord;
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
            for(int i = 0; i < gl_in.length(); i++)
            {
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
                //--------------------------------------------
                struct Vert v000, v001, v002;
                struct Vert v010, v011, v012;
                struct Vert v020, v021, v022;
                struct Vert v120, v121, v122;

                v010.position = normalize(0.75*p0 + 0.25*p1) * radius + c;
                v010.normal = normalize(0.75*v0.normal + 0.25*v1.normal);
                v010.vertexEyeSpace = v010.position.xyz;
                v010._texcoord = 0.75*v0._texcoord + 0.25*v1._texcoord;

                v011.position = normalize(0.50*p0 + 0.50*p1) * radius + c;
                v011.normal = normalize(0.50*v0.normal + 0.50*v1.normal);
                v011.vertexEyeSpace = v011.position.xyz;
                v011._texcoord = 0.50*v0._texcoord + 0.50*v1._texcoord;

                v012.position = normalize(0.25*p0 + 0.75*p1) * radius + c;
                v012.normal = normalize(0.25*v0.normal + 0.75*v1.normal);
                v012.vertexEyeSpace = v012.position.xyz;
                v012._texcoord = 0.25*v0._texcoord + 0.75*v1._texcoord;


                v120.position = normalize(0.75*p1 + 0.25*p2) * radius + c;
                v120.normal = normalize(0.75*v1.normal + 0.25*v2.normal);
                v120.vertexEyeSpace = v120.position.xyz;
                v120._texcoord = 0.75*v1._texcoord + 0.25*v2._texcoord;

                v121.position = normalize(0.50*p1 + 0.50*p2) * radius + c;
                v121.normal = normalize(0.50*v1.normal + 0.50*v2.normal);
                v121.vertexEyeSpace = v121.position.xyz;
                v121._texcoord = 0.50*v1._texcoord + 0.50*v2._texcoord;

                v122.position = normalize(0.25*p1 + 0.75*p2) * radius + c;
                v122.normal = normalize(0.25*v1.normal + 0.75*v2.normal);
                v122.vertexEyeSpace = v122.position.xyz;
                v122._texcoord = 0.25*v1._texcoord + 0.75*v2._texcoord;


                v020.position = normalize(0.75*p0 + 0.25*p2) * radius + c;
                v020.normal = normalize(0.75*v0.normal + 0.25*v2.normal);
                v020.vertexEyeSpace = v020.position.xyz;
                v020._texcoord = 0.75*v0._texcoord + 0.25*v2._texcoord;

                v021.position = normalize(0.50*p0 + 0.50*p2) * radius + c;
                v021.normal = normalize(0.50*v0.normal + 0.50*v2.normal);
                v021.vertexEyeSpace = v021.position.xyz;
                v021._texcoord = 0.50*v0._texcoord + 0.50*v2._texcoord;

                v022.position = normalize(0.25*p0 + 0.75*p2) * radius + c;
                v022.normal = normalize(0.25*v0.normal + 0.75*v2.normal);
                v022.vertexEyeSpace = v022.position.xyz;
                v022._texcoord = 0.25*v0._texcoord + 0.75*v2._texcoord;


                vec4 p010 = v010.position - c;
                vec4 p122 = v122.position - c;
                vec4 p011 = v010.position - c;
                vec4 p121 = v121.position - c;

                v000.position = normalize((2.0/3.0)*p010 + (1.0/3.0)*p122) * radius + c;
                v000.normal = normalize((2.0/3.0)*v010.normal + (1.0/3.0)*v122.normal);
                v000.vertexEyeSpace = v000.position.xyz;
                v000._texcoord = (2.0/3.0)*v010._texcoord + (1.0/3.0)*v122._texcoord;

                v002.position = normalize((1.0/3.0)*p010 + (2.0/3.0)*p122) * radius + c;
                v002.normal = normalize((1.0/3.0)*v010.normal + (2.0/3.0)*v122.normal);
                v002.vertexEyeSpace = v002.position.xyz;
                v002._texcoord = (1.0/3.0)*v010._texcoord + (2.0/3.0)*v122._texcoord;

                v001.position = normalize(0.50*p011 + 0.50*p121) * radius + c;
                v001.normal = normalize(0.50*v011.normal + 0.50*v121.normal);
                v001.vertexEyeSpace = v001.position.xyz;
                v001._texcoord = 0.50*v011._texcoord + 0.50*v121._texcoord;

                // output
                outputVert(v022);
                outputVert(v2);
                outputVert(v122);
                EndPrimitive();

                outputVert(v021);
                outputVert(v022);
                outputVert(v002);
                outputVert(v122);
                outputVert(v121);
                EndPrimitive();

                outputVert(v020);
                outputVert(v021);
                outputVert(v000);
                outputVert(v002);
                outputVert(v001);
                outputVert(v121);
                outputVert(v120);
                EndPrimitive();

                outputVert(v0);
                outputVert(v020);
                outputVert(v010);
                outputVert(v000);
                outputVert(v011);
                outputVert(v001);
                outputVert(v012);
                outputVert(v120);
                outputVert(v1);
                EndPrimitive();
            }
            break;
        }
    }

    return;
}
