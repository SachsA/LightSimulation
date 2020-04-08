#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"

Cube::Cube()
{
	initShader();

	setupVertices();
	setupGl();
}

void Cube::initShader()
{
	shader = new ShaderProgram();
	shader->initFromFiles("phong.vert", "phong.frag");

	_Ka = glm::vec3(0.0215, 0.1745, 0.0215);
	_Kd = glm::vec3(0.07568, 0.61424, 0.07568);
	_Ks = glm::vec3(0.633, 0.727811, 0.633);
	_shiness = 76.8;

	shader->addUniform("Material.Kd");
	shader->addUniform("Material.Ka");
	shader->addUniform("Material.Ks");

	shader->addUniform("mvp");

	shader->addUniform("Material.Shiness");

	std::string name;
	name = "Light[";
	for (size_t i = 0; i < 5; i++)
	{
		shader->addUniform(name + std::to_string(i) + "].LightLocation");
		shader->addUniform(name + std::to_string(i) + "].LightIntensity");
	}

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

glm::vec3 Cube::computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return glm::cross(p2 - p1, p3 - p1);
}

void Cube::setupVertices()
{
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));

	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	_cube_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));

	for (size_t i = 0; i < _cube_vertices.size(); i += 3) {
		_cube_normals.push_back(computeNormal(_cube_vertices[i], _cube_vertices[i + 1], _cube_vertices[i + 2]));
		_cube_normals.push_back(computeNormal(_cube_vertices[i], _cube_vertices[i + 1], _cube_vertices[i + 2]));
		_cube_normals.push_back(computeNormal(_cube_vertices[i], _cube_vertices[i + 1], _cube_vertices[i + 2]));
	}
}

// Call from constructor
void Cube::setupGl()
{
	// Create VAO
	glGenVertexArrays(1, &_vaoHandle); // 1 : of VAOs, vaoHandle : vao handler
	glBindVertexArray(_vaoHandle); // Bind means that "i want to do something" : activate VAO

	// Create VBO 0 : vertex
	glGenBuffers(1, &_vbo_cube_vertices);

	// GL_ARRAY_BUFFER : type = buffer for containing vertex attribute
	// Bind : activate VBO, "I want to do something on this vbo"
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_vertices);

	// Allocate memory on GPU and copy data
	// GL_STATIC_DRAW : This data are not going to be changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_vertices.size() * 4, _cube_vertices.data(), GL_STATIC_DRAW);

	// Let GPU know how to interpret this data...
	glVertexAttribPointer(
		0, // Attribute.number : "I'm going to set 0 for vertex positions"
		4, // Data for vertex (x,y,z,w)
		GL_FLOAT, // Data type
		GL_FALSE, // "Is it normalized?"
		0, // Offset between real data
		0 // Offset from the starting of buffer
	);
	glEnableVertexAttribArray(0); //Activate vertex attribute 0

	glGenBuffers(1, &_vbo_cube_normals);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_normals.size() * 3, _cube_normals.data(), GL_STATIC_DRAW);
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

void Cube::draw()
{
	shader->use();
	
	glBindVertexArray(_vaoHandle);

	for (int i = 0; i < _cube_vertices.size() * 3; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);
	
	shader->disable();
}