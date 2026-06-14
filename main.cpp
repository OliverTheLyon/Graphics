
#include "screen.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/geometric.hpp>
#include <vector>
using std::vector;

using glm::vec3;
using glm::cross;
int main(){
	OKengine::logger::GetInstance().log("[main] program start", debug_level::INFO);

	screen display(800, 600, "Graphics");
	
	shader s("resources/basic");
	
	vector<float> vertices({
				0.5, 0.5, 0.5,		1, 0,		//0
				0.5,0.5, -0.5,		1, 0,		//1
				 0.5, -0.5, 0.5,		1, 1,	//2
				 0.5,-0.5,-0.5,		1, 1,	//3
				-0.5, 0.5,0.5,		0, 0,	//4
				-0.5,0.5,-0.5,		0,0,	//5
				 -0.5,-0.5,0.5,		0,1,		//6
				 -0.5,-0.5,-0.5,		0,1		//7
			});
	vector<GLuint> indeces({
			0,4,6,	0,6,2, //front
			0,1,4,	4,1,5, //top
			7,5,3,	3,5,1, //back
			4,5,6,	6,5,7, //left
			2,3,0,	0,3,1, //right
			3,7,5,	5,1,3  //bottom
			});
	mesh square(vertices, indeces, "resources/texture.jpg");
	square.setShader(std::move(s));
	square.setUniform("colour", glm::vec3(1,0,0));
	
	vec3 pos(-2,-2,-2);
	vec3 g_up(0,1,0);
	vec3 right = glm::normalize(cross(pos,g_up));
	vec3 up = cross(pos,right);
	camera c1(pos, glm::vec3(0,0,0), up);
	square.setUniform("mvp", c1.viewMatrix());
	display.addMesh(std::move(square));

	
	display.mainLoop();

	OKengine::logger::GetInstance().log("[main] program end", debug_level::INFO);
	return 0;
}
