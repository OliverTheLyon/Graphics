
#include "texture.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <exception>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <string>
#include <utility>
using std::string;
#include <vector>
using std::vector;

#include "mesh.hpp"
#include "shader.hpp"
#include "logger.hpp"

namespace OKengine {
	mesh::~mesh(){
		glDeleteVertexArrays(1, &vao);
		OKengine::logger::GetInstance().log("[mesh::~mesh] begin", debug_level::DEBUG);
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &tbo);
	}

	mesh::mesh(vector<float> verts, vector<GLuint> idxs): vertex_coords(verts), vertex_indices(idxs){
		OKengine::logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs) with " + std::to_string(verts.size()) + " vertices and " + std::to_string(idxs.size()) + " indices", debug_level::DEBUG);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		model_matrix = glm::mat4(1.);
		upload();
	}

	mesh::mesh(vector<float> verts, vector<GLuint>idxs, string path): vertex_coords(verts), vertex_indices(idxs), tex(std::make_unique<texture>(path)){
		OKengine::logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs+path) path: " + path, debug_level::DEBUG);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &tbo);
		model_matrix = glm::mat4(1.);
		upload();
	}

	mesh::mesh(mesh && other) noexcept: normal_coords(other.normal_coords), normal_indices(other.normal_indices), texture_coords(other.texture_coords), texture_indices(other.texture_indices), vertex_coords(other.vertex_coords), vertex_indices(other.vertex_indices), vao(other.vao), vbo(other.vbo), ebo(other.ebo), tbo(other.tbo), model_matrix(other.model_matrix){
		OKengine::logger::GetInstance().log("[mesh::mesh] move constructor", debug_level::DEBUG);
		other.vao = 0;
		other.vbo = 0;
		other.ebo = 0;
		other.tbo = 0;

		if(other.tex != nullptr){
			tex = std::move(other.tex);
			other.tex = nullptr;
		}

		if(other.shader_prog != nullptr){
			shader_prog = std::move(other.shader_prog);
			other.shader_prog = nullptr;
		}
	}


	bool load_obj(string path, obj& outputs){
		std::ifstream file(path);
		if(!file.is_open()){
			OKengine::logger::GetInstance().log("[load_obj] file not found, or is unopenable (path: " + path + ")", debug_level::ERROR);
			return false;
		}

		char line[200];
		string contents;
		while(file.good()){
			float x = 0;
			float y = 0;
			float z = 0;

			file.getline(line, 200, '\n');
			contents = string(line);

			if(contents[0] == '#'){
				continue;
			}
			else if(contents.find("vt",0) == 0){

				int start_x = contents.find_first_of(' ', 0) + 1;;
				int end_x = contents.find_first_of(' ', start_x) -1;
				x = std::stof(contents.substr(start_x, end_x - start_x));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of(' ', start_y) -1;
				y = std::stof(contents.substr(start_y,end_y - start_y));

				outputs.uvs.push_back(x);
				outputs.uvs.push_back(y);
			}
			else if(contents.find("vn",0) == 0){
				int start_x = contents.find_first_of(' ', 0) + 1;
				int end_x = contents.find_first_of(' ', start_x) - 1;
				x = std::stof(contents.substr(start_x, end_x - start_x));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of(' ', start_y) - 1;
				y = std::stof(contents.substr(start_y,end_y - start_y));

				int start_z = end_y + 2;
				int end_z = contents.find_first_of(' ', start_z) - 1;
				z = std::stof(contents.substr(start_z, end_z - start_z));
				outputs.normals.push_back(x);
				outputs.normals.push_back(y);
				outputs.normals.push_back(z);
			}
			else if(contents.find("v",0) == 0){
				int start_x = contents.find_first_of(' ', 0) + 1;
				int end_x = contents.find_first_of(' ', start_x) - 1;
				x = std::stof(contents.substr(start_x, end_x - start_x));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of(' ', start_y) - 1;
				y = std::stof(contents.substr(start_y,end_y-start_y));

				int start_z = end_y + 2;
				int end_z = contents.find_first_of(' ', start_z) - 1;
				z = std::stof(contents.substr(start_z, end_z - start_z));
				outputs.vertices.push_back(x);
				outputs.vertices.push_back(y);
				outputs.vertices.push_back(z);
			}
			else if(contents.find("f",0) == 0){
				glm::vec3 verts(-1), texs(-1), norms(-1);
				int start_x = contents.find_first_of(' ', 0)+1;
				int end_x = contents.find_first_of('/', start_x) - 1;
				verts[0] = std::stoi(contents.substr(start_x, end_x - start_x));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y < end_y - 1){
					texs[0] = std::stoi(contents.substr(start_y,end_y - start_y));
				}

				int start_z = end_y + 2;
				int end_z = contents.find_first_of(' ', start_z) - 1;
				norms[0] = std::stoi(contents.substr(start_z, end_z - start_z));


				start_x = contents.find_first_of(' ', end_z) + 1;
				end_x = contents.find_first_of('/', start_x) - 1;
				verts[1] = std::stoi(contents.substr(start_x, end_x - start_x));

				start_y = end_x + 2;
				end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y < end_y - 1){
					texs[1] = std::stoi(contents.substr(start_y,end_y - start_y));
				}

				start_z = end_y + 2;
				end_z = contents.find_first_of(' ', start_z) - 1;
				norms[1] = std::stoi(contents.substr(start_z, end_z - start_y));


				start_x = contents.find_first_of(' ', end_z) + 1;
				end_x = contents.find_first_of('/', start_x) - 1;
				verts[2] = std::stoi(contents.substr(start_x, end_x - start_x));

				start_y = end_x + 2;
				end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y < end_y - 1){
					texs[2] = std::stoi(contents.substr(start_y,end_y - start_y));
				}

				start_z = end_y + 2;
				end_z = contents.find_first_of(' ', start_z) - 1;
				norms[2] = std::stoi(contents.substr(start_z, end_z - start_z));

				outputs.v_idxs.push_back(verts[0] - 1);
				outputs.v_idxs.push_back(verts[1] - 1);
				outputs.v_idxs.push_back(verts[2] - 1);

				if(texs != glm::vec3(-1)){
					outputs.uv_idxs.push_back(texs[0] - 1);
					outputs.uv_idxs.push_back(texs[1] - 1);
					outputs.uv_idxs.push_back(texs[2] - 1);
				}

				outputs.n_idxs.push_back(norms[0] - 1);
				outputs.n_idxs.push_back(norms[1] - 1);
				outputs.n_idxs.push_back(norms[2] - 1);
			}
			else {
				continue;
			}
		}

		if(!file.eof()){
			OKengine::logger::GetInstance().log("[load_obj] reading obj file ran into an error", debug_level::ERROR);
			return false;
		}
		return true;

	}


	mesh::mesh(string path){

		obj res;
		try{
			if(!load_obj(path, res)){
				OKengine::logger::GetInstance().log("[mesh::mesh] failed to load mesh from .obj file " + path, debug_level::ERROR);
			}
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glGenBuffers(1, &tbo);
			vertex_coords = vector<float>(res.vertices);
			vertex_indices = vector<GLuint>(res.v_idxs);

			texture_coords = vector<float>(res.uvs);
			texture_indices = vector<GLuint>(res.uv_idxs);

			normal_coords = vector<float>(res.normals);
			normal_indices = vector<GLuint>(res.n_idxs);

			bind();

		}
		catch(std::exception e){
			OKengine::logger::GetInstance().log("[mesh::mesh] failed to load mesh from .obj file " + path + "\n\t" + e.what(), debug_level::ERROR);
		}
	}


	bool mesh::upload(){
		OKengine::logger::GetInstance().log("[mesh::upload] begin", debug_level::DEBUG);
		bind();
		int stride = 0;
		if(tex != nullptr){
			stride = 5 * sizeof(GLfloat);
		}
		else{
			stride = 3 * sizeof(GLfloat);
		}

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_coords.size(), &vertex_coords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertex_indices.size(), &vertex_indices[0], GL_STATIC_DRAW);

		if(tex != nullptr){
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, tbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*)0);
		}
		return true;
	}


	bool mesh::bind(){
		OKengine::logger::GetInstance().log("[mesh::bind] begin", debug_level::DEBUG);
		if(tex != nullptr){
			tex->bind();
		}
		glBindVertexArray(vao);
		return true;
	}


	void mesh::setShader(shader&& s){
		OKengine::logger::GetInstance().log("[mesh::setShader] (rvalue ref)", debug_level::DEBUG);
		shader_prog = std::make_unique<class shader>(std::move(s));
	}


	void mesh::setShader(std::unique_ptr<class shader> s){
		OKengine::logger::GetInstance().log("[mesh::setShader] (unique_ptr)", debug_level::DEBUG);
		shader_prog = std::move(s);
	}


	void mesh::setTexture(texture && t){
		tex = std::make_unique<texture>(std::move(t));
	}


	void mesh::setTexture(std::unique_ptr<texture> t){
		tex = std::move(t);
	}

	bool mesh::draw(){
		OKengine::logger::GetInstance().log("[mesh::draw] begin", debug_level::DEBUG);
		if(shader_prog != nullptr){
			shader_prog->use();
		}
		bind();
		glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
		return true;
	};

	bool mesh::operator==(const mesh& other) const{
		OKengine::logger::GetInstance().log("[mesh::operator==] begin", debug_level::DEBUG);
		bool verts = true;
		bool inds = true;
		for(int i = 0; i < vertex_coords.size() && verts; i +=1){
			verts = vertex_coords[i] == other.vertex_coords[i];
		}

		for(int i = 0; i < vertex_coords.size() && inds; i += 1){
			inds = vertex_indices[i] == other.vertex_indices[i];
		}

		return verts && inds;
	}


	bool mesh::setUniform(string name, glm::mat4 val){
		OKengine::logger::GetInstance().log("[mesh::setUniform] (mat4) name: " + name, debug_level::DEBUG);
		return shader_prog->setUniform(name, val);
	};

	bool mesh::setUniform(string name, glm::vec3 val){
		OKengine::logger::GetInstance().log("[mesh::setUniform] (vec3) name: " + name, debug_level::DEBUG);
		return shader_prog->setUniform(name, val);
	};

	bool mesh::setUniform(string name, float val){
		OKengine::logger::GetInstance().log("[mesh::setUniform] (float) name: " + name + " val: " + std::to_string(val), debug_level::DEBUG);
		return shader_prog->setUniform(name, val);
	};
}


