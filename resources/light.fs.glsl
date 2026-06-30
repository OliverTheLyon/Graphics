#version 460 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec3 diffuseColour;
uniform vec3 ambientColour;
uniform float ambientStrength;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularity;

void main(){

	//ambient
	vec3 ambient = ambientStrength * ambientColour;

	// diffuse
	vec3 norm = normalize(Norm);
	vec3 light_dir = normalize(lightPos - FragPos);
	float diffuse_strength = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diffuse_strength * diffuseColour;

	//specular
	vec3 view_dir = normalize(viewPos - FragPos);
	vec3 reflection_dir = reflect(-light_dir, Norm);
	float specular_power = pow(max(dot(view_dir, reflection_dir), 0.0), 32);
	vec3 specular = specularity * specular_power * diffuseColour;

	FragColor = vec4((ambient + diffuse + specular), 1.0) * texture( tex, TexCoord );
}
