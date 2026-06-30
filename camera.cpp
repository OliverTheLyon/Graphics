
#include "camera.hpp"
#include "logger.hpp"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>

camera::camera(): position(0,0,1), target(1,0,0), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] default constructor", debug_level::DEBUG);
	view = glm::lookAt(position, target, up);
}


camera::camera(vec3 spot): position(spot), target(1,0,0), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot", debug_level::DEBUG);
	view = glm::lookAt(position, target, up);
}


camera::camera(vec3 spot, vec3 facing): position(spot), target(facing), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot and facing", debug_level::DEBUG);
	view = glm::lookAt(position, target, up);
}


camera::camera(vec3 spot, vec3 facing, vec3 upwards): position(spot), target(facing), up(upwards), view(glm::lookAt(spot, facing, upwards)){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot, facing, upwards", debug_level::DEBUG);
};


void camera::move(vec3 movement){
	OKengine::logger::GetInstance().log("[camera::move] begin", debug_level::DEBUG);
	position += movement;
	createViewMatrix();
}


void camera::place(vec3 spot, vec3 direction, vec3 upwards){
	OKengine::logger::GetInstance().log("[camera::place] begin", debug_level::DEBUG);
	position = vec3(spot);
	target = vec3(direction);
	up = vec3(upwards);
	createViewMatrix();
}


void camera::roll(vec3 axis, float radians){
	OKengine::logger::GetInstance().log("[camera::roll] begin", debug_level::DEBUG);
}


mat4 camera::viewMatrix(){
	OKengine::logger::GetInstance().log("[camera::viewMatrix] begin", debug_level::DEBUG);
	return view;
}


void camera::createViewMatrix(){
	OKengine::logger::GetInstance().log("[camera::createViewMatrix] begin", debug_level::DEBUG);
	view = glm::lookAt(position, target, up);
}

vec3 camera::getPosition(){
	return position;
}
