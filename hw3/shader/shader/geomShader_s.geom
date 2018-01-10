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
             /  \
            /    \
         v022----v122
          /  \  /  \
         /    \/    \
      v021---v002---v121
       /  \  /  \  /  \
      /    \/    \/    \
   v020---v000--v001---v120
    /  \  /  \  /  \  /  \
   /    \/    \/    \/    \
 v0---v010---v011---v012---v1
*/
layout(triangles) in;
layout(triangle_strip, max_vertices=85) out;

uniform int level;
uniform float radius;
uniform vec4 Centroid;

struct Vert {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec4 _texcoord;
    vec4 position;
};

in Vertex {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec4 _texcoord;
} vertex[];

out vec3 vertexEyeSpace;
out vec3 normal;
out vec2 tex;

void outputVert(struct Vert v)
{
    vertexEyeSpace = v.vertexEyeSpace;
    normal = v.normal;
    tex = v._texcoord.xy;
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
                tex = vertex[i]._texcoord.xy;
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
                struct Vert v[3];
                vec4 p0, p1, p2;

                for(int j = 0; j < 3; j++)
                {
                    v[j].vertexEyeSpace = vertex[i + j].vertexEyeSpace;
                    v[j].normal = vertex[i + j].normal;
                    v[j]._texcoord = vertex[i + j]._texcoord;
                    v[j].position = normalize(gl_in[i + j].gl_Position - c) * radius + c;
                }

                p0 = v[0].position - c;
                p1 = v[1].position - c;
                p2 = v[2].position - c;

                vec3 pe0, pe1, pe2;
                pe0 = v[0].vertexEyeSpace - c.xyz;
                pe1 = v[1].vertexEyeSpace - c.xyz;
                pe2 = v[2].vertexEyeSpace - c.xyz;
                //----------------------------------------------------
                struct Vert v01, v12, v02;

                //v01
                v01.position = normalize(0.5*p0 + 0.5*p1) * radius + c;
                v01.normal = normalize(v[0].normal + v[1].normal);
                v01.vertexEyeSpace = normalize(pe0 + pe1) * radius + c.xyz;
                v01._texcoord = 0.5*v[0]._texcoord + 0.5*v[1]._texcoord;

                //v12
                v12.position = normalize(0.5*p1 + 0.5*p2) * radius + c;
                v12.normal = normalize(v[1].normal + v[2].normal);
                v12.vertexEyeSpace = normalize(pe1 + pe2) * radius + c.xyz;
                v12._texcoord = 0.5*v[1]._texcoord + 0.5*v[2]._texcoord;

                //v02
                v02.position = normalize(0.5*p0 + 0.5*p2) * radius + c;
                v02.normal = normalize(v[0].normal + v[2].normal);
                v02.vertexEyeSpace = normalize(pe0 + pe2) * radius + c.xyz;
                v02._texcoord = 0.5*v[0]._texcoord + 0.5*v[2]._texcoord;
                //-------------------------------------------------------------------

                outputVert(v02);
                outputVert(v[2]);
                outputVert(v12);
                EndPrimitive();

                outputVert(v[0]);
                outputVert(v02);
                outputVert(v01);
                outputVert(v12);
                outputVert(v[1]);
                EndPrimitive();
            }
            break;
        }

        case 2:
        {
            for(int i = 0; i < gl_in.length(); i++)
            {
                struct Vert v[3];
                vec4 p0, p1, p2;

                for(int j = 0; j < 3; j++)
                {
                    v[j].vertexEyeSpace = vertex[i + j].vertexEyeSpace;
                    v[j].normal = vertex[i + j].normal;
                    v[j]._texcoord = vertex[i + j]._texcoord;
                    v[j].position = normalize(gl_in[i + j].gl_Position - c) * radius + c;
                }

                p0 = v[0].position - c;
                p1 = v[1].position - c;
                p2 = v[2].position - c;
                //--------------------------------------------
                struct Vert v000, v001, v002;
                struct Vert v010, v011, v012;
                struct Vert v020, v021, v022;
                struct Vert v120, v121, v122;

                v010.position = normalize(0.75*p0 + 0.25*p1) * radius + c;
                v010.normal = normalize(0.75*v[0].normal + 0.25*v[1].normal);
                v010.vertexEyeSpace = 0.75*v[0].vertexEyeSpace + 0.25*v[1].vertexEyeSpace;
                v010._texcoord = 0.75*v[0]._texcoord + 0.25*v[1]._texcoord;

                v011.position = normalize(0.50*p0 + 0.50*p1) * radius + c;
                v011.normal = normalize(0.50*v[0].normal + 0.50*v[1].normal);
                v011.vertexEyeSpace = 0.50*v[0].vertexEyeSpace + 0.50*v[1].vertexEyeSpace;
                v011._texcoord = 0.50*v[0]._texcoord + 0.50*v[1]._texcoord;

                v012.position = normalize(0.25*p0 + 0.75*p1) * radius + c;
                v012.normal = normalize(0.25*v[0].normal + 0.75*v[1].normal);
                v012.vertexEyeSpace = 0.25*v[0].vertexEyeSpace + 0.75*v[1].vertexEyeSpace;
                v012._texcoord = 0.25*v[0]._texcoord + 0.75*v[1]._texcoord;


                v120.position = normalize(0.75*p1 + 0.25*p2) * radius + c;
                v120.normal = normalize(0.75*v[1].normal + 0.25*v[2].normal);
                v120.vertexEyeSpace = 0.75*v[1].vertexEyeSpace + 0.25*v[2].vertexEyeSpace;
                v120._texcoord = 0.75*v[1]._texcoord + 0.25*v[2]._texcoord;

                v121.position = normalize(0.50*p1 + 0.50*p2) * radius + c;
                v121.normal = normalize(0.50*v[1].normal + 0.50*v[2].normal);
                v121.vertexEyeSpace = 0.50*v[1].vertexEyeSpace + 0.50*v[2].vertexEyeSpace;
                v121._texcoord = 0.50*v[1]._texcoord + 0.50*v[2]._texcoord;

                v122.position = normalize(0.25*p1 + 0.75*p2) * radius + c;
                v122.normal = normalize(0.25*v[1].normal + 0.75*v[2].normal);
                v122.vertexEyeSpace = 0.25*v[1].vertexEyeSpace + 0.75*v[2].vertexEyeSpace;
                v122._texcoord = 0.25*v[1]._texcoord + 0.75*v[2]._texcoord;


                v020.position = normalize(0.75*p0 + 0.25*p2) * radius + c;
                v020.normal = normalize(0.75*v[0].normal + 0.25*v[2].normal);
                v020.vertexEyeSpace = 0.75*v[0].vertexEyeSpace + 0.25*v[2].vertexEyeSpace;
                v020._texcoord = 0.75*v[0]._texcoord + 0.25*v[2]._texcoord;

                v021.position = normalize(0.50*p0 + 0.50*p2) * radius + c;
                v021.normal = normalize(0.50*v[0].normal + 0.50*v[2].normal);
                v021.vertexEyeSpace = 0.50*v[0].vertexEyeSpace + 0.50*v[2].vertexEyeSpace;
                v021._texcoord = 0.50*v[0]._texcoord + 0.50*v[2]._texcoord;

                v022.position = normalize(0.25*p0 + 0.75*p2) * radius + c;
                v022.normal = normalize(0.25*v[0].normal + 0.75*v[2].normal);
                v022.vertexEyeSpace = 0.25*v[0].vertexEyeSpace + 0.75*v[2].vertexEyeSpace;
                v022._texcoord = 0.25*v[0]._texcoord + 0.75*v[2]._texcoord;


                vec4 p010 = v010.position - c;
                vec4 p122 = v122.position - c;
                vec4 p011 = v010.position - c;
                vec4 p121 = v121.position - c;

                v000.position = normalize((2.0/3.0)*p010 + (1.0/3.0)*p122) * radius + c;
                v000.normal = normalize((2.0/3.0)*v010.normal + (1.0/3.0)*v122.normal);
                v000.vertexEyeSpace = (2.0/3.0)*v010.vertexEyeSpace + (1.0/3.0)*v122.vertexEyeSpace;
                v000._texcoord = (2.0/3.0)*v010._texcoord + (1.0/3.0)*v122._texcoord;

                v002.position = normalize((1.0/3.0)*p010 + (2.0/3.0)*p122) * radius + c;
                v002.normal = normalize((1.0/3.0)*v010.normal + (2.0/3.0)*v122.normal);
                v002.vertexEyeSpace = (1.0/3.0)*v010.vertexEyeSpace + (2.0/3.0)*v122.vertexEyeSpace;;
                v002._texcoord = (1.0/3.0)*v010._texcoord + (2.0/3.0)*v122._texcoord;

                v001.position = normalize(0.50*p011 + 0.50*p121) * radius + c;
                v001.normal = normalize(0.50*v011.normal + 0.50*v121.normal);
                v001.vertexEyeSpace = 0.50*v011.vertexEyeSpace + 0.50*v121.vertexEyeSpace;;
                v001._texcoord = 0.50*v011._texcoord + 0.50*v121._texcoord;

                // output
                outputVert(v022);
                outputVert(v[2]);
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

                outputVert(v[0]);
                outputVert(v020);
                outputVert(v010);
                outputVert(v000);
                outputVert(v011);
                outputVert(v001);
                outputVert(v012);
                outputVert(v120);
                outputVert(v[1]);
                EndPrimitive();
            }
            break;
        }
    }

    return;
}
