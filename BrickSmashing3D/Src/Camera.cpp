#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() : m_position({0,0,0})
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(Vector3f position)
{
	m_position = position;
}

void Camera::Input(GLFWwindow* window)
{

}

Vector3f Camera::GetPosition()
{
	return m_position;
}


Matrix4f Camera::GetViewMatrix()
{

	Matrix4f matrix;
	matrix = glm::rotate(matrix, glm::radians(Rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(Rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(Rotation.z), { 0, 0, 1 });

	matrix = glm::translate(matrix, -m_position);

	return matrix;
}
