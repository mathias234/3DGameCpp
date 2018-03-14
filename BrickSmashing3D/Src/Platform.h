#pragma once
#include "Common.h"
#include "vendor/physics/q3.h"
#include "Model.h"

class Shader;

class Platform
{
private:
	float m_Length, m_Height;

	Model* m_Model;

	Vector3f m_rotation;
	Vector3f m_position;

	q3Body* m_body;

	void Create(const std::string& path);
public:
	Platform(float length, float height, q3Scene& physicsScene);
	~Platform();


	void Reload();

	Matrix4f GetModelMatrix();
	void Draw(Shader& shader);
	void Update(){}

	void SetPosition(Vector3f position)
	{
		m_body->SetTransform({ position.x, position.y - 0.5f, position.z });
	}

	void SetRotation(Vector3f rotation)
	{
		m_rotation = rotation;
	}

	float GetLength()
	{
		return m_Length;
	}

	float GetHeight()
	{
		return m_Height;
	}

	Vector3f GetPosition()
	{
		return { m_body->GetTransform().position.x,m_body->GetTransform().position.y, m_body->GetTransform().position.z };
	}
};

