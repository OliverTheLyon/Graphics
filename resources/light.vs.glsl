#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;

out vec2 TexCoord;
out vec3 Norm;
out vec3 FragPos;

void main(){
	gl_Position = mvp * vec4(aPos, 1);
	TexCoord = texCoord;
	Norm = aNorm;
	FragPos = vec3( model * vec4(aPos, 1) );
}
