#pragma once
#include <string>
#include "Camera.h"
#include "vendor/physics/q3.h"
#include "Model.h"

class Shader;

class Player
{
private:
	Model* m_Model;
    bool m_editorMode;

	Vector3f m_position;

	q3Body* m_body;
	q3Scene* m_physicsScene;

	void Create(const std::string& path);
public:
	Player(q3Scene& physicsScene);
	~Player();

	void Reload();

	void Draw(Shader& shader);
	Matrix4f GetModelMatrix();
	void Input();
	Vector3f GetPosition();

    void SetEditorMode(bool value);

    void NormalInput();

	void EditorInput();
};

