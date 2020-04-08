#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Plane.h"

Plane::Plane(int size, int nsquare, float yPos) :
	_nb_plane_vertices(0), _plane_size(size), _nsquare(nsquare), _yPos(yPos)
{
	initShader();

	setupPlane();
	setupGl();
}

void Plane::initShader()
{
	shader = new ShaderProgram();
	//shader->initFromFiles("phong.vert", "phong.frag");
	shader->initFromFiles("spot.vert", "spot.frag");

	_Kd = glm::vec3(0.780392, 0.568627, 0.113725);
	_Ka = glm::vec3(0.329412, 0.223529, 0.027451);
	_Ks = glm::vec3(0.992157, 0.941176, 0.807843);
	_shiness = 27.89743616;

	shader->addUniform("Material.Kd");
	shader->addUniform("Material.Ka");
	shader->addUniform("Material.Ks");

	shader->addUniform("mvp");

	shader->addUniform("Material.Shiness");

	/*	std::string name;
		name = "Light[";
		for (size_t i = 0; i < 5; i++)
		{
			shader->addUniform(name + std::to_string(i) + "].LightLocation");
			shader->addUniform(name + std::to_string(i) + "].LightIntensity");
		}
	*/

	shader->addUniform("SpotLight.LightPosition");
	shader->addUniform("SpotLight.LightIntensity");
	shader->addUniform("SpotLight.LightDirection");
	shader->addUniform("SpotLight.LightExponent");
	shader->addUniform("SpotLight.LightCutoff");
	shader->addUniform("SpotLight.LightInnerCutoff");

	shader->addUniform("NormalMatrix");

	shader->addUniform("ViewMatrix");
	shader->addUniform("ModelViewMatrix");

	shader->use();
	glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(_Kd));
	glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(_Ka));
	glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(_Ks));
	glUniform1f(shader->uniform("Material.Shiness"), _shiness);
	shader->disable();
}

void Plane::setupPlane()
{
	glm::vec3 plane_normal = { 0, 1, 0 };

	float maxX = _plane_size / 2, maxY = _plane_size / 2;
	float minX = -_plane_size / 2, minY = -_plane_size / 2;

	int x, y, i = 0;

	float xp, yp;

	float xd = (maxX - minX) / ((float)_nsquare);
	float yd = (maxY - minY) / ((float)_nsquare);

	for (x = 0, xp = minX; x < _nsquare; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < _nsquare; y++, i++, yp += yd) {

			for (int c = 0; c < 6; c++)
				_plane_colors.push_back(plane_normal);
			// if i want a checkered floor
			// i % 2 == 1 ? _floor_colors.push_back(floor_colors1) : _floor_colors.push_back(floor_colors2);

			_plane_vertices.push_back(glm::vec3(xp, _yPos, yp));
			_plane_vertices.push_back(glm::vec3(xp, _yPos, yp + yd));
			_plane_vertices.push_back(glm::vec3(xp + xd, _yPos, yp + yd));
			_plane_vertices.push_back(glm::vec3(xp, _yPos, yp));
			_plane_vertices.push_back(glm::vec3(xp + xd, _yPos, yp));
			_plane_vertices.push_back(glm::vec3(xp + xd, _yPos, yp + yd));

			_nb_plane_vertices += 6;
		}
	}// end of for i
}

// Call from constructor
void Plane::setupGl()
{
	// Create VAO
	glGenVertexArrays(1, &_vaoHandle); // 1 : of VAOs, vaoHandle : vao handler
	glBindVertexArray(_vaoHandle); // Bind means that "i want to do something" : activate VAO

	// Create VBO 0 : vertex
	glGenBuffers(1, &_vbo_plane_vertices);

	// GL_ARRAY_BUFFER : type = buffer for containing vertex attribute
	// Bind : activate VBO, "I want to do something on this vbo"
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_plane_vertices);

	// Allocate memory on GPU and copy data
	// GL_STATIC_DRAW : This data are not going to be changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _plane_vertices.size() * 3, _plane_vertices.data(), GL_STATIC_DRAW);

	// Let GPU know how to interpret this data...
	glVertexAttribPointer(
		0, // Attribute.number : "I'm going to set 0 for vertex positions"
		3, // Data for vertex (x,y,z,w)
		GL_FLOAT, // Data type
		GL_FALSE, // "Is it normalized?"
		0, // Offset between real data
		0 // Offset from the starting of buffer
	);
	glEnableVertexAttribArray(0); //Activate vertex attribute 0

	glGenBuffers(1, &_vbo_plane_normal);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_plane_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _plane_colors.size() * 3, _plane_colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Close/Desactivate VAO
}

void Plane::draw()
{
	shader->use();
	
	glBindVertexArray(_vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, _nb_plane_vertices);
	
	shader->disable();
}