
#include <glm/glm.hpp>
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
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


void light::prepareShadows(){
	glViewport(0, 0, bufferWidth, bufferHeight);

	float z_near = 1.;
	float z_far = 7.5;
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, z_near, z_far);
	glm::mat4 lightView = glm::lookAt(getPosition(), glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 1.0, 0.0));
	shadow_shader->setUniform("v", lightView);
	shadow_shader->setUniform("p", lightProj);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void light::cleanupShadows(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

