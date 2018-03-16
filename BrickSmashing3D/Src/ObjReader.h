#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <tuple>
#include <map>
#include "IndexModel.h"


struct ObjIndex
{
public:
	int VertexIndex;
	int TexCoordIndex;
	int NormalIndex;


};

inline bool operator< (const ObjIndex& lhs, const ObjIndex& rhs)
{
	// std::tuple's lexicographic ordering does all the actual work for you
	// and using std::tie means no actual copies are made
	return std::tie(lhs.VertexIndex, lhs.NormalIndex, lhs.TexCoordIndex) <  std::tie(rhs.VertexIndex, rhs.NormalIndex, rhs.TexCoordIndex);
}

class ObjReader
{
public:
	static IndexedModel Read(const std::string& filepath);


private:
	ObjReader(const std::string& filepath);

	IndexedModel ToIndexModel();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> tangents;
	std::vector<ObjIndex> indices;


	static std::map<std::string, IndexedModel> modelCache;

	ObjIndex ParseObjIndex(const char* token);

	bool m_hasTexCoords = true;
	bool m_hasNormals;


};