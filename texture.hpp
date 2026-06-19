
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
		 * @brief TODO
		 **/
		~texture();
		/**
		 * @brief TODO
		 **/
		texture(std::string path);
		/**
		 * @brief TODO
		 **/
		texture(texture && other): id(other.id){
			other.id = 0;
		}
		texture(texture & other) = delete;
		
		/**
		 * @brief TODO
		 **/
		void bind();
		/**
		 * @brief TODO
		 **/
		GLuint getID();

	};
}
