//
// Created by matha on 27/03/2018.
//

#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(Vector3f position, Vector3f scale, Quaternion rotation, Model *model) {
    m_Position = position;
    m_Scale = scale;
    m_Rotation = rotation;
    m_Model = model;

    RecalculateModelMatrix();
}

void GameObject::Draw(Shader& shader) {
    shader.SetMatrix4("u_ModelMatrix", m_ModelMatrix);

    m_Model->Draw(shader);
}

void GameObject::RecalculateModelMatrix() {
    Matrix4f modelMatrix = Matrix4f();
    modelMatrix = glm::mat4_cast(m_Rotation);
    modelMatrix = glm::translate(modelMatrix, m_Position);
    modelMatrix = glm::scale(modelMatrix, m_Scale);

    m_ModelMatrix = modelMatrix;
}

Vector3f GameObject::GetScale() {
    return m_Scale;
}

void GameObject::SetScale(Vector3f scale) {
    m_Scale = scale;
    RecalculateModelMatrix();
}

Vector3f GameObject::GetPosition() {
    return m_Position;
}

void GameObject::SetPosition(Vector3f position) {
    m_Position = position;
    RecalculateModelMatrix();
}

Quaternion GameObject::GetRotation() {
    return m_Rotation;
}

void GameObject::SetRotation(Quaternion rotation) {
    m_Rotation = rotation;
    RecalculateModelMatrix();
}

Model *GameObject::GetModel() {
    return m_Model;
}

void GameObject::SetModel(Model *model) {
    m_Model = model;
}
