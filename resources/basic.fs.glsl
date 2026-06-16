#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;
uniform vec3 colour;

void main(){

	FragColor = texture( tex, TexCoord );
}
