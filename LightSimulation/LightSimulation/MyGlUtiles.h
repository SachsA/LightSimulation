#pragma once
class MyGlUtiles
{
public:
	MyGlUtiles() = default;
	~MyGlUtiles() = default;

	glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
	
	glm::mat4 perspective(float fovy, float aspect, float near, float far);
};

