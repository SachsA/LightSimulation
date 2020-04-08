#include <glm/gtc/type_ptr.hpp>

#include "MyGlWindow.h"
#include "MyGlUtiles.h"
#include "global.h"

static float DEFAULT_VIEW_POINT[3] = { 5, 10, -20 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 1, 5 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

MyGlWindow::MyGlWindow(int w, int h)
{
	setupViewer();
	initializeData(w, h);
}

void MyGlWindow::setupViewer()
{
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (float)(_m_width / _m_height);
	
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
}

void MyGlWindow::initializeData(int w, int h)
{
	_m_width = w;
	_m_height = h;

	float radius = 10;
	float angle = 0;
	glm::vec3 circlePosition(0, 20, 0);

	glm::vec3 intensities[5];
	intensities[0] = glm::vec3(0.0f, 0.8f, 0.8f);
	intensities[1] = glm::vec3(0.0f, 0.0f, 0.8f);
	intensities[2] = glm::vec3(0.8f, 0.0f, 0.0f);
	intensities[3] = glm::vec3(0.0f, 0.8f, 0.0f);
	intensities[4] = glm::vec3(0.8f, 0.8f, 0.8f);

	for (size_t i = 0; i < 5; i++)
	{
		_lights.emplace_back(glm::vec4(circlePosition.x + (radius * cos(glm::radians(angle))),
				circlePosition.y,
				circlePosition.z + (radius * sin(glm::radians(angle))), 1), intensities[i]);
		angle += 72;
	}

	_spotLight = new SpotLight(glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 10, 30, 30 * 0.7);

	//Init Plane
	_m_plane = new Plane(Global::floorSize.x, Global::floorSize.y, -0.51);

	//Init Cube
	_m_cube = new Cube();
	initLights(_m_cube->shader);

	_m_cube->shader->use();
	glUniform3fv(_m_cube->shader->uniform("SpotLight.LightIntensity"), 1, glm::value_ptr(_spotLight->lightIntensity));
	glUniform1f(_m_cube->shader->uniform("SpotLight.LightExponent"), _spotLight->lightExponent);
	glUniform1f(_m_cube->shader->uniform("SpotLight.LightCutoff"), _spotLight->lightCutoff);
	glUniform1f(_m_cube->shader->uniform("SpotLight.LightInnerCutoff"), _spotLight->lightInnerCutoff);
	_m_cube->shader->disable();

	//Init Sphere
	_m_sphere = new Sphere(1, 100, 100);
	initLights(_m_sphere->shader);

	_m_sphere->shader->use();
	glUniform3fv(_m_sphere->shader->uniform("SpotLight.LightIntensity"), 1, glm::value_ptr(_spotLight->lightIntensity));
	glUniform1f(_m_sphere->shader->uniform("SpotLight.LightExponent"), _spotLight->lightExponent);
	glUniform1f(_m_sphere->shader->uniform("SpotLight.LightCutoff"), _spotLight->lightCutoff);
	glUniform1f(_m_sphere->shader->uniform("SpotLight.LightInnerCutoff"), _spotLight->lightInnerCutoff);
	_m_sphere->shader->disable();

	//Init Teapot
	glm::mat4 m(1);
	_m_teapot = new Teapot(64, m);
	initLights(_m_teapot->shader);

	_m_teapot->shader->use();
	glUniform3fv(_m_teapot->shader->uniform("SpotLight.LightIntensity"), 1, glm::value_ptr(_spotLight->lightIntensity));
	glUniform1f(_m_teapot->shader->uniform("SpotLight.LightExponent"), _spotLight->lightExponent);
	glUniform1f(_m_teapot->shader->uniform("SpotLight.LightCutoff"), _spotLight->lightCutoff);
	glUniform1f(_m_teapot->shader->uniform("SpotLight.LightInnerCutoff"), _spotLight->lightInnerCutoff);
	_m_teapot->shader->disable();

	//Init Mesh
	_m_mesh = new Mesh("dragon.obj");
	initLights(_m_mesh->shader);

	_m_mesh->shader->use();
	glUniform3fv(_m_mesh->shader->uniform("SpotLight.LightIntensity"), 1, glm::value_ptr(_spotLight->lightIntensity));
	glUniform1f(_m_mesh->shader->uniform("SpotLight.LightExponent"), _spotLight->lightExponent);
	glUniform1f(_m_mesh->shader->uniform("SpotLight.LightCutoff"), _spotLight->lightCutoff);
	glUniform1f(_m_mesh->shader->uniform("SpotLight.LightInnerCutoff"), _spotLight->lightInnerCutoff);
	_m_mesh->shader->disable();
}

void MyGlWindow::initLights(ShaderProgram *shader)
{
	std::string nameLight = "Light[";

	shader->use();
	for (size_t i = 0; i < 5; i++)
	{
		glUniform4fv(shader->uniform(nameLight + std::to_string(i) + "].LightLocation"), 1, glm::value_ptr(_lights[i].lightPosition));
		glUniform3fv(shader->uniform(nameLight + std::to_string(i) + "].LightIntensity"), 1, glm::value_ptr(_lights[i].lightIntensity));
	}
	shader->disable();
}

void MyGlWindow::calculateSpotLight()
{
	glm::vec4 LightLookingAt(0, 0, 0, 1);

	static float rot = 0.0f;
	glm::vec3 lp = glm::vec3(cos(glm::radians(rot)) * 10, 15, sin(glm::radians(rot)) * 10);
	glm::vec4 lightPos = glm::vec4(lp, 1.0);
	rot += 2.0f;

	_spotLight->lightPosition = _view * lightPos;

	_spotLight->lightDirection = (_view * LightLookingAt) - _spotLight->lightPosition;
}

void MyGlWindow::setSize(int w, int h)
{
	_m_width = w;
	_m_height = h;
}

void MyGlWindow::setAspect(float r)
{
	m_viewer->setAspectRatio(r);
}

void MyGlWindow::setAngle(float angle)
{
	_angle = glm::degrees(angle);
}

void MyGlWindow::calculateMVP()
{
	_model = _m_model.getMatrix();

	_eye = m_viewer->getViewPoint();
	_look = m_viewer->getViewCenter();
	_up = m_viewer->getUpVector();
	_view = glm::lookAt(_eye, _look, _up);
	_projection = glm::perspective(45.0f, 1.0f * _m_width / _m_height, 0.1f, 500.0f);

	_mvp = _projection * _view * _model;
}

void MyGlWindow::calculateNmat()
{
	_model = _m_model.getMatrix();
	_mview = _view * _model;
	_imvp = glm::inverse(_mview);
	_nmat = glm::mat3(glm::transpose(_imvp));
}

void MyGlWindow::drawShader(ShaderProgram *shader)
{
	calculateNmat();
	calculateMVP();

	shader->use();

	glUniformMatrix4fv(shader->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(_mvp));

	glUniformMatrix4fv(shader->uniform("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(shader->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(_mview));

	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(_nmat));

	calculateSpotLight();
	glUniform4fv(_m_plane->shader->uniform("SpotLight.LightPosition"), 1, glm::value_ptr(_spotLight->lightPosition));
	glUniform3fv(_m_plane->shader->uniform("SpotLight.LightDirection"), 1, glm::value_ptr(_spotLight->lightDirection));

	shader->disable();
}

void MyGlWindow::draw()
{
	//Draw plane
	_m_model.glPushMatrix();

	if (_m_plane) {
		drawShader(_m_plane->shader);
		_m_plane->draw();
	}
	_m_model.glPopMatrix();

	//Draw cube
	_m_model.glPushMatrix();

	_m_model.glTranslate(-10, 0, 0);
	_m_model.glScale(2, 2, 2);

	if (_m_cube) {
		drawShader(_m_cube->shader);
		_m_cube->draw();
	}
	_m_model.glPopMatrix();

	//Draw sphere
	_m_model.glPushMatrix();

	_m_model.glTranslate(-5, 0, 0);
	_m_model.glScale(2, 2, 2);

	if (_m_sphere) {
		drawShader(_m_sphere->shader);
		_m_sphere->draw();
	}
	_m_model.glPopMatrix();

	//Draw teapot
	_m_model.glPushMatrix();

	_m_model.glTranslate(5, -0.51, 0);
	_m_model.glRotate(-90, 1, 0, 0);

	if (_m_teapot) {
		drawShader(_m_teapot->shader);
		_m_teapot->draw();
	}
	_m_model.glPopMatrix();

	//Draw mesh
	_m_model.glPushMatrix();

	_m_model.glTranslate(0, 0, 0);
	_m_model.glScale(2, 2, 2);

	if (_m_mesh) {
		drawShader(_m_mesh->shader);
		_m_mesh->draw();
	}
	_m_model.glPopMatrix();
}

/*
	//Activate Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Activate cull face
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	_m_OBJECT->draw();

	//Disable cull face and transparency
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
*/