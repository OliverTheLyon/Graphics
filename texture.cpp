
#include <string>
using std::string;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.hpp"
#include "logger.hpp"

namespace OKengine {
	texture::~texture(){
		OKengine::logger::GetInstance().log("[texture::~texture] begin", debug_level::DEBUG);
		glDeleteTextures(1, &id);
	}


	GLuint texture::getID(){
		OKengine::logger::GetInstance().log("[texture::getID] begin", debug_level::DEBUG);
		return id;
	}


	texture::texture(string path){
		OKengine::logger::GetInstance().log("[texture::texture] loading texture from path: " + path, debug_level::DEBUG);
		int width = 0;
		int height = 0;
		int chanels = 0;

		unsigned char *data = stbi_load(path.c_str(), &width, &height, &chanels, 0);

		if(!data){
			OKengine::logger::GetInstance().log("[texture::texture] failed to load image from " + path, debug_level::ERROR);
			return;
		}

		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		OKengine::logger::GetInstance().log("[texture::texture] success, path: " + path
			+ " (width: " + std::to_string(width)
			+ ", height: " + std::to_string(height)
			+ ", channels: " + std::to_string(chanels)
			+ ", id: " + std::to_string(id) + ")", debug_level::DEBUG);
	}


	void texture::bind(GLuint unit){
		OKengine::logger::GetInstance().log("[texture::bind] begin, unit: " + std::to_string(unit) + ", id: " + std::to_string(id), debug_level::DEBUG);
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}
