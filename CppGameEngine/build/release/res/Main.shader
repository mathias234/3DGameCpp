#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightSpaceMatrix;
uniform float u_SomeNumber;

out vec2 texCoord0;
out vec3 normal0;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out mat3 tbnMatrix;

void main()
{
	mat4 mvp = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix;

	// SET MVP
	gl_Position = mvp * vec4(position, 1);
	texCoord0 = texCoord;
	normal0 = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    fragPos = vec3(u_ModelMatrix * vec4(position, 1.0));
    fragPosLightSpace = u_LightSpaceMatrix * vec4(fragPos, 1.0);

    vec3 n = normalize((u_ModelMatrix * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((u_ModelMatrix * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t, biTangent, n);
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
uniform sampler2D u_ShadowMap;

uniform sampler2D u_Diffuse;

uniform bool u_UseSpecMap;
uniform sampler2D u_SpecMap;
uniform float u_SpecStrength;
uniform float u_SpecPow;

uniform sampler2D u_NormalMap;


uniform vec2 u_Tiling;
uniform vec3 u_ViewPos;

in vec2 texCoord0;
in vec3 normal0;
in vec3 fragPos;
in vec4 fragPosLightSpace;
in mat3 tbnMatrix;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5f;
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005; //max(0.05 * (1.0 - dot(normal0, u_DirectionalLight.Direction)), 0.0);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    vec2 texCoord = texCoord0.xy;// * u_Tiling;

	vec4 tex = texture2D(u_Diffuse, texCoord);
    vec4 specTex = texture2D(u_SpecMap, texCoord);
    vec3 normal = normal0;//normalize(tbnMatrix * (255.0/128.0 * texture2D(u_NormalMap, texCoord).xyz - 1));

	vec3 AmbientColor = vec3(u_DirectionalLight.AmbientIntensity);

	float DiffuseFactor = clamp(dot(normal, -u_DirectionalLight.Direction), 0.0, 1.0);
	
	vec3 DiffuseColor = vec3(0,0,0);
	
	if (DiffuseFactor > 0) {
		DiffuseColor = vec3(u_DirectionalLight.DiffuseIntensity * DiffuseFactor);
	}

    float SpecularStrength = 200;

    vec3 ViewDir = normalize(u_ViewPos - fragPos);
    vec3 ReflectDir = reflect(u_DirectionalLight.Direction, normal);

    float spec = pow(max(dot(ViewDir, ReflectDir), 0.0), u_SpecPow);

    vec3 specular = (u_SpecStrength * spec) * (u_UseSpecMap == true ? vec3(specTex) : vec3(1.0f, 1.0f, 1.0f));


    // calc shadows
    float shadow = ShadowCalculation(fragPosLightSpace);

	color = tex; //+ vec4((AmbientColor + (1.0 - shadow) * (DiffuseColor)), 1.0);
};