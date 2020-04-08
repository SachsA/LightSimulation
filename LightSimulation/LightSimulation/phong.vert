#version 440

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix; 
uniform mat4 mvp;

void main()
{
   Position = ModelViewMatrix * VertexPosition;

   Normal = normalize(NormalMatrix * VertexNormal);
   
   gl_Position = mvp * VertexPosition;
}
