#ifndef TEAPOT_H
#define TEAPOT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "Loader.h"

using glm::vec4;
using glm::vec3;
using glm::mat3;
using glm::mat4;

class Teapot
{
public:
	ShaderProgram *shader;

	Teapot() = default;
	Teapot(int grid, mat4 lidTransform);
	~Teapot() = default;

	void draw();

private:
	GLuint _vaoHandle, _vbo_teapot_vertices, _vbo_teapot_normals, _ibo_teapot;
	GLuint _teapot_vertices, _slices, _faces;

	glm::vec3 _Kd;
	glm::vec3 _Ka;
	glm::vec3 _Ks;
	float _shiness;

	void initShader();

	void generatePatches(float * v, float * n, float *tc, unsigned int* el, int grid);
	void buildPatchReflect(int patchNum,
		float *B, float *dB,
		float *v, float *n, float *, unsigned int *el,
		int &index, int &elIndex, int &, int grid,
		bool reflectX, bool reflectY);
	void buildPatch(vec3 patch[][4],
		float *B, float *dB,
		float *v, float *n, float *, unsigned int *el,
		int &index, int &elIndex, int &, int grid, mat3 reflect, bool invertNormal);
	void getPatch(int patchNum, vec3 patch[][4], bool reverseV);

	void computeBasisFunctions(float * B, float * dB, int grid);
	vec3 evaluate(int gridU, int gridV, float *B, vec3 patch[][4]);
	vec3 evaluateNormal(int gridU, int gridV, float *B, float *dB, vec3 patch[][4]);
	void moveLid(int, float *, mat4);
};

#endif // TEAPOT_H