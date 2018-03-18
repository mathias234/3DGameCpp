#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"
#include "Texture.h"
#include "DepthMap.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
struct ShaderSamplerId {
	std::string UniformName;
	int SamplerId;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::unordered_map<std::string, int> m_SamplerIdsMap; // Use this to get sampler slot from string name
	std::vector<ShaderSamplerId> m_SamplerIds;            // Use this only if you need to iterate over the sampler maps

public:
	Shader(const std::string& filepath);
	~Shader();

	void Reload();
	void BindTexture(const std::string &uniformName, Texture& texture);
	void BindTexture(const std::string &uniformName, DepthMap& texture);
	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4fv(const std::string & name, Matrix4f value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string & name, Vector3f vec3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform2f(const std::string& name, Vector2f value);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);

};