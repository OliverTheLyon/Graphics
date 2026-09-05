
#include "camera.hpp"
#include "logger.hpp"

#include <cmath>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <string>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>

using OKengine::vec3_to_string;

camera::camera(): transform(vec3(0,0,1)), target(1,0,0), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] default constructor, spot: " + vec3_to_string(getPosition()) + ", facing: " + vec3_to_string(target) + ", up: " + vec3_to_string(up), debug_level::DEBUG);
	cached_ = glm::lookAt(getPosition(), getTarget(), up);
}

camera::camera(vec3 spot): transform((spot)), target(1,0,0), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot: " + vec3_to_string(spot) + ", facing: " + vec3_to_string(target) + ", up: " + vec3_to_string(up), debug_level::DEBUG);
	cached_ = glm::lookAt(getPosition(), getTarget(), up);
}

camera::camera(vec3 spot, vec3 facing): transform((spot)), target(facing), up(0,1,0){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot: " + vec3_to_string(spot) + ", facing: " + vec3_to_string(facing) + ", up: " + vec3_to_string(up), debug_level::DEBUG);
	cached_ = glm::lookAt(getPosition(), getTarget(), up);
}

camera::camera(vec3 spot, vec3 facing, vec3 upwards): transform((spot)), target(facing), up(upwards){
	OKengine::logger::GetInstance().log("[camera::camera] constructor with spot: " + vec3_to_string(spot) + ", facing: " + vec3_to_string(facing) + ", upwards: " + vec3_to_string(upwards), debug_level::DEBUG);
	cached_ = glm::lookAt(spot, spot +facing, upwards);
};


void camera::move(vec3 movement){
	OKengine::logger::GetInstance().log("[camera::move] begin, movement: " + vec3_to_string(movement), debug_level::DEBUG);
	vec3 trgt = getForward();
	vec3 n_up = getUp();
	vec3 fore_move = trgt * movement.x;
	vec3 up_move = movement.y * getUp();
	vec3 right_move = movement.z * glm::normalize(glm::cross( trgt, n_up));
	translate(fore_move + up_move + right_move);
	dirty_ = true;
	OKengine::logger::GetInstance().log("[camera::move] success, new position: " + vec3_to_string(position()), debug_level::DEBUG);
}

void camera::setMouseMode(control_mode m){
	mode_ = m;

	if(control_mode::fps == m){
		vec3 forward = glm::normalize(rotation()*target);
		yaw_ = atan2(forward.x, forward.z);
		pitch_ = asin(glm::clamp(forward.y, -1.f,1.f));
		dirty_ = true;
	}

}

void camera::look(double dx, double dy, double sensitivity){
	if(dx == 0 && dy == 0){
		return;
	}

	double yaw = dx * sensitivity;
	double pitch = -dy * sensitivity;

	vec3 c_pos = getPosition();
	vec3 c_fore = getForward();
	vec3 c_up = getUp();
	vec3 c_right = glm::normalize(glm::cross(c_fore, c_up));


	vec3 front = glm::normalize((float)cos(pitch)*((float)cos(yaw) * c_fore + (float)sin(yaw)* c_right) + (float)sin(pitch)*c_up);

	vec3 cr = glm::cross(c_fore, front);
	float len2 = dot(cr, cr);
	if(len2 < 1e-15f) return;

	vec3 axis = glm::normalize(cr);
	float angle = glm::acos(glm::clamp(  glm::dot(c_fore, front), -1.f, 1.f ));
	roll(axis, angle);
}

void camera::place(vec3 spot, vec3 direction, vec3 upwards){
	OKengine::logger::GetInstance().log("[camera::place] begin, spot: " + vec3_to_string(spot) + ", direction: " + vec3_to_string(direction) + ", upwards: " + vec3_to_string(upwards), debug_level::DEBUG);
	set_position(spot);
	target = vec3(direction);
	up = vec3(upwards);
	dirty_ = true;
}

void camera::roll(glm::vec3 axis, float angle){
	OKengine::logger::GetInstance().log("[camera::roll] begin, axis: ([" + std::to_string(axis.x) + ", " + std::to_string(axis.y) + ", " + std::to_string(axis.z) + "], angle: " + std::to_string(angle) + ")", debug_level::DEBUG);
	vec3 n_axis = glm::normalize(axis);
	glm::quat rot = rotation();
	float s_2 = sin(angle/2.);
	glm::quat delta = glm::quat(cos(angle/2.), n_axis.x * s_2, n_axis.y * s_2, n_axis.z * s_2);
	delta = glm::normalize(delta);
	set_rotation(glm::normalize(delta * rot));
	
}


const mat4 & camera::matrix(){
	if(dirty_){
		OKengine::logger::GetInstance().log("[camera::matrix] view matrix dirty, recomputing", debug_level::DEBUG);
		cached_ = glm::lookAt(position(),  getTarget(), getUp());
		dirty_ = false;
	}
	else{
		OKengine::logger::GetInstance().log("[camera::matrix] returning cached view matrix", debug_level::DEBUG);
	}
	return cached_;
}


vec3 camera::getPosition(){
	OKengine::logger::GetInstance().log("[camera::getPosition] returning " + vec3_to_string(position()), debug_level::DEBUG);
	return position();
}

vec3 camera::getTarget(){
	OKengine::logger::GetInstance().log("[camera::getTarget] returning " + vec3_to_string(position() + getForward()), debug_level::DEBUG);
	return position() + getForward();
}

vec3 camera::getForward(){
	OKengine::logger::GetInstance().log("[camera::getForward] returning " + vec3_to_string(glm::normalize(rotation() * target)), debug_level::DEBUG);
	return glm::normalize(rotation() * target);
}

vec3 camera::getUp(){
	OKengine::logger::GetInstance().log("[camera::getUp] returning " + vec3_to_string(glm::normalize(rotation() * up)), debug_level::DEBUG);
	return glm::normalize(rotation() * up);
}

