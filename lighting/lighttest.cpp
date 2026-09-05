
#include "../screen.hpp"
#include <cmath>
#include <string>
using OKengine::screen;
#include <memory>
#include "../scene.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../texture.hpp"
#include "light.hpp"
#include "../logger.hpp"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
using glm::vec3;

int width = 900;
int height = 600;

bool running = true;
std::shared_ptr<camera> c1;
void control(GLFWwindow *w, int key, int scancode, int action, int mods){
	switch(key){
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_RELEASE){
				running = false;
			}
			break;
		case GLFW_KEY_W:
			c1->move(vec3(1,0,0));
			break;
		case GLFW_KEY_A:
			c1->move(vec3(1,0,-1));
			break;
		case GLFW_KEY_S:
			c1->move(vec3(-1,0,0));
			break;
		case GLFW_KEY_D:
			c1->move(vec3(0,0,1));
			break;
		default:
		break;
		}
}


double px = 0;
double py = 0;
double sen = 1.5e-4;
void look(GLFWwindow * window, double xpos, double ypos){

	double dx = xpos - px;
	double dy = ypos - py;
	OKengine::logger::GetInstance().log("[lighttest::look] looking with delta (" + std::to_string(dx) + ", " + std::to_string(dy) + ")", debug_level::DEBUG);
	px = xpos;
	py = ypos;

	c1->look(dx, dy, sen);
}


int main(){
	OKengine::logger::GetInstance().log("[lighttest::main] program start", debug_level::INFO);

	screen display(width,height, "Graphics");
	scene scn;
	
	OKengine::shader s("resources/light");
	
	OKengine::mesh square("resources/Test.obj");
	square.setTexture(OKengine::texture("resources/texture.jpg"));
	square.setShader(s);
	square.setSpecularity(32.0f);

	
	vec3 pos(3,0,4);
	c1 = std::make_shared<camera>(pos, glm::vec3(0,0,0) - pos, vec3(0,1,0));
	display.setKeyCallback(control);
	display.setMouseCallback(look);

	light p_light(light_type::ambientlight, vec3(30,0,20));
	p_light.setShader(s);

	OKengine::shader shad("resources/shadow");
	p_light.setShadowShader(shad);

	p_light.setAmbient(glm::vec3(0.4, 0.7, 0.4));
	p_light.setDiffuse(glm::vec3(0.,1.,0.));
	p_light.setSpecular(glm::vec3(0.25, 0.25, 0.25));

//	p_light.setCutoff(32.0f);
//	p_light.setOuterCutoff(38.0f);
	
//	p_light.setFactors(glm::vec3(1., 0.5, 0.25));

	scn.addMesh(std::move(square));
	scn.addLight(p_light);
	scn.setCamera(c1);

	display.setScene(scn);

	float delta = 0.05;
	float dx=delta, dy=delta/2., dz=delta/4.;
	int rate = 60;
	int accum = 0;
	do {
		if(accum == rate){  
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
			accum = -1;
		}
		accum += 1;
		display.enterDrawState();
		display.draw();
		display.exitDrawState();
	}while(running);

	OKengine::logger::GetInstance().log("[lighttest::main] program end", debug_level::INFO);
	return 0;
}
