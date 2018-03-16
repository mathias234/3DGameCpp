//
// Created by matha on 16/03/2018.
//

#include "IndexModel.h"
#include "Common.h"


void IndexedModel::CalculateTangents(){
    for (int i = 0; i < indices.size(); i += 3) {
        GLuint i0 = indices[i];
        GLuint i1 = indices[i + 1];
        GLuint i2 = indices[i + 2];


        Vector3f edge1 = positions[i1] - positions[i0];
        Vector3f edge2 = positions[i2] - positions[i0];


        float deltaU1 = texCoords[i1].x - texCoords[i0].x;
        float deltaV1 = texCoords[i1].y - texCoords[i0].y;
        float deltaU2 = texCoords[i2].x - texCoords[i0].x;
        float deltaV2 = texCoords[i2].y - texCoords[i0].y;

        float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
        float f = abs(dividend) < 0.001f ? 0.0f : 1.0f / dividend;

        Vector3f tangent = Vector3f(0, 0, 0);
        tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
        tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
        tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

        tangents[i0] = tangents[i0] + tangent;
        tangents[i1] = tangents[i1] + tangent;
        tangents[i2] = tangents[i2] + tangent;
    }

    for (int i = 0; i < tangents.size(); i++) {
        tangents[i] = glm::normalize(tangents[i]);
    }
}