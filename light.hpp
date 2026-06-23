
#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "shader.hpp"

enum light_type {spotlight, ambientlight, pointlight};

class light{

	private:
		glm::vec3 position; // (x,y,z) position
		glm::vec3 direction; // (x,y,z) center of the cone of light. Optional
		
		std::shared_ptr<OKengine::shader> shadow_shader; // ponter to the shader
														 // for doing shadows.

		light_type type; // type of light

	public:

		/**
		 * @brief Default constructor, treated as ambientlight at high noon.
		 **/
		light();
		/**
		 * @brief Constructor for a pointlight at a given position.
		 *
		 * @param pos: the position of the point light (x,y,z)
		 **/
		light(glm::vec3 pos);
		/**
		 * @brief Constructor for a spotlight at a given position, pointed at a given direction
		 *
		 * @param pos: the position of the spotlight (x,y,z)
		 * @param dir: the direction the spotlight is pointing in.
		 **/
		light(glm::vec3 pos, glm::vec3 dir);

		/**
		 * @brief A general constructor which assumes nothing about the type of light.
		 *
		 * @param t: the type of light (light_type enum defined above).
		 * @param pos: [optional] position for the light source, defaults to [1,0,0]
		 * @param dir: [optional] direction for the light, defaults to [1,0,0]
		 **/ 
		light(light_type t, glm::vec3 pos=glm::vec3(1.0,0,0), glm::vec3 dir=glm::vec3(1.0,0,0));

		/**
		 * @brief Setter for the shadow shader
		 *
		 * @param path: path to the shadow shader, excluding file extension.
		 **/
		void setShader(std::string path);

		/**
		 * @brief Setter for the shadow shader.
		 *
		 * @param shadow: a precompiled shader program for calculating the shadows.
		 **/
		void setShader(std::shared_ptr<OKengine::shader> shadow);

		/**
		 * @brief Setter for the shadow shader
		 *
		 * @param shadow: a referennce to the new shadow shader.
		 **/
		void setShader(const OKengine::shader & shadow);

		/**
		 * @brief mutator for light position
		 *
		 * @param delta: how much to move by in each axis, i.e. pos + (x,y,z)
		 **/
		void moveBy(glm::vec3 delta);

		/**
		 * @brief mutator for the light direction
		 *
		 * @param delta: how much to move the focal point of the cone of light by,
		 * i.e. dir + (x,y,z)
		 **/
		 void tiltBy(glm::vec3 delta);

		 /**
		  * @brief setter for the light position
		  *
		  * @param loc: the (x,y,z) coordinates to set the light's position to.
		  **/
		 void setPosition(glm::vec3 pos);

		 /**
		  * @brief setter for the light direction
		  *
		  * @param loc: the (x,y,z) coordinates to set the focal point of the 
		  * cone of light to.
		  **/
		 void setDirection(glm::vec3 dir);
};
