//
// Created by matha on 08/03/2018.
//

#include <vendor/tinyobjloader/tiny_obj_loader.h>
#include "Model.h"
#include "vendor/tinyobjloader/tiny_obj_loader.h"
#include "Texture.h"
std::map<std::string, Model*> Model::m_ModelCache;

Model *Model::GetModel(const std::string &filepath) {
    if (m_ModelCache.find(filepath) == m_ModelCache.end())
    {
        Model *finalModel = new Model();

        std::string err;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str(), "res/");


        if(!ret)
        {
            std::cout << "Failed to load model: " << err << std::endl;
            return nullptr;
        }

        for (size_t s = 0; s < shapes.size(); s++) {
            Submesh* model = new Submesh;

            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                int fv = shapes[s].mesh.num_face_vertices[f];

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                    tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                    tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                    tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                    tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                    tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                    tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                    tinyobj::real_t ty =  attrib.texcoords[2*idx.texcoord_index+1];
                    model->Indices.push_back(index_offset + v);
                    model->Vertices.push_back(Vertex({vx, vy, vz}, {nx,ny,nz}, {tx,ty}, {1.0f, 1.0f, 1.0f}));
                }
                index_offset += fv;

            }



            model->Material.Diffuse = new Texture("res/" + materials[shapes[s].mesh.material_ids[0]].diffuse_texname);
            //model->Material.NormalMap = new Texture("res/" + materials[shapes[s].mesh.material_ids[0]].normal_texname);
            //model->Material.SpecMap = new Texture("res/" + materials[shapes[s].mesh.material_ids[0]].specular_texname);


            GLCall(glGenBuffers(1, &model->m_Vbo));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, model->m_Vbo));
            GLCall(glBufferData(GL_ARRAY_BUFFER,
                                model->Vertices.size() * sizeof(Vertex),
                                &model->Vertices.front(),
                                GL_STATIC_DRAW));


            GLCall(glGenBuffers(1, &model->m_Ibo));
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->m_Ibo));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                model->Indices.size() * sizeof(unsigned int),
                                &model->Indices.front(),
                                GL_STATIC_DRAW));

            model->m_IndexCount = model->Indices.size();


            model->RecalculateTangents();

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

void Model::Draw(Shader& shader) const {
    for (int i = 0; i < m_Submeshes.size(); ++i) {
        //shader.BindTexture("u_Diffuse", *m_Submeshes[i]->Material.Diffuse);
        //shader.BindTexture("u_NormalMap", *m_Submeshes[i]->Material.Diffuse);


        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glEnableVertexAttribArray(2));
        GLCall(glEnableVertexAttribArray(3));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Submeshes[i]->m_Vbo));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0)); // Positions offset = 0

        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(Vector3f))); // Normals offset = Positions

        GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (void *) (sizeof(Vector3f) + sizeof(Vector3f)))); // TextureCoords offset = Positions + Normals

        GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (void *) (sizeof(Vector3f) + sizeof(Vector3f) + sizeof(Vector2f)))); // Tangents offset = Positions + Normals + TextureCoords

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Submeshes[i]->m_Ibo));
        GLCall(glDrawElements(GL_TRIANGLES, m_Submeshes[i]->m_IndexCount, GL_UNSIGNED_INT, nullptr));

        GLCall(glDisableVertexAttribArray(0));
        GLCall(glDisableVertexAttribArray(1));
        GLCall(glDisableVertexAttribArray(2));
        GLCall(glDisableVertexAttribArray(3));
    }
}

void Submesh::RecalculateTangents(){
    for (int i = 0; i < Indices.size(); i += 3) {
        GLuint i0 = Indices[i];
        GLuint i1 = Indices[i + 1];
        GLuint i2 = Indices[i + 2];


        Vector3f edge1 = Vertices[i1].Position - Vertices[i0].Position;
        Vector3f edge2 = Vertices[i2].Position - Vertices[i0].Position;


        float deltaU1 = Vertices[i1].TexCoord.x - Vertices[i0].TexCoord.x;
        float deltaV1 = Vertices[i1].TexCoord.y - Vertices[i0].TexCoord.y;
        float deltaU2 = Vertices[i2].TexCoord.x - Vertices[i0].TexCoord.x;
        float deltaV2 = Vertices[i2].TexCoord.y - Vertices[i0].TexCoord.y;

        float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
        float f = abs(dividend) < 0.001f ? 0.0f : 1.0f / dividend;

        Vector3f tangent = Vector3f(0, 0, 0);
        tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
        tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
        tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

        Vertices[i0].Tangent = Vertices[i0].Tangent + tangent;
        Vertices[i1].Tangent = Vertices[i1].Tangent + tangent;
        Vertices[i2].Tangent = Vertices[i2].Tangent + tangent;
    }

    for (int i = 0; i < Vertices.size(); i++) {
        Vertices[i].Tangent = glm::normalize(Vertices[i].Tangent);
    }
}

Vertex::Vertex(Vector3f position, Vector3f normal, Vector2f texCoord, Vector3f tangent) {
    Position = position;
    Normal = normal;
    TexCoord = texCoord;
    Tangent = tangent;
}