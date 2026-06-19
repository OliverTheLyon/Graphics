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
#include <glm/glm.hpp>

class camera{
	private:

		// VARIABLES
		glm::vec3 position; // affine position
		glm::vec3 target; // coordinates of the point being 'focussed' on
		glm::vec3 up; // up vector of the camera, relative to the camera's orientation

		glm::mat4 view; // a matrix computed by this class resulting from the transformations
						// applied to the class. The 'V' in the MVP matrix.

		/**
		 * @brief Does the actual work in constructing the view matrix
		 **/
		void createViewMatrix();

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
		 * @brief a method to move the camera by some amount relative to its 
		 * current position.
		 *
		 * @param movement: a vector where each dimension represents how much to
		 * add to the camera's current position in the respective dimension. e.g:
		 *	camera_pos_0: [0,0,0]
		 *	camera.move([1,2,3])
		 *	camera_pos_1: [1,2,3]
		 *	camera.move([2,3,5])
		 *	camera_pos_2: [3,4,8]
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
		void roll(glm::vec3 axis, float radians);

		/**
		 * @brief get the view matrix from the camera.
		 **/
		glm::mat4 viewMatrix();

};
