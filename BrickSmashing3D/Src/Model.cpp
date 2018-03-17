//
// Created by matha on 08/03/2018.
//

#include "Model.h"
#include "OBJ_Loader.h"

std::map<std::string, Model*> Model::m_ModelCache;

Model *Model::GetModel(const std::string &filepath) {
    if (m_ModelCache.find(filepath) == m_ModelCache.end())
    {
        Model *finalModel = new Model();

        objl::Loader m_Loader;

        if(!m_Loader.LoadFile(filepath))
        {
            std::cout << "Failed to load model at path: " << filepath << std::endl;
            return nullptr;
        }

    

        for (int i = 0; i < m_Loader.LoadedMeshes.size(); ++i) {
            auto loadedMesh = m_Loader.LoadedMeshes[i];
            Submesh* model = new Submesh;

            GLCall(glGenBuffers(1, &model->m_Vbo));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, model->m_Vbo));
            GLCall(glBufferData(GL_ARRAY_BUFFER, loadedMesh.Vertices.size() * sizeof(loadedMesh.Vertices[1]),
                                &loadedMesh.Vertices.front(), GL_STATIC_DRAW));


            GLCall(glGenBuffers(1, &model->m_Ibo));
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->m_Ibo));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedMesh.Indices.size() * sizeof(unsigned int),
                                &loadedMesh.Indices.front(), GL_STATIC_DRAW));

            model->m_IndexCount = loadedMesh.Indices.size();

            finalModel->m_Submeshes.push_back(model);
        }

        m_ModelCache.emplace(filepath, finalModel);
    }

    return m_ModelCache[filepath];
}

Model::~Model() {
    std::cout << "Deleting model" << std::endl;
    for (int i = 0; i < m_Submeshes.size(); ++i) {
        GLCall(glDeleteBuffers(1, &m_Submeshes[i]->m_Vbo));
        GLCall(glDeleteBuffers(1, &m_Submeshes[i]->m_Ibo));
    }
}

void Model::Draw() const {
    for (int i = 0; i < m_Submeshes.size(); ++i) {
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glEnableVertexAttribArray(2));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Submeshes[i]->m_Vbo));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void *) 0));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void *) sizeof(objl::Vector3)));
        GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex),
                                     (void *) (sizeof(objl::Vector3) + sizeof(objl::Vector3))));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Submeshes[i]->m_Ibo));
        GLCall(glDrawElements(GL_TRIANGLES, m_Submeshes[i]->m_IndexCount, GL_UNSIGNED_INT, nullptr));

        GLCall(glDisableVertexAttribArray(0));
        GLCall(glDisableVertexAttribArray(1));
        GLCall(glDisableVertexAttribArray(2));
    }
}


