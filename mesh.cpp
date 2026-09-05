
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
#include "Utility/errors.hpp"

namespace OKengine {
	mesh::~mesh(){
		OKengine::logger::GetInstance().log("[mesh::~mesh] begin, vao: " + std::to_string(vao), debug_level::DEBUG);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &tbo);
		glDeleteBuffers(1, &nbo);
	}

	mesh::mesh(vector<glm::vec3> verts, vector<GLuint> idxs): vertex_coords(verts), vertex_indices(idxs){
		OKengine::logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs) with " + std::to_string(verts.size()) + " vertices and " + std::to_string(idxs.size()) + " indices", debug_level::DEBUG);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		model_matrix = glm::mat4(1.);
		upload();
	}

	mesh::mesh(vector<glm::vec3> verts, vector<GLuint>idxs, string path): vertex_coords(verts), vertex_indices(idxs), tex(std::make_unique<texture>(path)){
		OKengine::logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs+path) path: " + path, debug_level::DEBUG);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &tbo);
		model_matrix = glm::mat4(1.);
		upload();
	}

	mesh::mesh(mesh && other) noexcept: 
		normal_coords(other.normal_coords), normal_indices(other.normal_indices), 
		texture_coords(other.texture_coords), texture_indices(other.texture_indices), 
		vertex_coords(other.vertex_coords), vertex_indices(other.vertex_indices), 
		vao(other.vao), vbo(other.vbo), ebo(other.ebo), tbo(other.tbo),nbo(other.nbo), 
		model_matrix(other.model_matrix){
		OKengine::logger::GetInstance().log("[mesh::mesh] move constructor", debug_level::DEBUG);
		other.vao = 0;
		other.vbo = 0;
		other.ebo = 0;
		other.tbo = 0;
		other.nbo = 0;

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
		OKengine::logger::GetInstance().log("[load_obj] begin, path: " + path, debug_level::DEBUG);
		std::ifstream file(path);
		if(!file.is_open()){
			OKengine::logger::GetInstance().log("[load_obj] file not found, or is unopenable (path: " + path + ")", debug_level::ERROR);
			return false;
		}

		char line[200];
		string contents;
		vector<float> temp_vert, temp_norm, temp_uv;
		
		while(file.good()){
			float x = 0;
			float y = 0;
			float z = 0;

			file.getline(line, 200, '\n');
			contents = string(line);

			if(contents[0] == '#' || contents[0] == 0){
				continue;
			}
			else if(contents.find("vt",0) == 0){

				int start_x = contents.find_first_of(' ', 0) + 1;
				int end_x = contents.find_first_of(' ', start_x) -1;
				x = std::stof(contents.substr(start_x, end_x - start_x));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of(' ', start_y) -1;
				y = std::stof(contents.substr(start_y,end_y - start_y));

				temp_uv.push_back(x);
				temp_uv.push_back(y);
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
				temp_norm.push_back(x);
				temp_norm.push_back(y);
				temp_norm.push_back(z);
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
				temp_vert.push_back(x);
				temp_vert.push_back(y);
				temp_vert.push_back(z);
			}
			else if(contents.find("f",0) == 0){
				glm::vec3 verts(-1), texs(-1), norms(-1);
				int start_x = contents.find_first_of(' ', 0)+1;
				int end_x = contents.find_first_of('/', start_x) - 1;
				verts[0] = std::stoi(contents.substr(start_x, end_x - start_x + 1));

				int start_y = end_x + 2;
				int end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y <= end_y){
					texs[0] = std::stoi(contents.substr(start_y,end_y - start_y + 1));
				}

				int start_z = end_y + 2;
				int end_z = contents.find_first_of(' ', start_z) - 1;
				norms[0] = std::stoi(contents.substr(start_z, end_z - start_z + 1));


				start_x = contents.find_first_of(' ', end_z) + 1;
				end_x = contents.find_first_of('/', start_x) - 1;
				verts[1] = std::stoi(contents.substr(start_x, end_x - start_x + 1));

				start_y = end_x + 2;
				end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y <= end_y){
					texs[1] = std::stoi(contents.substr(start_y,end_y - start_y + 1));
				}

				start_z = end_y + 2;
				end_z = contents.find_first_of(' ', start_z) - 1;
				norms[1] = std::stoi(contents.substr(start_z, end_z - start_z + 1));


				start_x = contents.find_first_of(' ', end_z) + 1;
				end_x = contents.find_first_of('/', start_x) - 1;
				verts[2] = std::stoi(contents.substr(start_x, end_x - start_x + 1));

				start_y = end_x + 2;
				end_y = contents.find_first_of('/', start_y) - 1;
				if(start_y <= end_y){
					texs[2] = std::stoi(contents.substr(start_y,end_y - start_y + 1));
				}

				start_z = end_y + 2;
				end_z = contents.find_first_of(' ', start_z) - 1;
				norms[2] = std::stoi(contents.substr(start_z, end_z - start_z + 1));

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
		file.close();

		if(!outputs.v_idxs.empty()){
			size_t count = outputs.v_idxs.size();
			outputs.vertices.resize(count);
			for(size_t i = 0; i < count; i+= 1){
				int vi = outputs.v_idxs[i];
				outputs.vertices[i] = glm::vec3(
					temp_vert[vi * 3], temp_vert[vi * 3 + 1], temp_vert[vi * 3 + 2]
				);
				outputs.v_idxs[i] = i;
			}
		}

		if(!outputs.uv_idxs.empty()){
			size_t count = outputs.uv_idxs.size();
			outputs.uvs.resize(count);
			for(size_t i = 0; i < count; i++){
				int uvi = outputs.uv_idxs[i];
				outputs.uvs[i] = glm::vec2(
					temp_uv[uvi * 2], temp_uv[uvi * 2 + 1]
				);
				outputs.uv_idxs[i] = i;
			}
		}

		if(!outputs.n_idxs.empty()){
			size_t count = outputs.n_idxs.size();
			outputs.normals.resize(count);
			for(size_t i = 0; i < count; i++){
				int ni = outputs.n_idxs[i];
				outputs.normals[i] = glm::vec3(
					temp_norm[ni * 3], temp_norm[ni * 3 + 1], temp_norm[ni * 3 + 2]
				);
				outputs.n_idxs[i] = i;
			}
		}

		if(!outputs.vertices.empty() && !outputs.uvs.empty()){
			for(int i = 0; i < outputs.n_idxs.size() - 2; i += 1){
				glm::vec3 pos1 = outputs.vertices[outputs.v_idxs[i]];
				glm::vec3 pos2 = outputs.vertices[outputs.v_idxs[i + 1]];
				glm::vec3 pos3 = outputs.vertices[outputs.v_idxs[i + 2]];

				glm::vec2 uv1 = outputs.uvs[outputs.uv_idxs[i]];
				glm::vec2 uv2 = outputs.uvs[outputs.uv_idxs[i + 1]];
				glm::vec2 uv3 = outputs.uvs[outputs.uv_idxs[i + 2]];

				glm::vec3 e1 = pos2 - pos1;
				glm::vec3 e2 = pos3 - pos1;

				glm::vec2 duv1 = uv2 - uv1;
				glm::vec2 duv2 = uv3 - uv1;

				float fractional = 1./(duv1.x * duv2.y - duv1.y * duv2.x);
				glm::vec3 tan(
						duv2.y * e1.x - duv1.y - e2.x,
						duv2.y * e1.y - duv1.y * e2.y,
						duv2.y * e1.z - duv1.y * e2.z
						);
				tan *= fractional;
				glm::vec3 bitan(
						-duv2.x * e1.x + duv1.x * e2.x,
						-duv2.x * e1.y + duv1.x * e2.y,
						-duv2.x * e1.z + duv1.x * e2.z
						);
				bitan *= fractional;

				outputs.tangents.push_back(tan);
				outputs.bitangents.push_back(bitan);

			}
		}

		OKengine::logger::GetInstance().log("[load_obj] success, path: " + path
			+ " (vertices: " + std::to_string(outputs.vertices.size())
			+ ", uvs: " + std::to_string(outputs.uvs.size())
			+ ", normals: " + std::to_string(outputs.normals.size()) + ")", debug_level::DEBUG);
		return true;

	}


	mesh::mesh(string path){

		obj res;
		try{
		if(!load_obj(path, res)){
				OKengine::error("[mesh::mesh] failed to load mesh from .obj file " + path);
			}
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glGenBuffers(1, &tbo);
			glGenBuffers(1, &nbo);
			vertex_coords = vector<glm::vec3>(res.vertices);
			vertex_indices = vector<GLuint>(res.v_idxs);

			texture_coords = vector<glm::vec2>(res.uvs);
			texture_indices = vector<GLuint>(res.uv_idxs);

			normal_coords = vector<glm::vec3>(res.normals);
			normal_indices = vector<GLuint>(res.n_idxs);

			model_matrix = glm::mat4(1.);
			upload();

		}
		catch(const std::exception& e){
			OKengine::logger::GetInstance().log("[mesh::mesh] failed to load mesh from .obj file " + path + "\n\t" + e.what(), debug_level::ERROR);
			throw;
		}
	}


	bool mesh::upload(){
		OKengine::logger::GetInstance().log("[mesh::upload] begin (vertices: " + std::to_string(vertex_coords.size())
			+ ", indices: " + std::to_string(vertex_indices.size())
			+ ", uvs: " + std::to_string(texture_coords.size())
			+ ", normals: " + std::to_string(normal_coords.size()) + ")", debug_level::DEBUG);
		bind();
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertex_coords.size(), &vertex_coords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertex_indices.size(), &vertex_indices[0], GL_STATIC_DRAW);

		if(tex != nullptr){
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, tbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normal_coords.size(), &normal_coords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		OKengine::logger::GetInstance().log("[mesh::upload] success, returning", debug_level::DEBUG);
		return true;
	}


	bool mesh::bind(){
		OKengine::logger::GetInstance().log("[mesh::bind] begin", debug_level::DEBUG);
		if(tex != nullptr){
			OKengine::logger::GetInstance().log("[mesh::bind] setting texture", debug_level::DEBUG);
			tex->bind();
			if(shader_prog != nullptr){
				shader_prog->setUniform("mat.diffuse", 0);
				shader_prog->setUniform("mat.specular", 0);
			}
		}
		if(shader_prog != nullptr){
			shader_prog->setUniform("mat.specularity", specularity_);
		}

		OKengine::logger::GetInstance().log("[mesh::bind] binding vao (" + std::to_string(vao) + ")", debug_level::DEBUG);
		glBindVertexArray(vao);
		OKengine::logger::GetInstance().log("[mesh::bind] success. returning", debug_level::DEBUG);
		return true;
	}


	void mesh::setShader(shader & s){
		OKengine::logger::GetInstance().log("[mesh::setShader] (lvalue ref)", debug_level::DEBUG);
		shader_prog = std::make_shared<shader>(s);
	}


	void mesh::setShader(std::shared_ptr<shader> s){
		OKengine::logger::GetInstance().log("[mesh::setShader] (shared_ptr" + string(s == nullptr ? ", null" : "") + ")", debug_level::DEBUG);
		shader_prog = s;
	}


	void mesh::setTexture(texture && t){
		OKengine::logger::GetInstance().log("[mesh::setTexture] (rvalue ref)", debug_level::DEBUG);
		tex = std::make_unique<texture>(std::move(t));
		upload();
	}


	void mesh::setTexture(std::unique_ptr<texture> t){
		if(t == nullptr){
			OKengine::logger::GetInstance().log("[mesh::setTexture] attempt to set null texture, ignoring", debug_level::WARN);
			return;
		}
		OKengine::logger::GetInstance().log("[mesh::setTexture] (unique_ptr) texture id: " + std::to_string(t->getID()), debug_level::DEBUG);
		tex = std::move(t);
		upload();
	}

	bool mesh::draw(){
		OKengine::logger::GetInstance().log("[mesh::draw] begin", debug_level::DEBUG);
		if(shader_prog != nullptr){
			OKengine::logger::GetInstance().log("[mesh::draw] activating shader program", debug_level::DEBUG);
			shader_prog->use();
		}

		OKengine::logger::GetInstance().log("[mesh::draw] activating bindings.", debug_level::DEBUG);
		bind();
		OKengine::logger::GetInstance().log("[mesh::draw] activating draw call.", debug_level::DEBUG);
		glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
		OKengine::logger::GetInstance().log("[mesh::draw] completed. success. returning.", debug_level::DEBUG);
		return true;
	};

	bool mesh::draw(shader & program){
		OKengine::logger::GetInstance().log("[mesh::draw(program)] begin", debug_level::DEBUG);

		program.use();
		program.setUniform("m", model_matrix);

		if(tex != nullptr){
			tex->bind();
		}
		OKengine::logger::GetInstance().log("[mesh::draw(program)] binding vao", debug_level::DEBUG);
		glBindVertexArray(vao);
		OKengine::logger::GetInstance().log("[mesh::draw(program)] activating draw call.", debug_level::DEBUG);
		glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
		OKengine::logger::GetInstance().log("[mesh::draw(program)] completed. success. returning.", debug_level::DEBUG);
		return true;
	};

	bool mesh::draw(glm::mat4 vp, glm::vec3 viewPos){
		OKengine::logger::GetInstance().log("[mesh::draw(vp, viewPos)] begin", debug_level::DEBUG);
		if(shader_prog != nullptr){
			shader_prog->use();
			shader_prog->setUniform("mvp", vp * model_matrix);
			shader_prog->setUniform("model", model_matrix);
			shader_prog->setUniform("viewPos", viewPos);
		}

		OKengine::logger::GetInstance().log("[mesh::draw(vp, viewPos)] activating bindings.", debug_level::DEBUG);
		bind();
		OKengine::logger::GetInstance().log("[mesh::draw(vp, viewPos)] activating draw call.", debug_level::DEBUG);
		glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
		OKengine::logger::GetInstance().log("[mesh::draw(vp, viewPos)] completed. success. returning.", debug_level::DEBUG);
		return true;
	};

	void mesh::setSpecularity(float s){
		OKengine::logger::GetInstance().log("[mesh::setSpecularity] val: " + std::to_string(s), debug_level::DEBUG);
		specularity_ = s;
	}

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
		if(shader_prog == nullptr){
			OKengine::logger::GetInstance().log("[mesh::setUniform] attempt to set uniform with null shader.", debug_level::ERROR);
			return false;
		}
		return shader_prog->setUniform(name, val);
	};

	bool mesh::setUniform(string name, glm::vec3 val){
		OKengine::logger::GetInstance().log("[mesh::setUniform] (vec3) name: " + name, debug_level::DEBUG);
		if(shader_prog == nullptr){
			OKengine::logger::GetInstance().log("[mesh::setUniform] attempt to set uniform with null shader.", debug_level::ERROR);
			return false;
		}
		return shader_prog->setUniform(name, val);
	};

	bool mesh::setUniform(string name, float val){
		OKengine::logger::GetInstance().log("[mesh::setUniform] (float) name: " + name + " val: " + std::to_string(val), debug_level::DEBUG);
		if(shader_prog == nullptr){
			OKengine::logger::GetInstance().log("[mesh::setUniform] attempt to set uniform with null shader.", debug_level::ERROR);
			return false;
		}
		return shader_prog->setUniform(name, val);
	};
}


