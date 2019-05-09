#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() : Position({0,0,0})
{
}

Camera::~Camera()
{
}


void Camera::Input(GLFWwindow* window)
{

}



Matrix4f Camera::GetViewMatrix()
{

	Matrix4f matrix = Matrix4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	

	matrix = glm::rotate(matrix, glm::radians(20.0f),	   { 1.0f, 0.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f });

	matrix = glm::translate(matrix, -Position);

	return matrix;
}
