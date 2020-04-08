#include <glm/gtx/string_cast.hpp>

#include "MyGlUtiles.h"

//When i fill lines, it's in column !
glm::mat4 MyGlUtiles::lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up)
{
	glm::mat4 mat;

	glm::vec3 Zc = glm::normalize(campos - look);
	glm::vec3 Xc = glm::normalize(glm::cross(up, Zc));
	glm::vec3 Yc = glm::normalize(glm::cross(Zc, Xc));

	mat[0] = glm::vec4(Xc.x, Yc.x, Zc.x, 0);
	mat[1] = glm::vec4(Xc.y, Yc.y, Zc.y, 0);
	mat[2] = glm::vec4(Xc.z, Yc.z, Zc.z, 0);
	mat[3] = glm::vec4(0, 0, 0, 1);

	return glm::translate(mat, look - campos);
}

//Don't forget to convert to radian + when i fill lines, it's in column !
glm::mat4 MyGlUtiles::perspective(float fovy, float aspect, float near, float far)
{
	glm::mat4 mat;

	float angle = glm::tan(glm::radians(fovy / 2));

	mat[0] = glm::vec4(1 / (aspect * angle), 0, 0, 0);
	mat[1] = glm::vec4(0, 1 / angle, 0, 0);
	mat[2] = glm::vec4(0, 0, -((far + near) / (far - near)), -1);
	mat[3] = glm::vec4(0, 0, -((2 * far * near) / (far - near)), 0);

	return mat;
}

/*

These are examples of the uses of the lookat and perpective functions.

//EXECUTE LOOK AT FUNCTION
glm::vec3 camPos(5, 5, 5);
glm::vec3 look(0, 0, 0);
glm::vec3 up(0, 1, 0);

std::cout << glm::to_string(lookAt(camPos, look, up)) << std::endl;

//EXECUTE PERSPECTIVE FUNCTION
float w = 780;
float h = 750;
float fovY = 45.0f;
float nearZ = 0.1f;
float farZ = 500.0f;

std::cout << glm::to_string(perspective(fovY, w/h, nearZ, farZ)) << std::endl;

*/