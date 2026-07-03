
#include "screen.hpp"
#include <iostream>
using OKengine::screen;
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

bool running = true;

void stop(GLFWwindow *w, int key, int scancode, int action, int mods){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
		running = false;
	}
}

int main(){
	OKengine::logger::GetInstance().log("[main] program start", debug_level::INFO);

	screen display(800, 600, "Graphics");
	
	OKengine::shader s("resources/light");
	
	OKengine::mesh square("resources/Test.obj");
	square.setTexture(OKengine::texture("resources/texture.jpg"));
	square.setShader(s);
	
	vec3 pos(0,0,-4);
	camera c1(-pos, glm::vec3(0,0,0), vec3(0,1,0));

	glm::mat4 mv = glm::mat4(c1.viewMatrix());
	glm::mat4 p = glm::perspective(45., 800./600.,0.1,10.); 
	square.setUniform("mvp", p * mv);
	square.setUniform("model", glm::mat4(1.0));

	square.setUniform("diffuseColour", glm::vec3(0.,1.,0.));
	square.setUniform("ambientColour", glm::vec3(0.4, 0.7, 0.4));
	square.setUniform("ambientStrength", 0.5);
	square.setUniform("viewPos", c1.getPosition());
	square.setUniform("specularity", 0.5);

	display.setKeyCallback(stop);
	light p_light(vec3(3,0,2),vec3(0,0,0));
	
	p_light.setShader(s);

	display.addMesh(std::move(square));

	float delta = 0.02;
	float dx=delta, dy=delta/2., dz=delta/4.;
	do {
		vec3 l_pos = p_light.getPosition();
		float x = l_pos.x;
		float y = l_pos.y;
		float z = l_pos.z;


		if(x > 3){
			dx = -delta;
		}else if(x < -3){
			dx = delta;
		}

		if(y > 3){
			dy = -delta/2.;
		}else if(y < -3){
			dy = delta/2.;
		}

		if(z > 3){
			dz = -delta/4.;
		}else if(z < -3 ){
			dz = delta/4.;
		}

		p_light.moveBy(vec3(dx, dy, dz));
		
		display.enterDrawState();
		display.draw();
		display.exitDrawState();
	}while(running);

	OKengine::logger::GetInstance().log("[main] program end", debug_level::INFO);
	return 0;
}
