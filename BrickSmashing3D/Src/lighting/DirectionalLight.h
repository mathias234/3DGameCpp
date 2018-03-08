//
// Created by matha on 08/03/2018.
//

#pragma once

#include <Common.h>
#include <Shader.h>

class DirectionalLight {
public:
    DirectionalLight();
    DirectionalLight(Vector3f color, Vector3f direction, float ambientIntensity, float diffuseIntensity);


    void Bind(Shader& shader) const;

    const Vector3f &GetColor() const;
    void SetColor(const Vector3f &m_Color);

    const Vector3f &GetDirection() const;
    void SetDirection(const Vector3f &m_Direction);

    float GetAmbientIntensity() const;
    void SetAmbientIntensity(float m_AmbientIntensity);

    float GetDiffuseIntensity() const;
    void SetDiffuseIntensity(float m_DiffuseIntensity);



private:
    Vector3f m_Color;
    Vector3f m_Direction;
    float m_AmbientIntensity;
    float m_DiffuseIntensity;
};

