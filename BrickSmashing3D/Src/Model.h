//
// Created by matha on 08/03/2018.
//
#pragma once

#include <vector>
#include <map>
#include "Common.h"

class Model {
private:
    static std::map<std::string, Model*> m_ModelCache;
    unsigned int m_Vbo, m_Ibo, m_Tbo, m_Nbo, m_IndexCount;

    std::vector<Vector3f> m_Vertices;
    std::vector<Vector3f> m_Normals;
    std::vector<Vector2f> m_Uvs;
    std::vector<float> m_Indices;

public:
    ~Model();

    static Model* GetModel(const std::string& path);

    void Draw() const;
};

