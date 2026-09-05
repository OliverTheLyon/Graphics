 /**
 * @file camera.hpp
 * @brief An abstraction of the mathematics behind rendering with a camera.
 * 
 * Part of OKengine
 * @author Kody Manastyrski
 * @date 13/06/2026
 **/

#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include "Utility/transform.hpp"

enum class control_mode {flight, fps};

class camera: private OKengine::transform{
	private:

		// VARIABLES
		glm::vec3 target; // coordinates of the point being 'focussed' on
		glm::vec3 up; // up vector of the camera, relative to the camera's orientation

		glm::mat4 view; // a matrix computed by this class resulting from the transformations
						// applied to the class. The 'V' in the MVP matrix.
		
		float yaw_ = 0.f;
		float pitch_ = 0.f;

		control_mode mode_ = control_mode::flight;


	public:

		// CONSTRUCTORS

		/**
		 * @brief Default constructor. Initializes values to opengl defaults.
		 **/
		camera();
		/**
		 * @brief constructor that places the camera at $spot position, leaving
		 * target and up to [0,0,0] and [0,1,0] respectively.
		 *
		 * @param spot: the point position relative to the origin where the camera
		 * is to be.
		 **/
		camera(glm::vec3 spot);
		/**
		 * @brief constructor placing a camera at $spot, and pointing it towards
		 * $facing.
		 * 
		 * @param spot: the point position relative to the origin where the camera
		 * is to be.
		 * @param facing: the point position relative to the origin which the
		 * camera should 'focus' on.
		 **/
		camera(glm::vec3 spot, glm::vec3 facing);
		/**
		 * @brief constructor placing a camera at $spot, pointing it towards
		 * $facing, and orienting it so that up is $upwards.
		 * 
		 * @param spot: the point position relative to the origin where the camera
		 * is to be.
		 * @param facing: the point position relative to the origin which the
		 * camera should 'focus' on.
		 * @param: the direction, relative to the camera, which will be up when
		 * viewed from the camera.
		 **/
		camera(glm::vec3 spot, glm::vec3 facing, glm::vec3 upwards);

		/**
		 * @brief method to switch between flight and fps cameras
		 *
		 * @param m: the mode to switch to; type control_mode
		 **/
		void setMouseMode(control_mode m);


		/**
		 * @brief a method implementing mouselook given basic inputs
		 *
		 * @param dx: change in cursor screen x position; type double
		 * @param dy: change in cursor screen y position; type double
		 * @param sensitivity: a scaling mouse sensitivity value; type double
		 **/
		void look(double dx, double dy, double sensitivity);

		/**
		 * @brief a method to move the camera by some amount relative to its 
		 * current position.
		 *
		 * @param movement: how much to move the camera with respect to the
		 * directions in the order forward, up, right. e.g:
		 *  camera_facing_0 along the x-axis ([1,0,0]), with up being positive 
		 *  y-axis ([0,1,0]) and right being negative z-axis ([0,0,-1])
		 *	camera_pos_0: [0,0,0] (absolute coordinates)
		 *	camera.move([1,2,3])
		 *	camera_pos_1: [1,2,-3] (absolute coordinates)
		 *	**change camera facing to forward along y-axis ([0,1,0]), up along 
		 *	negative x-axis ([-1,0,0]) and right along the negative z-axis ([0,0,-1])
		 *	camera.move([2,3,5])
		 *	camera_pos_2: [-1,4,-8] (absolute coordinates)
		 **/
		void move(glm::vec3 movement);
		/**
		 * @brief a method to pick up the camera and put it down at a specific 
		 * position, facing a given direction, with a given upwards direction.
		 *
		 * @param spot:
		 * @param direction:
		 * @param upwards:
		 **/
		void place(glm::vec3 spot, glm::vec3 direction, glm::vec3 upwards);

		/**
		 * @brief a method to roll the camera at it's current position, along a 
		 * given axis by $radians.
		 **/

		void roll(glm::vec3 axis, float angle);
		/**
		 * @brief get the view matrix from the camera.
		 **/
		const glm::mat4 & matrix();

		/**
		 * @brief getter for position
		 **/
		glm::vec3 getPosition();

		glm::vec3 getForward();
		/**
		 * @brief getter for target
		 **/
		glm::vec3 getTarget();

		/**
		 * @brief getter for up
		 **/
		glm::vec3 getUp();

};
