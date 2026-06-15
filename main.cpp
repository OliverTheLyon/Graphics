
#include "screen.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::cross;
int main(){
	OKengine::logger::GetInstance().log("[main] program start", debug_level::INFO);

	OKengine::screen display(800, 600, "Graphics");
	
	OKengine::shader s("resources/basic");
	
	OKengine::mesh square("resources/Test.obj");
	square.setTexture(OKengine::texture("resources/texture.jpg"));
	square.setShader(std::move(s));
	square.setUniform("colour", glm::vec3(1,0,0));
	
	vec3 pos(0,0,-4);
	vec3 g_up(0,1,0);
	vec3 right = glm::normalize(cross(pos,g_up));
	vec3 up = cross(pos,right);
	camera c1(pos, glm::vec3(0,0,0), up);

	glm::mat4 mv = glm::mat4(c1.viewMatrix());
	glm::mat4 p = glm::perspective(45., 800./600.,0.1,10.); 
	square.setUniform("mvp", p * mv);
	display.addMesh(std::move(square));

	
	display.mainLoop();

	OKengine::logger::GetInstance().log("[main] program end", debug_level::INFO);
	return 0;
}
