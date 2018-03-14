//
// Created by matha on 08/03/2018.
//

#include "Model.h"
#include "ObjReader.h"

std::map<std::string, Model*> Model::m_ModelCache;

Model *Model::GetModel(const std::string &filepath) {
    if (m_ModelCache.find(filepath) == m_ModelCache.end())
    {
        Model *model = new Model();
        auto indexedModel = ObjReader::Read(filepath);

        model->m_Vbo = 0;
        model->m_Tbo = 0;
        model->m_Nbo = 0;
        model->m_Ibo = 0;

        GLCall(glGenBuffers(1, &model->m_Vbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, model->m_Vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, indexedModel.positions.size() * sizeof(indexedModel.positions[1]),
                            &indexedModel.positions.front(), GL_STATIC_DRAW));

        GLCall(glGenBuffers(1, &model->m_Tbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, model->m_Tbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, indexedModel.texCoords.size() * sizeof(indexedModel.texCoords[1]),
                            &indexedModel.texCoords.front(), GL_STATIC_DRAW));

        GLCall(glGenBuffers(1, &model->m_Nbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, model->m_Nbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, indexedModel.normals.size() * sizeof(indexedModel.normals[1]),
                            &indexedModel.normals.front(),
                            GL_STATIC_DRAW));

        GLCall(glGenBuffers(1, &model->m_Ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->m_Ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexedModel.indices.size() * sizeof(unsigned int),
                            &indexedModel.indices.front(), GL_STATIC_DRAW));
        model->m_IndexCount = indexedModel.indices.size();

        m_ModelCache.emplace(filepath, model);
    }

    return m_ModelCache[filepath];
}

Model::~Model() {
    std::cout << "Deleting model" << std::endl;
    glDeleteBuffers(1, &m_Vbo);
    glDeleteBuffers(1, &m_Tbo);
    glDeleteBuffers(1, &m_Ibo);
}

void Model::Draw() const {
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


