#include "SpotLight.h"

SpotLight::SpotLight(
	glm::vec4 position,
	glm::vec3 intensity,
	glm::vec3 direction,
	float exponent,
	float cutoff,
	float innerCutoff)
	: lightPosition(position),
	lightIntensity(intensity),
	lightDirection(direction),
	lightExponent(exponent),
	lightCutoff(cutoff),
	lightInnerCutoff(innerCutoff)
{}