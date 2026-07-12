#version 460 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in vec3 FragPos;

//#define dirCount 4
struct directionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
// uniform directionalLight dirLights[dircount]

//#define ptCount 4
struct pointLight{
	vec3 position;

	// x = constant, y=linear, z=quadratic
	vec3 factors;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//uniform pointLight ptLights[ptCount];

#define sptCount 1
struct spotLight{
	vec3 position;
	vec3 direction;

	float cutoff;
	float outerCutOff;

	vec3 factors;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform spotLight sptLights[sptCount];

struct material{
	sampler2D diffuse;
	sampler2D specular;
	float specularity;
};
uniform material mat;

uniform vec3 viewPos;

struct basics{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
basics calcBasics(vec3 lightDir, vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular){
	basics o;
	float diff = max(dot(Norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, Norm);
	float spec = pow(
			max(
				dot(viewDir, reflectDir),
				0.0
			   ), mat.specularity
			);
	o.ambient = ambient * vec3(texture(mat.diffuse, TexCoord));
	o.diffuse = diffuse * diff * vec3(texture(mat.diffuse, TexCoord));
	o.specular = specular * spec * vec3(texture(mat.specular, TexCoord));
	return o;
}

vec3 calcDirLight(directionalLight light, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	basics res = calcBasics(lightDir, viewDir, light.ambient, light.diffuse, light.specular);
	return res.ambient + res.diffuse + res.specular;
}

float calcAttenuation(vec3 light, vec3 factors){
	float dist = length(light - FragPos);
	float attenuation = 1.0/(factors.x +
			factors.y * dist +
			factors.z * (dist * dist));
	return attenuation;
}

vec3 calcPointLight(pointLight light, vec3 viewDir){
	vec3 lightDir = normalize(light.position - FragPos);

	basics res = calcBasics(lightDir, viewDir, light.ambient, light.diffuse, light.specular);

	float attenuation = calcAttenuation(light.position, light.factors);

	return (res.ambient + res.diffuse + res.specular) * attenuation;
}

vec3 calcSpotLight(spotLight light, vec3 viewDir){
	vec3 lightDir = normalize(light.position - FragPos);
	basics res = calcBasics(lightDir, viewDir, light.ambient, light.diffuse, light.specular);
	float attenuation = calcAttenuation(light.position, light.factors);

	float theta = dot(lightDir, normalize(light.direction));
	float epsilon = light.cutoff - light.outerCutOff;

	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

	return (res.ambient + res.diffuse + res.specular) * attenuation * intensity;

}

void main(){

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 c = vec3(0.0);
//	for(int i = 0; i < dirCount; i += 1){
//		c += calcDirLight(dirLights[i]);
//	}
//
//	for(int i = 0; i < ptCount; i += 1){
//		c += calcPointLight(ptLights[i]);
//	}

	for(int i = 0; i < sptCount; i += 1){
		c += calcSpotLight(sptLights[i], viewDir);
	}

	FragColor = vec4(c, 1.0);

}
