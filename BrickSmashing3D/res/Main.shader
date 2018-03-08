#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;


uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

out vec2 texCoord0;
out vec3 normal0;

void main()
{
	mat4 mvp = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix;

	// SET MVP
	gl_Position = mvp * vec4(position, 1);
	texCoord0 = texCoord;
	normal0 = mat3(transpose(inverse(u_ModelMatrix))) * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct DirectionalLight {
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Direction;
};

uniform DirectionalLight u_DirectionalLight;
uniform sampler2D u_Diffuse;
uniform vec2 u_Tiling;

in vec2 texCoord0;
in vec3 normal0;

void main()
{
	vec4 AmbientColor = vec4(u_DirectionalLight.Color * u_DirectionalLight.AmbientIntensity, 1.0f);
	
	float DiffuseFactor = clamp(dot(normal0, -normalize(u_DirectionalLight.Direction)), 0.0, 1.0);
	
	vec4 DiffuseColor;
	
	if (DiffuseFactor > 0) {
		DiffuseColor = vec4(u_DirectionalLight.Color * u_DirectionalLight.DiffuseIntensity * DiffuseFactor, 1.0f);
	}
	else {
		DiffuseColor = vec4(0, 0, 0, 0);
	}
	
	vec4 tex = texture2D(u_Diffuse, texCoord0.xy * u_Tiling);
	color = (tex) * (AmbientColor + DiffuseColor);
};