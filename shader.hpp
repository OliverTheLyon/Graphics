
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>

#include "logger.hpp"

namespace OKengine {
	class shader{
	private:
		GLuint program;
		std::string _path;
		/**
		 * @brief The method to compile the shaders. Abstracted away from the 
		 * various constructors.
		 *
		 * @param path: the path to the shader files; must not have file 
		 * extensions.
		 **/
		GLuint compile(std::string path);

	public:
		/**
		 * @brief Constructor.
		 *
		 * @param path: the path to the shader files; must not have file 
		 * extensions.
		 **/
		shader(std::string path);
		/**
		 * @brief Move constructor. Ensures that the original location does not
		 * have a reference to the compiled program.
		 **/
		shader(shader && other)noexcept : _path(other._path), program(other.program){
			other.program = 0;
		}
		shader(const shader & other) noexcept: _path(other._path), program(compile(other._path)){
			OKengine::logger::GetInstance().log("[shader::shader] copy constructed from path: " + _path, debug_level::DEBUG);
		}

		/**
		 * @brief Destructor.
		 **/
		~shader();

		/**
		 * @brief A method to activate the shader. Needs to be called before any
		 * values inside of the shader are set (e.g. uniforms, buffers, etc)
		 **/
		bool use();
		/**
		 * @brief A method to get the idea of a named uniform.
		 *
		 * @param name: the string which names the uniform; case sensitive.
		 **/
		int getUniformID(std::string name);
		/**
		 * @brief A method to set the value of a mat4 uniform in the shader. 
		 * Note: needs only be called once for a given value of a uniform; 
		 * subsequent draw calls will use the value.
		 *
		 * @param name: the string naming the uniform; case sensitive.
		 * @param val: the value to set the uniform to.
		 **/
		bool setUniform(std::string name, glm::mat4 val);
		/**
		 * @brief A method to set a vec3 uniform value in the shader.
		 * Note: needs only be called once for a value of a uniform.
		 *
		 * @param name: the string naming the uniform; case sensitive.
		 * @param val: the value to set the uniform to.
		 **/
		bool setUniform(std::string name, glm::vec3 val);
		/**
		 * @brief A method to set a float uniform value in the shader.
		 * Note: only call once for a value of a uniform.
		 *
		 * @param name: the string naming the uniform; case sensitive.
		 * @param val: the value to set the uniform to.
		 **/
		bool setUniform(std:: string name, float val);
		/**
		 * @brief A method to set an integer uniform value in the shader.
		 * Note: only call once for a value of a uniform.
		 *
		 * @param name: the string naming the uniform; case sensitive.
		 * @param val: the value to set the uniform to.
		 **/
		bool setUniform(std::string name, int val);


	};
}
