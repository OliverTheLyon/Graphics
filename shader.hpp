
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>

namespace OKengine {
	class shader{
	private:
		GLuint program;
		/**
		 * @brief TODO
		 **/
		GLuint compile(std::string path);

	public:
		/**
		 * @brief TODO
		 **/
		shader(std::string path);
		/**
		 * @brief TODO
		 **/
		shader(shader && other)noexcept : program(other.program){
			other.program = 0;
		}
		shader(const shader &) = delete;

		/**
		 * @brief TODO
		 **/
		~shader();

		/**
		 * @brief TODO
		 **/
		bool use();
		/**
		 * @brief TODO
		 **/
		int getUniformID(std::string name);
		/**
		 * @brief TODO
		 **/
		bool setUniform(std::string name, glm::mat4 val);
		/**
		 * @brief TODO
		 **/
		bool setUniform(std::string name, glm::vec3 val);
		/**
		 * @brief TODO
		 **/
		bool setUniform(std:: string name, float val);
		/**
		 * @brief TODO
		 **/
		bool setUniform(std::string name, int val);


	};
}
