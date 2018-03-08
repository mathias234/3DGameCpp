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
	auto model = ObjReader::Read(path);



	GLCall(glGenBuffers(1, &m_Vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[1]), &model.positions.front(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_Tbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Tbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[1]), &model.texCoords.front(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_Nbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Nbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[1]), &model.normals.front(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &m_Ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), &model.indices.front(), GL_STATIC_DRAW));

	m_IndexCount = model.indices.size();

}

Platform::Platform(float length, float height, q3Scene& physicsScene) : m_Length(length), m_Height(height), m_Vbo(0), m_Tbo(0), m_Ibo(0)
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


	Reload();
}

Platform::~Platform()
{
	std::cout << "Deleting platform";
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Tbo);
	glDeleteBuffers(1, &m_Ibo);
}

void Platform::Reload()
{
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Tbo);
	glDeleteBuffers(1, &m_Ibo);

	Create("./res/cube.obj");
}

Matrix4f Platform::GetModelMatrix()
{
	Matrix4f matrix;

	matrix = glm::translate(matrix,  {m_body->GetTransform().position.x, m_body->GetTransform().position.y + 1, m_body->GetTransform().position.z});
	matrix = glm::scale(matrix, { m_Length,m_Height,1 });

	matrix = glm::rotate(matrix, glm::radians(m_rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(m_rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(m_rotation.z), { 0, 0, 1 });

	return matrix;
}

void Platform::Draw(Shader& shader)
{
	shader.SetUniform2f("u_Tiling", { m_Length / 2, m_Height / 2 });
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Tbo));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Nbo));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo));
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));

	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	GLCall(glDisableVertexAttribArray(2));

}

