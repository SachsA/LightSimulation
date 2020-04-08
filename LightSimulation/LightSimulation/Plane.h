#include "GL/glew.h"

#include "Loader.h"

class Plane
{
public:
	ShaderProgram *shader;

	Plane() = default;
	Plane(int size, int nsquare, float yPos);
	~Plane() = default;

	void draw();

private:
	GLuint _vaoHandle, _vbo_plane_vertices, _vbo_plane_normal;

	glm::vec3 _Kd;
	glm::vec3 _Ka;
	glm::vec3 _Ks;
	float _shiness;

	int _plane_size;
	int _nsquare;

	int _nb_plane_vertices;

	float _yPos;

	std::vector<glm::vec3> _plane_vertices;
	std::vector<glm::vec3> _plane_colors;

	void setupPlane();
	void setupGl();
	void initShader();
};

