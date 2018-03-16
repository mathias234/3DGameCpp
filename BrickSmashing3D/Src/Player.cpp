#include "Player.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjReader.h"
#include "Common.h"
#include "Shader.h"
#include "vendor/physics/q3.h"
#include "Raycast.h"
#include "InputManager.h"
#include "Model.h"

void Player::Create(const std::string& path)
{
	m_Model = Model::GetModel(path);
}


Player::Player(q3Scene& physicsScene)
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
}

void Player::Reload()
{
	Create("res/player.obj");
}

void Player::Draw(Shader& shader)
{
	shader.SetUniform2f("u_Tiling", { 0.5f,0.5f });
	m_Model->Draw();

}

Matrix4f Player::GetModelMatrix()
{
	Matrix4f matrix;

	matrix = glm::rotate(matrix, glm::radians(0.0f), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(0.0f), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(0.0f), { 0, 0, 1 });

    matrix = glm::translate(matrix, { m_body->GetTransform().position.x, m_body->GetTransform().position.y, m_body->GetTransform().position.z });
    matrix = glm::scale(matrix, { 1,1,1 });

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
