#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec3 aTan;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 lightSpaceMat;
uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{ 
	vec2 TexCoord;
	vec3 FragPos;
	vec4 FragLightPos;
	vec3 TanLightPos;
	vec3 TanViewPos;
	vec3 TanFragPos;
} vs_out;

void main(){
	vs_out.TexCoord = texCoord;
	
	mat3 normMat = transpose(inverse(mat3(model)));
	vec3 norm = normalize( normMat * aNorm );
	vec3 fragTan = normalize(normMat * aTan);
	vec3 fragBitan = cross(norm, fragTan);
	mat3 tbn = mat3(fragTan, fragBitan, norm);

	vs_out.FragPos = vec3( model * vec4(aPos, 1) );
	vs_out.FragLightPos = lightSpaceMat * vec4(vs_out.FragPos, 1.0);
	vs_out.TanLightPos = tbn * lightPos;
	vs_out.TanViewPos = tbn * viewPos;
	vs_out.TanFragPos = tbn * vs_out.FragPos;

	gl_Position = mvp * vec4(aPos, 1);
}
