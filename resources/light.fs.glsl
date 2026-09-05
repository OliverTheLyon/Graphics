#version 460 core

out vec4 FragColor;

in VS_OUT{
	vec2 TexCoord;
	vec3 FragPos;
	vec4 FragLightPos;
	vec3 TanLightPos;
	vec3 TanViewPos;
	vec3 TanFragPos;
} fs_in;

struct directionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D shadowMap;
};
uniform directionalLight dirLight;

struct pointLight{
	vec3 position;

	// x = constant, y=linear, z=quadratic
	vec3 factors;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D shadowMap;
};
uniform pointLight ptLight;

struct spotLight{
	vec3 position;
	vec3 direction;

	float cutoff;
	float outerCutOff;

	vec3 factors;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D shadowMap;
};
uniform spotLight sptLight;

struct material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
	float specularity;
};
uniform material mat;

uniform vec3 viewPos;

subroutine vec3 light_t (vec3 viewDir, vec3 norm);
subroutine uniform light_t calcLight;

struct basics{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

float calcShadow(sampler2D shadowMap, vec3 normal, vec3 lightDirection){
	vec3 projCoords = fs_in.FragLightPos.xyz / fs_in.FragLightPos.w;
	projCoords *= 0.5;
	projCoords += 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

basics calcBasics(vec3 lightDir, vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular, vec3 norm){
	basics o;
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(
			max(
				dot(normalize(norm), halfDir),
				0.0
			   ), mat.specularity
			);
	o.ambient = ambient * vec3(texture(mat.diffuse, fs_in.TexCoord));
	o.diffuse = diffuse * diff * vec3(texture(mat.diffuse, fs_in.TexCoord));
	o.specular = specular * spec * vec3(texture(mat.specular, fs_in.TexCoord));
	return o;
}

subroutine(light_t)
	vec3 calcDirLight(vec3 viewDir, vec3 norm){
		vec3 lightDir = normalize(-dirLight.direction);
		basics res = calcBasics(lightDir, viewDir, dirLight.ambient, dirLight.diffuse, dirLight.specular, norm);
		float shadow = calcShadow(dirLight.shadowMap, norm, viewDir);
		return (res.ambient + res.diffuse + res.specular) * (1-shadow);
	}

float calcAttenuation(vec3 light, vec3 factors){
	float dist = length(light - fs_in.FragPos);
	float attenuation = 1.0/(factors.x +
			factors.y * dist +
			factors.z * (dist * dist));
	return attenuation;
}

subroutine(light_t)
	vec3 calcPointLight(vec3 viewDir, vec3 norm){
		vec3 lightDir = normalize(ptLight.position - fs_in.FragPos);

		basics res = calcBasics(lightDir, viewDir, ptLight.ambient, ptLight.diffuse, ptLight.specular, norm);

		float attenuation = calcAttenuation(ptLight.position, ptLight.factors);

		float shadow = calcShadow(ptLight.shadowMap, norm, viewDir);
		return ((res.ambient + res.diffuse + res.specular)*(1-shadow)) * attenuation ;
	}

subroutine(light_t) 
	vec3 calcSpotLight(vec3 viewDir, vec3 norm){
		vec3 lightDir = normalize(sptLight.position - fs_in.FragPos);
		basics res = calcBasics(lightDir, viewDir, sptLight.ambient, sptLight.diffuse, sptLight.specular, norm);
		float attenuation = calcAttenuation(sptLight.position, sptLight.factors);

		float theta = dot(lightDir, normalize(sptLight.direction));
		float epsilon = sptLight.cutoff - sptLight.outerCutOff;

		float intensity = clamp((theta - sptLight.outerCutOff)/epsilon, 0.0, 1.0);

		float shadow = calcShadow(sptLight.shadowMap, norm, viewDir);
		return (res.ambient + res.diffuse + res.specular) * attenuation * intensity;

	}


void main(){

	vec3 norm = texture(mat.normalMap, fs_in.TexCoord).rgb;
	vec3 col = texture(mat.diffuse, fs_in.TexCoord).rgb;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec3 lighting = calcLight(viewDir, norm);
	FragColor = vec4(lighting, 1.0);

}
