//#define  FREEGLUT_LIB_PRAGMAS  0

#pragma warning(push)

// Convert void* to long
#pragma warning(disable:4311)

// Convert long to void*
#pragma warning(disable:4312)

#include <iostream>
#include "GL/glew.h"
#include <string>
#include <stack>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#pragma warning(pop)

#include "Loader.h"

#include "Model.h"
#include "Viewer.h"

#include "Light.h"
#include "SpotLight.h"

#include "Cube.h"
#include "Sphere.h"
#include "Teapot.h"
#include "Mesh.h"
#include "Plane.h"

class MyGlWindow {
public:
	Viewer *m_viewer;

	MyGlWindow() = default;
	~MyGlWindow() = default;

	MyGlWindow(int w, int h);

	void setSize(int w, int h);
	void setAspect(float r);
	void setAngle(float angle);

	void draw();

private:
	int _m_width;
	int _m_height;

	float _angle;

	glm::vec3 _eye;
	glm::vec3 _look;
	glm::vec3 _up;

	glm::mat4 _model;
	glm::mat4 _view;
	glm::mat4 _mview;
	glm::mat3 _nmat;
	glm::mat4 _imvp;
	glm::mat4 _projection;
	glm::mat4 _mvp;

	Plane *_m_plane;
	Cube *_m_cube;
	Sphere *_m_sphere;
	Teapot *_m_teapot;
	Mesh *_m_mesh;

	Model _m_model;

	std::vector<Light> _lights;
	SpotLight *_spotLight;

	void setupViewer();

	void initializeData(int w, int h);

	void initLights(ShaderProgram *shader);
	void calculateSpotLight();

	void drawShader(ShaderProgram *shader);

	void calculateMVP();
	void calculateNmat();
};