
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
		std::shared_ptr<shader> shader_prog;

		GLuint vao; // vertex array
		GLuint vbo; // vertex buffer
		GLuint ebo; // element buffer (indices)
		GLuint tbo; // texture buffer
		GLuint nbo; // normal buffer

		glm::mat4 model_matrix;

		/**
		 * @brief A method to send the mesh data to the GPU, i.e. set the buffers.
		 **/
		bool upload();
		/**
		 * @brief A method to bind all of the buffers.
		 **/
		bool bind();

	public:
		/**
		 * @brief Destructor.
		 **/
		~mesh();
		/**
		 * @brief File Constructor. Takes in the path to a file, which must be
		 * .obj formatted, for parsing. Does not set the shader or texure.
		 *
		 * @param path: a path pointing to the .obj file containing the model 
		 * the mesh should represent.
		 **/
		mesh(std::string path);
		/**
		 * @brief a simple constructor to create a mesh in program.
		 *
		 * @param vertices: a vector of glm::vec3s holding the affine vertices 
		 * of the mesh.
		 * @param indeces: a vector of GLuints (unsigned int) which represent 
		 * the order to use the vertices in.
		 **/
		mesh(std::vector<glm::vec3> vertices, std::vector<GLuint> indeces);
		/**
		 * @brief a simple constructor for creating a shaded mesh in program.
		 *
		 * @param vertices: a vector of glm::vec3s holding the affine vertices 
		 * of the mesh.
		 * @param indeces: a vector of GLuints (unsigned int) which represent 
		 * the order to use the vertices in.
		 * @param path: the path to the shader files. Should not include file
		 * extensions. 
		 **/
		mesh(std::vector<glm::vec3> vertices, std::vector<GLuint> indeces, std::string path);
		/**
		 * @brief move constructor.
		 **/
		mesh(mesh && other) noexcept;


		/**
		 * @brief Setter for the shader; uses move semantics. Will overwrite the
		 * current shader if one exists.
		 *
		 * @param shader: the shader object the mesh should use.
		 **/
		void setShader(shader & s);
		/**
		 * @brief Setter for the shader; uses move semantics. Will overwrite the
		 * current shader if one exists.
		 *
		 * @param shader: the shader object the mesh should use.
		 **/
		void setShader(std::shared_ptr<class shader> s);

		/**
		 * @brief Setter for the texture; uses move semantics. Will overwrite the
		 * current shader if one exists
		 *
		 * @param texture: the texture object the mesh should use.
		 **/
		void setTexture(texture &&t);
		/**
		 * @brief Setter for the texture; uses move semantics. Will overwrite the
		 * current shader if one exists
		 *
		 * @param texture: the texture object the mesh should use.
		 **/
		void setTexture(std::unique_ptr<texture> t);

		/**
		 * @brief A method to setup and call the necessary functions to display
		 * the mesh.
		 **/
		bool draw();

		/**
		 * @brief comparison operator. Only cares about the affine position of the
		 * vertices, and the indices.
		 **/
		bool operator==(const mesh& other) const;

		/**
		 * @brief A passthrough method to set a shader uniform. Need only be 
		 * called once per mesh for unchanging uniforms, or each time the 
		 * uniform is changed.
		 *
		 * @param name: the name of the uniform, case sensitive.
		 * @param val: what to set the value of uniform to. 
		 * @returns: true if successful, false otherwise. Otherwise conditions 
		 * are if the shader is not set (e.g. shader == null) or if the uniform
		 * name cannot be found.
		 **/
		bool setUniform(std::string name, glm::mat4 val);
		/**
		 * @brief A passthrough method to set a shader uniform. Need only be 
		 * called once per mesh for unchanging uniforms, or each time the 
		 * uniform is changed.
		 *
		 * @param name: the name of the uniform, case sensitive.
		 * @param val: what to set the value of uniform to. 
		 * @returns: true if successful, false otherwise. Otherwise conditions 
		 * are if the shader is not set (e.g. shader == null) or if the uniform
		 * name cannot be found.
		 **/
		bool setUniform(std::string name, glm::vec3 val);
		/**
		 * @brief A passthrough method to set a shader uniform. Need only be 
		 * called once per mesh for unchanging uniforms, or each time the 
		 * uniform is changed.
		 *
		 * @param name: the name of the uniform, case sensitive.
		 * @param val: what to set the value of uniform to. 
		 * @returns: true if successful, false otherwise. Otherwise conditions 
		 * are if the shader is not set (e.g. shader == null) or if the uniform
		 * name cannot be found.
		 **/
		bool setUniform(std::string name, float val);

	};
}
