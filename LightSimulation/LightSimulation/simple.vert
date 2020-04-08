#version 400 

//In-build variable for going to next stage of pipeline
//This variable sets the vertex positions (or vertex colors)
//It's required and must be a vec4 type (or vec3 for colors)

//Vertex positions
layout(location = 0) in vec4 coord3d;

//Vetex color
layout(location = 1) in vec3 v_color;

uniform mat4 mvp;

out vec3 f_color;
void main(void)
{
	gl_Position = mvp * coord3d;
	f_color = v_color;
}