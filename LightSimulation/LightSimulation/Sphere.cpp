#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Sphere::Sphere(float rad, GLuint sl, GLuint st) :
_radius(rad), _slices(sl), _stacks(st)
{
	initShader();

	_sphere_vertices = (_slices + 1) * (_stacks + 1);
	_sphere_elements = (_slices * 2 * (_stacks - 1)) * 3;

	// Verts
	float *v = new float[3 * _sphere_vertices];
	// Normals
	float *n = new float[3 * _sphere_vertices];  
	// Tex coords
	float *tex = new float[2 * _sphere_vertices]; //we don't use it now
	// Index
	unsigned int *sphere_elements = new unsigned int[_sphere_elements]; //index

	// Generate the vertex data
	generateVerts(v, n, tex, sphere_elements);

	glGenVertexArrays(1, &_vaoHandle);
	glBindVertexArray(_vaoHandle);

	glGenBuffers(1, &_ibo_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_sphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _sphere_elements, sphere_elements, GL_STATIC_DRAW);

	glGenBuffers(1, &_vbo_sphere_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_sphere_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * _sphere_vertices, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_vbo_sphere_normals);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_sphere_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * _sphere_vertices, n, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete[] v;
	delete[] n;
	delete[] sphere_elements;
	delete[] tex;
}

void Sphere::initShader()
{
	shader = new ShaderProgram();
	shader->initFromFiles("phong.vert", "phong.frag");

	_Kd = glm::vec3(0.135, 0.2225, 0.1575);
	_Ka = glm::vec3(0.54, 0.89, 0.63);
	_Ks = glm::vec3(0.316228, 0.316228, 0.316228);
	_shiness = 12.8;

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

void Sphere::draw() 
{
	int size;

	shader->use();
	
	glBindVertexArray(_vaoHandle);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	
	shader->disable();
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / _slices;
	GLfloat phiFac = glm::pi<float>() / _stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= _slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / _slices;
		for (GLuint j = 0; j <= _stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / _stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = _radius * nx; verts[idx + 1] = _radius * ny; verts[idx + 2] = _radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < _slices; i++) {
		GLuint stackStart = i * (_stacks + 1);
		GLuint nextStackStart = (i + 1) * (_stacks + 1);
		for (GLuint j = 0; j < _stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == _stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

int Sphere::getVertexArrayHandle() 
{
	return this->_vaoHandle;
}