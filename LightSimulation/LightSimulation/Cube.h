#include <iostream>
#include <vector>

#include "GL/glew.h"

#include "Loader.h"

class Cube
{
public:
	ShaderProgram *shader;

	Cube();
	~Cube() = default;

	void draw();

private:
	GLuint _vaoHandle;
	GLuint _vbo_cube_vertices, _vbo_cube_normals;

	std::vector<glm::vec4> _cube_vertices;
	std::vector<glm::vec3> _cube_normals;

	glm::vec3 _Kd;
	glm::vec3 _Ka;
	glm::vec3 _Ks;
	float _shiness;

	void setupVertices();

	void initShader();
	void setupGl();

	glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};