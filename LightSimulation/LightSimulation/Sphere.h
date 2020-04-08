#ifndef ___SPHERE_H
#define ___SPHERE_H

#include "GL/glew.h"
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"

class Sphere
{
public:
	ShaderProgram *shader;
	
	Sphere() = default;
	Sphere(float rad, GLuint sl, GLuint st);
	~Sphere() = default;

	void draw();
	int getVertexArrayHandle();

private:
	float _radius;

	GLuint _vaoHandle, _vbo_sphere_vertices, _vbo_sphere_normals, _ibo_sphere;
	GLuint _sphere_vertices, _sphere_elements, _slices, _stacks;

	glm::vec3 _Kd;
	glm::vec3 _Ka;
	glm::vec3 _Ks;
	float _shiness;

	void initShader();
	void generateVerts(float *, float *, float *, GLuint *);
};


#endif