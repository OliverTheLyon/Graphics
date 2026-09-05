
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace OKengine {
	class texture{
	private:
		GLuint id;

	public:

		/**
		 * @brief Destructor.
		 **/
		~texture();
		/**
		 * @brief Constructor. Image formats are limited to jpg, png, and bmp
		 *
		 * @param path: the path to the image, including file extension.
		 **/
		texture(std::string path);
		/**
		 * @brief: move constructor.
		 **/
		texture(texture && other): id(other.id){
			other.id = 0;
		}
		texture(texture & other) = delete;
		
		/**
		 * @brief a method to bind the texture for use with the shader. 
		 *
		 * @param unit: the texture to be bound to; optional; GLuint
		 **/
		void bind(GLuint unit = GL_TEXTURE0);
		/**
		 * @brief A method to get the shader ID of the texture.
		 **/
		GLuint getID();

	};
}
