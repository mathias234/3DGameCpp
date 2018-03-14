#include "Platform.h"
#include <vector>
#include <GL/glew.h>
#include "Common.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ObjReader.h"
#include "Shader.h"
#include "vendor/physics/q3.h"

void Platform::Create(const std::string& path)
{
	m_Model = Model::GetModel(path);
}

Platform::Platform(float length, float height, q3Scene& physicsScene) : m_Length(length), m_Height(height)
{
	q3BodyDef bodyDef;
	bodyDef.active = true;
	bodyDef.bodyType = eKinematicBody;
	m_body = physicsScene.CreateBody(bodyDef);
	q3Transform localSpace;
	q3Identity(localSpace);
	q3BoxDef boxDef;
	boxDef.SetRestitution(0);
	boxDef.Set(localSpace, q3Vec3(length, 1.0, 1.0f));
	m_body->AddBox(boxDef);


	Create("./res/cube.obj");
}

Platform::~Platform()
{
	std::cout << "Deleting platform";
}

void Platform::Reload()
{
	Create("./res/cube.obj");
}

Matrix4f Platform::GetModelMatrix()
{
	Matrix4f matrix;

	matrix = glm::rotate(matrix, glm::radians(m_rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(m_rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(m_rotation.z), { 0, 0, 1 });
    matrix = glm::translate(matrix,  {m_body->GetTransform().position.x, m_body->GetTransform().position.y + 1, m_body->GetTransform().position.z});
    matrix = glm::scale(matrix, { m_Length,m_Height, 10 });
	return matrix;
}

void Platform::Draw(Shader& shader)
{
	shader.SetUniform2f("u_Tiling", { m_Length / 2, m_Height / 2 });
	m_Model->Draw();
}

