#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Light
{
public:
	Light(glm::vec4 position, glm::vec3 intensity);
	~Light() = default;

	glm::vec4 lightPosition;
	glm::vec3 lightIntensity;
};