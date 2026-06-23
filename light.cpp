
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


void light::setShader(string path){
	shadow_shader = std::make_shared<shader>(path);
}

void light::setShader(shared_ptr<shader> shadow){
	shadow_shader = shadow;
}

void light::setShader(const OKengine::shader & shadow){
	shadow_shader = std::make_shared<shader>(shadow);
}


void light::moveBy(vec3 delta){
	position += delta;
}

void light::tiltBy(vec3 delta){
	direction += delta;
}


void light::setPosition(vec3 pos){
	position = vec3(pos);
}

void light::setDirection(vec3 dir){
	direction = vec3(dir);
}

