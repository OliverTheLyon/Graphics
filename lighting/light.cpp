
#include <glm/glm.hpp>
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
using std::shared_ptr;

#include "../shader.hpp"
using OKengine::shader;
#include "../logger.hpp"
#include "light.hpp"

using std::string;

light::light(): light(ambientlight, vec3(1.,0.,0.), vec3(1.,0.,0.)){
	OKengine::logger::GetInstance().log("[light::light] default constructor", debug_level::DEBUG);
}

light::light(vec3 pos):light(pointlight, pos, vec3(1.,0.,0.)){
	OKengine::logger::GetInstance().log("[light::light] pointlight constructor, position: " + OKengine::vec3_to_string(pos), debug_level::DEBUG);
}

light::light(vec3 pos, vec3 dir): light(spotlight, pos, dir){
	OKengine::logger::GetInstance().log("[light::light] spotlight constructor, position: " + OKengine::vec3_to_string(pos) + ", direction: " + OKengine::vec3_to_string(dir), debug_level::DEBUG);
}

light::light(light_type t, vec3 pos, vec3 dir): type(t), position(pos), direction(dir){
	OKengine::logger::GetInstance().log("[light::light] general constructor, type: " + std::to_string(static_cast<int>(t)) + ", position: " + OKengine::vec3_to_string(pos) + ", direction: " + OKengine::vec3_to_string(dir), debug_level::DEBUG);
}

bool light::operator==(const light& other){
	bool equal = type == other.type && position == other.position && direction == other.direction;
	OKengine::logger::GetInstance().log("[light::operator==] returning " + string(equal ? "true" : "false"), debug_level::DEBUG);
	return equal;
}


void light::setShadowShader(string path){
	OKengine::logger::GetInstance().log("[light::setShadowShader] (path) path: " + path + ", buffer " + std::to_string(bufferWidth) + "x" + std::to_string(bufferHeight), debug_level::DEBUG);
	shadow_shader = std::make_shared<shader>(path);
	
	if(depthFBO != 0){
		glDeleteBuffers(1, &depthFBO);
	}
	glGenFramebuffers(1, &depthFBO);
	
	if(depthTex != 0){
		glDeleteTextures(1, &depthTex);
	}
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

}

void light::setShadowShader(shared_ptr<shader> shadow){
	OKengine::logger::GetInstance().log("[light::setShadowShader] (shared_ptr" + string(shadow == nullptr ? ", null" : "") + "), buffer " + std::to_string(bufferWidth) + "x" + std::to_string(bufferHeight), debug_level::DEBUG);
	shadow_shader = shadow;
	if(depthFBO != 0){
		glDeleteBuffers(1, &depthFBO);
	}
	glGenFramebuffers(1, &depthFBO);
	
	if(depthTex != 0){
		glDeleteTextures(1, &depthTex);
	}
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void light::setShadowShader(const OKengine::shader & shadow){
	OKengine::logger::GetInstance().log("[light::setShadowShader] (const ref), buffer " + std::to_string(bufferWidth) + "x" + std::to_string(bufferHeight), debug_level::DEBUG);
	shadow_shader = std::make_shared<shader>(shadow);
	if(depthFBO != 0){
		glDeleteBuffers(1, &depthFBO);
	}
	glGenFramebuffers(1, &depthFBO);
	
	if(depthTex != 0){
		glDeleteTextures(1, &depthTex);
	}
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void light::setShader(string path){
	OKengine::logger::GetInstance().log("[light::setShader] (path) path: " + path, debug_level::DEBUG);
	regular_shader = std::make_shared<shader>(path);
	regular_shader->setUniform("lightPos", position);
}

void light::setShader(shared_ptr<shader> shdr){
	OKengine::logger::GetInstance().log("[light::setShader] (shared_ptr" + string(shdr == nullptr ? ", null" : "") + ")", debug_level::DEBUG);
	regular_shader = shdr;
	if(regular_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
}

void light::setShader(const OKengine::shader & shdr){
	OKengine::logger::GetInstance().log("[light::setShader] (const ref)", debug_level::DEBUG);
	regular_shader = std::make_shared<shader>(shdr);
	regular_shader->setUniform("lightPos", position);
}


void light::moveBy(vec3 delta){
	OKengine::logger::GetInstance().log("[light::moveBy] begin, delta: " + OKengine::vec3_to_string(delta), debug_level::DEBUG);
	position += delta;
	dirty_ = true;
	if(regular_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
}

void light::tiltBy(vec3 delta){
	OKengine::logger::GetInstance().log("[light::tiltBy] begin, delta: " + OKengine::vec3_to_string(delta), debug_level::DEBUG);
	direction += delta;
	dirty_ = true;
}


void light::setPosition(vec3 pos){
	OKengine::logger::GetInstance().log("[light::setPosition] begin, new position: " + OKengine::vec3_to_string(pos), debug_level::DEBUG);
	position = vec3(pos);
	dirty_ = true;
	if(regular_shader != nullptr){
		regular_shader->setUniform("lightPos", position);
	}
}

void light::setDirection(vec3 dir){
	OKengine::logger::GetInstance().log("[light::setDirection] begin, new direction: " + OKengine::vec3_to_string(dir), debug_level::DEBUG);
	direction = vec3(dir);
	dirty_ = true;
}



light_type light::getType(){
	OKengine::logger::GetInstance().log("[light::getType] returning type: " + std::to_string(static_cast<int>(type)), debug_level::DEBUG);
	return type;
}

vec3 light::getPosition(){
	OKengine::logger::GetInstance().log("[light::getPosition] returning " + OKengine::vec3_to_string(position), debug_level::DEBUG);
	return position;
}

vec3 light::getDirection(){
	OKengine::logger::GetInstance().log("[light::getDirection] returning " + OKengine::vec3_to_string(direction), debug_level::DEBUG);
	return direction;
}

bool light::hasShadowShader(){
	bool has = nullptr != shadow_shader;
	OKengine::logger::GetInstance().log("[light::hasShadowShader] returning " + string(has ? "true" : "false"), debug_level::DEBUG);
	return has;
}

OKengine::shader& light::shadowShader(){
	if(shadow_shader == nullptr){
		OKengine::logger::GetInstance().log("[light::shadowShader] shadow shader requested but not set, dereferencing null pointer", debug_level::ERROR);
	}
	return *shadow_shader;
}

void light::bind(){
	OKengine::logger::GetInstance().log("[light::bind] begin, type: " + std::to_string(static_cast<int>(type)) + ", position: " + OKengine::vec3_to_string(position) + ", direction: " + OKengine::vec3_to_string(direction), debug_level::DEBUG);
	if(dirty_){
		recomputeMatrices();
	}
	if(regular_shader == nullptr){
		OKengine::logger::GetInstance().log("[light::bind] attempt to bind light with null regular shader, aborting", debug_level::ERROR);
		return;
	}
	string uniName;
	switch(type){
		case light_type::ambientlight:
			regular_shader->setSubroutine("calcLight", GL_FRAGMENT_SHADER, "calcDirLight");
			uniName = "dirLight";
			break;
		case light_type::pointlight:
			regular_shader->setSubroutine("calcLight", GL_FRAGMENT_SHADER, "calcPointLight");
			uniName = "ptLight";
			break;
		case light_type::spotlight:
			regular_shader->setSubroutine("calcLight", GL_FRAGMENT_SHADER, "calcSpotLight");
			uniName = "sptLight";
			break;
	}

	if(type != pointlight){
		regular_shader->setUniform(uniName+".direction", direction);
	}
	if(type != ambientlight){
		regular_shader->setUniform(uniName+".position", position);
		regular_shader->setUniform(uniName+".factors", factors);
	}
	if(type == spotlight){
		regular_shader->setUniform(uniName+".cutoff", glm::cos(glm::radians(innerCutoff)));
		regular_shader->setUniform(uniName+".outerCutOff", glm::cos(glm::radians(outerCutoff)));
	}
	regular_shader->setUniform(uniName+".ambient", amb);
	regular_shader->setUniform(uniName+".diffuse", dif);
	regular_shader->setUniform(uniName+".specular", spec);
	regular_shader->setUniform(uniName+".shadowMap", depthTex);
	regular_shader->setUniform("lightSpaceMat", view*proj);
	OKengine::logger::GetInstance().log("[light::bind] success, uniform block: " + uniName, debug_level::DEBUG);
}

void light::prepareShadows(){
	if(shadow_shader == nullptr){
		OKengine::logger::GetInstance().log("[light::prepareShadows] attempting shadow shading with null shadow shader. Stopping that.", debug_level::WARN);
		return;
	}
	OKengine::logger::GetInstance().log("[light::prepareShadows] begin, buffer " + std::to_string(bufferWidth) + "x" + std::to_string(bufferHeight) + ", depth fbo: " + std::to_string(depthFBO), debug_level::DEBUG);
	if(dirty_){
		recomputeMatrices();
	}
	glViewport(0, 0, bufferWidth, bufferHeight);

	shadow_shader->setUniform("v", view);
	shadow_shader->setUniform("p", proj);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void light::cleanupShadows(){
	if(shadow_shader == nullptr){		
		return;
	}
	OKengine::logger::GetInstance().log("[light::cleanupShadows] begin, unbinding shadow framebuffer", debug_level::DEBUG);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void light::setAmbient(glm::vec3 a){
	OKengine::logger::GetInstance().log("[light::setAmbient] new ambient: " + OKengine::vec3_to_string(a), debug_level::DEBUG);
	amb = a;
}

void light::setDiffuse(glm::vec3 d){
	OKengine::logger::GetInstance().log("[light::setDiffuse] new diffuse: " + OKengine::vec3_to_string(d), debug_level::DEBUG);
	dif = d;
}

void light::setSpecular(glm::vec3 s){
	OKengine::logger::GetInstance().log("[light::setSpecular] new specular: " + OKengine::vec3_to_string(s), debug_level::DEBUG);
	spec = s;
}

void light::setFactors(glm::vec3 f){
	OKengine::logger::GetInstance().log("[light::setFactors] new factors: " + OKengine::vec3_to_string(f), debug_level::DEBUG);
	factors = f;
}

void light::setCutoff(float c){
	OKengine::logger::GetInstance().log("[light::setCutoff] new cutoff: " + std::to_string(c), debug_level::DEBUG);
	innerCutoff = c;
}

void light::setOuterCutoff(float c){
	OKengine::logger::GetInstance().log("[light::setOuterCutoff] new outer cutoff: " + std::to_string(c), debug_level::DEBUG);
	outerCutoff = c;
}

void light::recomputeMatrices(){
	float z_near = 1.;
	float z_far = 7.5;
	proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, z_near, z_far);
	view = glm::lookAt(getPosition(), glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 1.0, 0.0));
}

