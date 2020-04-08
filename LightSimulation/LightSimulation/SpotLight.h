#pragma once

#include <glm/gtc/type_ptr.hpp>

class SpotLight
{
public:
	SpotLight(
		glm::vec4 position,
		glm::vec3 intensity,
		glm::vec3 direction,
		float exponent,
		float cutoff,
		float innerCutoff);
	~SpotLight() = default;

	glm::vec4 lightPosition;
	glm::vec3 lightIntensity;
	glm::vec3 lightDirection;

	float lightExponent;
	float lightCutoff;
	float lightInnerCutoff;
};