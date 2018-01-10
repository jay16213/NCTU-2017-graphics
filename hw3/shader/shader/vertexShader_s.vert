#version 150 compatibility

out Vertex {
    vec3 vertexEyeSpace;
    vec3 normal;
    vec4 _texcoord;
};

void main()
{
    vertexEyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    _texcoord = gl_TexCoord[0];
    gl_Position = gl_ModelViewMatrix * gl_Vertex;
}
