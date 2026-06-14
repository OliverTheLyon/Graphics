
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <string>


#include "shader.hpp"
#include "texture.hpp"

struct obj{
	std::vector<float> vertices;
	std::vector<unsigned int> v_idxs;
	
	std::vector<float> normals;
	std::vector<unsigned int> n_idxs;

	std::vector<float> uvs;
	std::vector<unsigned int> uv_idxs;
};
typedef struct obj obj;

class mesh{

	private:
		std::vector<float> vertices;
		std::vector<GLuint> indeces;

		std::unique_ptr<texture> tex;
		std::unique_ptr<shader> shader_prog;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;

		glm::mat4 model_matrix;

		bool upload();
		bool bind();

	public:
		~mesh();
		mesh(std::string path);
		mesh(std::vector<float> vertices, std::vector<GLuint> indeces);
		mesh(std::vector<float> vertices, std::vector<GLuint> indeces, std::string path);
		mesh(mesh && other) noexcept;


		void setShader(shader&& s);
		void setShader(std::unique_ptr<class shader> s);

		bool draw();

		bool operator==(const mesh& other) const;

		bool setUniform(std::string name, glm::mat4 val);
		bool setUniform(std::string name, glm::vec3 val);
		bool setUniform(std::string name, float val);

};
