
#include <glm/glm.hpp>
using glm::vec3;
#include <memory>
using std::shared_ptr;

#include "shader.hpp"
using OKengine::shader;
#include "light.hpp"

using std::string;

light::light(): type(ambientlight), position(0.,100.,0.), direction(0.){}

light::light(vec3 pos):type(pointlight), position(pos), direction(0.){}

light::light(vec3 pos, vec3 dir): type(spotlight), position(pos), direction(dir){}

light::light(light_type t, vec3 pos, vec3 dir): type(t), position(pos), direction(dir){}


void light::setShadowShader(string path){
	shadow_shader = std::make_shared<shader>(path);
}

void light::setShadowShader(shared_ptr<shader> shadow){
	shadow_shader = shadow;
}

void light::setShadowShader(const OKengine::shader & shadow){
	shadow_shader = std::make_shared<shader>(shadow);
}


void light::setShader(string path){
	regular_shader = std::make_shared<shader>(path);
	regular_shader->setUniform("lightPos", position);
}

void light::setShader(shared_ptr<shader> shdr){
	regular_shader = shdr;
	regular_shader->setUniform("lightPos", position);
}

void light::setShader(const OKengine::shader & shdr){
	regular_shader = std::make_shared<shader>(shdr);
	regular_shader->setUniform("lightPos", position);
}


void light::moveBy(vec3 delta){
	position += delta;
	if(regular_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
	if(shadow_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
}

void light::tiltBy(vec3 delta){
	direction += delta;
}


void light::setPosition(vec3 pos){
	position = vec3(pos);
	if(regular_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
	if(shadow_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
}

void light::setDirection(vec3 dir){
	direction = vec3(dir);
}



light_type light::getType(){
	return type;
}

vec3 light::getPosition(){
	return position;
}

vec3 light::getDirection(){
	return direction;
}

