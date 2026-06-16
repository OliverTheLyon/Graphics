
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>
#include <string>


#include "shader.hpp"
#include "texture.hpp"
namespace OKengine {
	struct obj{
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> v_idxs;

		std::vector<glm::vec3> normals;
		std::vector<unsigned int> n_idxs;

		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> uv_idxs;
	};
	typedef struct obj obj;

	class mesh{

	private:
		std::vector<glm::vec3> vertex_coords;
		std::vector<GLuint> vertex_indices;

		std::vector<glm::vec2> texture_coords;
		std::vector<GLuint> texture_indices;

		std::vector<glm::vec3> normal_coords;
		std::vector<GLuint> normal_indices;

		std::unique_ptr<texture> tex;
		std::unique_ptr<shader> shader_prog;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint tbo;

		glm::mat4 model_matrix;

		bool upload();
		bool bind();

	public:
		~mesh();
		mesh(std::string path);
		mesh(std::vector<glm::vec3> vertices, std::vector<GLuint> indeces);
		mesh(std::vector<glm::vec3> vertices, std::vector<GLuint> indeces, std::string path);
		mesh(mesh && other) noexcept;


		void setShader(shader&& s);
		void setShader(std::unique_ptr<class shader> s);

		void setTexture(texture &&t);
		void setTexture(std::unique_ptr<texture> t);

		bool draw();

		bool operator==(const mesh& other) const;

		bool setUniform(std::string name, glm::mat4 val);
		bool setUniform(std::string name, glm::vec3 val);
		bool setUniform(std::string name, float val);

	};
}
