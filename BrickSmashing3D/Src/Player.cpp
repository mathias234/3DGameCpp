#include "Player.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjReader.h"
#include "Common.h"
#include "Shader.h"
#include "vendor/physics/q3.h"
#include "Raycast.h"
#include "InputManager.h"

void Player::Create(const std::string& path)
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


Player::Player(q3Scene& physicsScene) : m_Vbo(0), m_Ibo(0), m_Tbo(0)
{
    m_editorMode = false;
	m_physicsScene = &physicsScene;
	q3BodyDef bodyDef;
	bodyDef.active = true;
	bodyDef.bodyType = eDynamicBody;
	bodyDef.position = q3Vec3(0, 20, 0);
	m_body = physicsScene.CreateBody(bodyDef);


	q3Transform localSpace;
	q3Identity(localSpace);

	q3BoxDef boxDef;
	boxDef.SetRestitution(0);
	boxDef.Set(localSpace, q3Vec3(1.0, 1.0, 1.0f));
	m_body->AddBox(boxDef);

	Create("res/player.obj");
}


Player::~Player()
{
	std::cout << "Deleting player";
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Tbo);
	glDeleteBuffers(1, &m_Ibo);
}

void Player::Reload()
{
	glDeleteBuffers(1, &m_Vbo);
	glDeleteBuffers(1, &m_Tbo);
	glDeleteBuffers(1, &m_Ibo);
	Create("res/player.obj");
}

void Player::Draw(Shader& shader)
{
	shader.SetUniform2f("u_Tiling", { 1,1 });
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

Matrix4f Player::GetModelMatrix()
{
	Matrix4f matrix;

	matrix = glm::translate(matrix, { m_body->GetTransform().position.x, m_body->GetTransform().position.y, m_body->GetTransform().position.z });
	matrix = glm::scale(matrix, { 1,1,1 });

	matrix = glm::rotate(matrix, glm::radians(0.0f), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(0.0f), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(0.0f), { 0, 0, 1 });

	return matrix;
}

void Player::NormalInput() {
    auto oldPos = m_body->GetTransform().position;
    // Force Z to always be 0
    m_body->SetTransform({ oldPos.x, oldPos.y, 0 });

    float speed = 0.25f;

    q3Vec3 newVelocity = m_body->GetLinearVelocity();

    if (InputManager::GetKey(GLFW_KEY_A))
    {
        newVelocity -= { speed, 0, 0 };
    }
    if (InputManager::GetKey(GLFW_KEY_D))
    {
        newVelocity += { speed, 0, 0 };
    }
    if (InputManager::GetKeyDown(GLFW_KEY_SPACE))
    {
        float distance = 50000;
        Raycast raycast;
        auto raycastPoint = m_body->GetTransform().position;
        raycastPoint.y -= 0.8f;
        raycast.Init(raycastPoint, { 0, -1, 0 });
        m_physicsScene->RayCast(&raycast, raycast.data);

        if (raycast.impactBody)
        {
            Vector3f impact = { raycast.data.GetImpactPoint().x, raycast.data.GetImpactPoint().y, raycast.data.GetImpactPoint().z };
            Vector3f playerPos = { m_body->GetTransform().position.x, m_body->GetTransform().position.y, m_body->GetTransform().position.z };
            distance = glm::distance(impact, playerPos);
        }

        if (distance <= 1) {
            newVelocity += {0, 8, 0};
        }
    }




    newVelocity.x = q3Clamp(-5, 5, newVelocity.x);
    newVelocity.y = q3Clamp(-50000, 10, newVelocity.y);

    m_body->SetLinearVelocity(newVelocity);
}


void Player::EditorInput() {
    float speed = 0.2f;

    q3Vec3 newPos = m_body->GetTransform().position;

    if(InputManager::GetKey(GLFW_KEY_A)) {
        newPos -= {speed, 0, 0};
    }
    if(InputManager::GetKey(GLFW_KEY_D)) {
        newPos += {speed, 0, 0};
    }
    if(InputManager::GetKey(GLFW_KEY_W)) {
        newPos += {0, speed, 0};
    }
    if(InputManager::GetKey(GLFW_KEY_S)) {
        newPos -= {0, speed, 0};
    }


    m_body->SetTransform(newPos);
}

void Player::Input()
{
    if(!m_editorMode)
        NormalInput();
    else
        EditorInput();
}

Vector3f Player::GetPosition()
{
	return  { m_body->GetTransform().position.x, m_body->GetTransform().position.y, m_body->GetTransform().position.z };
}

void Player::SetEditorMode(bool value) {
    m_editorMode = value;
}
