#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;


uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightSpaceMatrix;

out vec2 texCoord0;
out vec3 normal0;
out vec3 fragPos;
out vec4 fragPosLightSpace;
void main()
{
	mat4 mvp = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix;

	// SET MVP
	gl_Position = mvp * vec4(position, 1);
	texCoord0 = texCoord;
	normal0 = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    fragPos = vec3(u_ModelMatrix * vec4(position, 1.0));
    fragPosLightSpace = u_LightSpaceMatrix * vec4(fragPos, 1.0);
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
uniform sampler2D u_ShadowMap;
uniform vec2 u_Tiling;
uniform vec3 u_ViewPos;

in vec2 texCoord0;
in vec3 normal0;
in vec3 fragPos;
in vec4 fragPosLightSpace;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5f;
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal0, u_DirectionalLight.Direction)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{

	vec3 AmbientColor = vec3(u_DirectionalLight.AmbientIntensity);

	float DiffuseFactor = clamp(dot(normal0, u_DirectionalLight.Direction), 0.0, 1.0);
	
	vec3 DiffuseColor = vec3(0,0,0);
	
	if (DiffuseFactor > 0) {
		DiffuseColor = vec3(u_DirectionalLight.DiffuseIntensity * DiffuseFactor);
	}


    float SpecularStrength = 0.5f;

    vec3 ViewDir = normalize(u_ViewPos - fragPos);
    vec3 ReflectDir = reflect(-u_DirectionalLight.Direction, normal0);

    float spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);

    vec3 specular = SpecularStrength * spec * u_DirectionalLight.Color;

	vec4 tex = texture2D(u_Diffuse, texCoord0.xy * u_Tiling);

    // calc shadows
    float shadow = ShadowCalculation(fragPosLightSpace);


	color = (tex) * vec4((AmbientColor + (1.0 - shadow)) * (DiffuseColor), 1.0);
};