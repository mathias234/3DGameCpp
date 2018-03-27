//
// Created by matha on 27/03/2018.
//

#pragma once

#include "Common.h"
#include "Model.h"

class GameObject {
private:
    Model* m_Model;
    Vector3f m_Scale;
    Vector3f m_Position;
    Quaternion m_Rotation;
    Matrix4f m_ModelMatrix;

    void RecalculateModelMatrix();
public:
    GameObject(Vector3f position, Vector3f scale, Quaternion rotation, Model* model);

    void Draw(Shader& shader);

    Vector3f GetScale();
    void SetScale(Vector3f scale);

    Vector3f GetPosition();
    void SetPosition(Vector3f position);

    Quaternion GetRotation();
    void SetRotation(Quaternion rotation);

    Model* GetModel();
    void SetModel(Model* model);
};

