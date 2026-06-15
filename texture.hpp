
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace OKengine {
	class texture{
	private:
		GLuint id;

	public:

		~texture();
		texture(std::string path);
		texture(texture && other): id(other.id){
			other.id = 0;
		}
		texture(texture & other) = delete;
		
		void bind();
		GLuint getID();

	};
}