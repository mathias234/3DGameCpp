#pragma once
#include "Common.h"
#include <GLFW/glfw3.h>

class Camera
{
private:
	Vector3f m_position;
public:
	Camera();
	~Camera();

	Vector3f Rotation;

	Matrix4f GetViewMatrix();
	void SetPosition(Vector3f position);
	void Input(GLFWwindow* window);
	Vector3f GetPosition();
};

