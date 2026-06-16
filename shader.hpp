
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>

namespace OKengine {
	class shader{
	private:
		GLuint program;
		GLuint compile(std::string path);

	public:
		shader(std::string path);
		shader(shader && other)noexcept : program(other.program){
			other.program = 0;
		}
		shader(const shader &) = delete;

		~shader();

		bool use();
		int getUniformID(std::string name);
		bool setUniform(std::string name, glm::mat4 val);
		bool setUniform(std::string name, glm::vec3 val);
		bool setUniform(std:: string name, float val);
		bool setUniform(std::string name, int val);


	};
}