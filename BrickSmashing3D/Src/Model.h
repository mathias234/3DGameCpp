//
// Created by matha on 08/03/2018.
//
#pragma once

#include <vector>
#include <map>
#include "Common.h"

struct Submesh {
    unsigned int m_Vbo, m_Ibo, m_IndexCount;
};

class Model {
private:
    static std::map<std::string, Model*> m_ModelCache;
    std::vector<Submesh*> m_Submeshes;

public:
    ~Model();

    static Model* GetModel(const std::string& path);

    void Draw() const;
};

