#version 430 core

struct Material
{
	vec4 emission;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float intensity;
};

struct DirectionLight
{
	vec3 direction;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;	
};

struct PointLight
{
	vec3 position;

	vec4 diffuse;
	vec4 ambient;
	vec4 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec4 diffuse;
	vec4 ambient;
	vec4 specular;

	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

in vec2 texcoord1;
in vec3 fNormal;
in vec3 fPos;

out vec4 fragColor;

uniform Material material;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

uniform DirectionLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

void EnableDirectionLight()
{
	vec3 fLightDir = normalize(dirLight.direction);
	vec3 fLookDir = normalize(viewPos - fPos);

	vec4 diffuseColor = dirLight.diffuse * material.diffuse;
	vec4 ambientColor = dirLight.ambient * material.ambient;
	vec4 specularColor = dirLight.specular * material.specular;

	// diffuse color
	vec4 dColor = max(dot(fLightDir, fNormal), 0.0) * diffuseColor;
	vec3 fReflect = reflect(-fLightDir, fNormal);

	// specular color
	float specularPow = pow(max(dot(fLookDir, fReflect), 0.0), 64);
	vec4 sColor = specularColor * specularPow;

	vec4 boxColor = texture(texSampler1, texcoord1);
	vec4 frameColor = texture(texSampler2, texcoord1);

	fragColor = (dColor + ambientColor) * boxColor + sColor * frameColor;
}

void EnablePointLight()
{
	vec3 fLightDir = normalize(pointLight.position - fPos);
	vec3 fLookDir = normalize(viewPos - fPos);

	float distance = length(pointLight.position - fPos);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear*distance + pointLight.quadratic*(distance*distance));

	vec4 diffuseColor = pointLight.diffuse * material.diffuse * attenuation;
	vec4 ambientColor = pointLight.ambient * material.ambient * attenuation;
	vec4 specularColor = pointLight.specular * material.specular * attenuation;

	// diffuse color
	vec4 dColor = max(dot(fLightDir, fNormal), 0.0) * diffuseColor;
	vec3 fReflect = reflect(-fLightDir, fNormal);

	// specular color
	float specularPow = pow(max(dot(fLookDir, fReflect), 0.0), 64);
	vec4 sColor = specularColor * specularPow;

	vec4 boxColor = texture(texSampler1, texcoord1);
	vec4 frameColor = texture(texSampler2, texcoord1);

	fragColor = (dColor + ambientColor) * boxColor + sColor * frameColor;
}

void EnableSpotLight()
{
	vec3 fLightDir = normalize(spotLight.position - fPos);
	vec3 fLookDir = normalize(viewPos - fPos);

	vec4 boxColor = texture(texSampler1, texcoord1);
	vec4 frameColor = texture(texSampler2, texcoord1);

	float distance = length(spotLight.position - fPos);
	float attenuation = 1.0f / (spotLight.constant + spotLight.linear*distance + spotLight.quadratic*(distance*distance));

	float theta = dot(fLightDir, spotLight.direction);

	if (theta > spotLight.outerCutOff)
	{
		float epsilon = spotLight.cutOff - spotLight.outerCutOff;
		float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

		float distance = length(spotLight.position - fPos);
		float attenuation = 1.0f / (spotLight.constant + spotLight.linear*distance + spotLight.quadratic*(distance*distance));

		vec4 diffuseColor = spotLight.diffuse * material.diffuse * intensity * attenuation;
		vec4 ambientColor = spotLight.ambient * material.ambient;
		vec4 specularColor = spotLight.specular * material.specular * intensity * attenuation;

		// diffuse color
		vec4 dColor = max(dot(fLightDir, fNormal), 0.0) * diffuseColor;
		vec3 fReflect = reflect(-fLightDir, fNormal);

		// specular color
		float specularPow = pow(max(dot(fLookDir, fReflect), 0.0), 64);
		vec4 sColor = specularColor * specularPow;

		fragColor = (dColor + ambientColor) * boxColor + sColor * frameColor;
	}
	else
	{
		// out of cutOff, only use ambient lighting
		vec4 ambientColor = spotLight.ambient * material.ambient;
		fragColor = ambientColor * boxColor + ambientColor * frameColor;
	}
}

void main()
{
	//EnableDirectionLight();
	//EnablePointLight();
	EnableSpotLight();
}
