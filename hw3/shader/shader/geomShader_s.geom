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
                struct Vert v01[3];
                struct Vert v02[3];
                struct Vert v12[3];

                for(int j = 0; j < 3; j++)
                {
                    int ratio1 = 1 * (j + 1);
                    int ratio0 = 4 - ratio1;

                    v01[j].position = normalize(ratio0*p0 + ratio1*p1) * radius + c;
                    v01[j].normal = normalize(ratio0*v[0].normal + ratio1*v[1].normal);
                    v01[j].vertexEyeSpace = (ratio0*v[0].vertexEyeSpace + ratio1*v[1].vertexEyeSpace)*0.25;
                    v01[j]._texcoord = (ratio0*v[0]._texcoord + ratio1*v[1]._texcoord)*0.25;

                    v12[j].position = normalize(ratio0*p1 + ratio1*p2) * radius + c;
                    v12[j].normal = normalize(ratio0*v[1].normal + ratio1*v[2].normal);
                    v12[j].vertexEyeSpace = (ratio0*v[1].vertexEyeSpace + ratio1*v[2].vertexEyeSpace)*0.25;
                    v12[j]._texcoord = (ratio0*v[1]._texcoord + ratio1*v[2]._texcoord) * 0.25;

                    v02[j].position = normalize(ratio0*p0 + ratio1*p2) * radius + c;
                    v02[j].normal = normalize(ratio0*v[0].normal + ratio1*v[2].normal);
                    v02[j].vertexEyeSpace = (ratio0*v[0].vertexEyeSpace + ratio1*v[2].vertexEyeSpace)*0.25;
                    v02[j]._texcoord = (ratio0*v[0]._texcoord + ratio1*v[2]._texcoord)*0.25;
                }

                vec4 p010 = v01[0].position - c;
                vec4 p122 = v12[2].position - c;
                vec4 p011 = v01[0].position - c;
                vec4 p121 = v12[1].position - c;

                v000.position = normalize((2.0/3.0)*p010 + (1.0/3.0)*p122) * radius + c;
                v000.normal = normalize((2.0/3.0)*v01[0].normal + (1.0/3.0)*v12[2].normal);
                v000.vertexEyeSpace = (2.0/3.0)*v01[0].vertexEyeSpace + (1.0/3.0)*v12[2].vertexEyeSpace;
                v000._texcoord = (2.0/3.0)*v01[0]._texcoord + (1.0/3.0)*v12[2]._texcoord;

                v002.position = normalize((1.0/3.0)*p010 + (2.0/3.0)*p122) * radius + c;
                v002.normal = normalize((1.0/3.0)*v01[0].normal + (2.0/3.0)*v12[2].normal);
                v002.vertexEyeSpace = (1.0/3.0)*v01[0].vertexEyeSpace + (2.0/3.0)*v12[2].vertexEyeSpace;;
                v002._texcoord = (1.0/3.0)*v01[0]._texcoord + (2.0/3.0)*v12[2]._texcoord;

                v001.position = normalize(0.50*p011 + 0.50*p121) * radius + c;
                v001.normal = normalize(0.50*v01[1].normal + 0.50*v12[1].normal);
                v001.vertexEyeSpace = 0.50*v01[1].vertexEyeSpace + 0.50*v12[1].vertexEyeSpace;;
                v001._texcoord = 0.50*v01[1]._texcoord + 0.50*v12[1]._texcoord;

                // output
                outputVert(v02[2]);
                outputVert(v[2]);
                outputVert(v12[2]);
                EndPrimitive();

                outputVert(v02[1]);
                outputVert(v02[2]);
                outputVert(v002);
                outputVert(v12[2]);
                outputVert(v12[1]);
                EndPrimitive();

                outputVert(v02[0]);
                outputVert(v02[1]);
                outputVert(v000);
                outputVert(v002);
                outputVert(v001);
                outputVert(v12[1]);
                outputVert(v12[0]);
                EndPrimitive();

                outputVert(v[0]);
                outputVert(v02[0]);
                outputVert(v01[0]);
                outputVert(v000);
                outputVert(v01[1]);
                outputVert(v001);
                outputVert(v01[2]);
                outputVert(v12[0]);
                outputVert(v[1]);
                EndPrimitive();
            }
            break;
        }
    }

    return;
}
