//
// Created by matha on 08/03/2018.
//

#include "Common.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3f color, Vector3f rotation, float ambientIntensity, float diffuseIntensity) {
    m_Color = color;

    m_Rotation = rotation;
    m_AmbientIntensity = ambientIntensity;
    m_DiffuseIntensity = diffuseIntensity;
}

DirectionalLight::DirectionalLight() = default;

void DirectionalLight::Bind(Shader &shader) const {
    shader.SetUniform3f("u_DirectionalLight.Color", m_Color);
    shader.SetUniform3f("u_DirectionalLight.Direction", GetDirection());
    shader.SetUniform1f("u_DirectionalLight.AmbientIntensity", m_AmbientIntensity);
    shader.SetUniform1f("u_DirectionalLight.DiffuseIntensity", m_DiffuseIntensity);
}

const Vector3f &DirectionalLight::GetColor() const {
    return m_Color;
}

void DirectionalLight::SetColor(const Vector3f &m_Color) {
    DirectionalLight::m_Color = m_Color;
}

const Vector3f DirectionalLight::GetDirection() const {
    Vector3f transformedRotation = {m_Rotation.x, m_Rotation.y + 360, m_Rotation.z};

    Quaternion finalOrientation = Quaternion(transformedRotation);

    Vector3f forwardDirection = glm::rotate(finalOrientation, {0,0,-1});

    return forwardDirection;
}


const Vector3f &DirectionalLight::GetRotation() const {
    return m_Rotation;
}

void DirectionalLight::SetRotation(const Vector3f &rotation) {
    m_Rotation = rotation;
}


float DirectionalLight::GetAmbientIntensity() const {
    return m_AmbientIntensity;
}

void DirectionalLight::SetAmbientIntensity(float m_AmbientIntensity) {
    DirectionalLight::m_AmbientIntensity = m_AmbientIntensity;
}

float DirectionalLight::GetDiffuseIntensity() const {
    return m_DiffuseIntensity;
}

void DirectionalLight::SetDiffuseIntensity(float m_DiffuseIntensity) {
    DirectionalLight::m_DiffuseIntensity = m_DiffuseIntensity;
}

