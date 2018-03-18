//
// Created by matha on 16/03/2018.
//

#pragma once
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <tuple>
#include <map>

struct IndexedModel
{
public:
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> tangents;
	std::vector<GLuint> indices;

public:
	void CalculateTangents();
};


