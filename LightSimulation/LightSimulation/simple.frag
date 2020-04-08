#version 400

in vec3 f_color;

//Fragma Shader must have out variable (vec4)
out vec4 FragColors;

void main()
{
	FragColors = vec4(f_color, 1.0); // r g b a
}