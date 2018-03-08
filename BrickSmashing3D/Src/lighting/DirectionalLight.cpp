//
// Created by matha on 08/03/2018.
//

#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(Vector3f color, Vector3f direction, float ambientIntensity, float diffuseIntensity) {
    m_Color = color;
    m_Direction = direction;
    m_AmbientIntensity = ambientIntensity;
    m_DiffuseIntensity = diffuseIntensity;
}

DirectionalLight::DirectionalLight() = default;

void DirectionalLight::Bind(Shader &shader) const {
    shader.SetUniform3f("u_DirectionalLight.Color", m_Color);
    shader.SetUniform3f("u_DirectionalLight.Direction", m_Direction);
    shader.SetUniform1f("u_DirectionalLight.AmbientIntensity", m_AmbientIntensity);
    shader.SetUniform1f("u_DirectionalLight.DiffuseIntensity", m_DiffuseIntensity);
}

const Vector3f &DirectionalLight::GetColor() const {
    return m_Color;
}

void DirectionalLight::SetColor(const Vector3f &m_Color) {
    DirectionalLight::m_Color = m_Color;
}

const Vector3f &DirectionalLight::GetDirection() const {
    return m_Direction;
}

void DirectionalLight::SetDirection(const Vector3f &m_Direction) {
    DirectionalLight::m_Direction = m_Direction;
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
