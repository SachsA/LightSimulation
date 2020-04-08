#ifndef _MODEL
#define _MODEL

#include <stack>
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:

	std::stack<glm::mat4> modelstack;

	Model() {
		glm::mat4 id(1.0);
		modelstack.push(id);
	}

	void glTranslate(float x, float y, float z)
	{

		glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		modelstack.top() = modelstack.top() * Trans;

	}

	void glRotate(float degree, float x, float y, float z)
	{
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(x, y, z));
		modelstack.top() = modelstack.top() * rot;
	}

	void glScale(float x, float y, float z)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
		modelstack.top() = modelstack.top() * scale;
	}

	void glPushMatrix()
	{
		glm::mat4 t = getMatrix();
		modelstack.push(t);
	}

	glm::mat4 getMatrix()
	{
		if (modelstack.size() > 0)
			return modelstack.top();

		return glm::mat4(1.0);
	}

	void glPopMatrix()
	{
		modelstack.pop();
	}

};

#endif