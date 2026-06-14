
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <iterator>
#include <memory>
#include <string>
#include <utility>
using std::string;
#include <vector>
using std::vector;

#include "mesh.hpp"
#include "shader.hpp"
#include "Logger.hpp"

mesh::~mesh(){
	Logger::GetInstance().log("[mesh::~mesh] begin", debug_level::DEBUG);
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

mesh::mesh(vector<float> verts, vector<GLuint> idxs): vertices(verts), indeces(idxs){
	Logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs) with " + std::to_string(verts.size()) + " vertices and " + std::to_string(idxs.size()) + " indices", debug_level::DEBUG);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	model_matrix = glm::mat4(1.);
	upload();
}

mesh::mesh(vector<float> verts, vector<GLuint>idxs, string path): vertices(verts), indeces(idxs), tex(std::make_unique<texture>(path)){
	Logger::GetInstance().log("[mesh::mesh] constructor (verts+idxs+path) path: " + path, debug_level::DEBUG);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	model_matrix = glm::mat4(1.);
	upload();
}

mesh::mesh(mesh && other) noexcept: vertices(other.vertices), indeces(other.indeces), vao(other.vao), vbo(other.vbo), ebo(other.ebo), model_matrix(other.model_matrix){
	Logger::GetInstance().log("[mesh::mesh] move constructor", debug_level::DEBUG);
	other.vao = 0;
	other.vbo = 0;
	other.ebo = 0;

	if(other.tex != nullptr){
		tex = std::move(other.tex);
		other.tex = nullptr;
	}

	if(other.shader_prog != nullptr){
		shader_prog = std::move(other.shader_prog);
		other.shader_prog = nullptr;
	}
	upload();

}


bool load_obj(string path, obj& outputs){
	std::ifstream file(path);
	if(!file.is_open()){
		Logger::GetInstance().log("[load_obj] file not found, or is unopenable (path: " + path + ")", debug_level::ERROR);
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

		if(contents.find("vt" == 0)){

			int start_x = contents.find_first_of(' ', 1);
			int end_x = contents.find_first_of(' ', start_x);
			x = std::stof(contents.substr(start_x, end_x - start_x));

			int start_y = end_x + 1;
			int end_y = contents.find_first_of(' ', start_y);
			y = std::stof(contents.substr(start_y,end_y));

			outputs.uvs.push_back(x);
			outputs.uvs.push_back(y);
		}
		else if(contents.find("nv") == 0){
			int start_x = contents.find_first_of(' ', 1);
			int end_x = contents.find_first_of(' ', start_x);
			x = std::stof(contents.substr(start_x, end_x - start_x));

			int start_y = end_x + 1;
			int end_y = contents.find_first_of(' ', start_y);
			y = std::stof(contents.substr(start_y,end_y));
			
			int start_z = end_y + 1;
			int end_z = contents.find_first_of(' ', start_z);
			z = std::stof(contents.substr(start_z, end_z));
			outputs.normals.push_back(x);
			outputs.normals.push_back(y);
			outputs.normals.push_back(z);
		}
		else if(contents.find("v") == 0){
			int start_x = contents.find_first_of(' ', 1);
			int end_x = contents.find_first_of(' ', start_x);
			x = std::stof(contents.substr(start_x, end_x - start_x));

			int start_y = end_x + 1;
			int end_y = contents.find_first_of(' ', start_y);
			y = std::stof(contents.substr(start_y,end_y));
			
			int start_z = end_y + 1;
			int end_z = contents.find_first_of(' ', start_z);
			z = std::stof(contents.substr(start_z, end_z));
			outputs.vertices.push_back(x);
			outputs.vertices.push_back(y);
			outputs.vertices.push_back(z);
		}
		else if(contents.find("f") == 0){
			int start_x = contents.find_first_of(' ', 1);
			int end_x = contents.find_first_of('/', start_x);
			x = std::stoi(contents.substr(start_x, end_x - start_x));

			int start_y = end_x + 1;
			int end_y = contents.find_first_of('/', start_y);
			y = std::stoi(contents.substr(start_y,end_y));
			
			int start_z = end_y + 1;
			int end_z = contents.find_first_of(' ', start_z);
			z = std::stoi(contents.substr(start_z, end_z));

			outputs.v_idxs.push_back(x);
			outputs.v_idxs.push_back(y);
			outputs.v_idxs.push_back(z);

			start_x = contents.find_first_of(' ', end_z);
			end_x = contents.find_first_of('/', start_x);
			x = std::stoi(contents.substr(start_x, end_x - start_x));

			start_y = end_x + 1;
			end_y = contents.find_first_of('/', start_y);
			y = std::stoi(contents.substr(start_y,end_y));
			
			start_z = end_y + 1;
			end_z = contents.find_first_of(' ', start_z);
			z = std::stoi(contents.substr(start_z, end_z));

			outputs.uv_idxs.push_back(x);
			outputs.uv_idxs.push_back(y);
			outputs.uv_idxs.push_back(z);

			start_x = contents.find_first_of(' ', end_z);
			end_x = contents.find_first_of('/', start_x);
			x = std::stoi(contents.substr(start_x, end_x - start_x));

			start_y = end_x + 1;
			end_y = contents.find_first_of('/', start_y);
			y = std::stoi(contents.substr(start_y,end_y));
			
			start_z = end_y + 1;
			end_z = contents.find_first_of(' ', start_z);
			z = std::stoi(contents.substr(start_z, end_z));

			outputs.uv_idxs.push_back(x);
			outputs.uv_idxs.push_back(y);
			outputs.uv_idxs.push_back(z);
		}
	}

	if(!file.eof()){
		Logger::GetInstance().log("[load_obj] reading obj file ran into an error", debug_level::ERROR);
		return false;
	}
	return true;
	
}


mesh::mesh(string path){

	obj res;
	if(!load_obj(path, res)){
		Logger::GetInstance().log("[mesh::mesh] failed to load mesh from .obj file " + path, debug_level::ERROR)
	}

}


bool mesh::upload(){
	Logger::GetInstance().log("[mesh::upload] begin", debug_level::DEBUG);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indeces.size(), &indeces[0], GL_STATIC_DRAW);

	if(tex != nullptr){
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
	}
	return true;
}


bool mesh::bind(){
	Logger::GetInstance().log("[mesh::bind] begin", debug_level::DEBUG);
	if(tex != nullptr){
		tex->bind();
	}
	glBindVertexArray(vao);

	return true;
}


void mesh::setShader(shader&& s){
	Logger::GetInstance().log("[mesh::setShader] (rvalue ref)", debug_level::DEBUG);
	shader_prog = std::make_unique<class shader>(std::move(s));
}


void mesh::setShader(std::unique_ptr<class shader> s){
	Logger::GetInstance().log("[mesh::setShader] (unique_ptr)", debug_level::DEBUG);
	shader_prog = std::move(s);
}


bool mesh::draw(){
	Logger::GetInstance().log("[mesh::draw] begin", debug_level::DEBUG);
	if(shader_prog != nullptr){
		shader_prog->use();
	}
	bind();
	glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);
	return true;
};

bool mesh::operator==(const mesh& other) const{
	Logger::GetInstance().log("[mesh::operator==] begin", debug_level::DEBUG);
	bool verts = true;
	bool inds = true;
	for(int i = 0; i < vertices.size() && verts; i +=1){
		verts = vertices[i] == other.vertices[i];
	}

	for(int i = 0; i < indeces.size() && inds; i += 1){
		inds = indeces[i] == other.indeces[i];
	}

	return verts && inds;
}


bool mesh::setUniform(string name, glm::mat4 val){
	Logger::GetInstance().log("[mesh::setUniform] (mat4) name: " + name, debug_level::DEBUG);
	return shader_prog->setUniform(name, val);
};

bool mesh::setUniform(string name, glm::vec3 val){
	Logger::GetInstance().log("[mesh::setUniform] (vec3) name: " + name, debug_level::DEBUG);
	return shader_prog->setUniform(name, val);
};

bool mesh::setUniform(string name, float val){
	Logger::GetInstance().log("[mesh::setUniform] (float) name: " + name + " val: " + std::to_string(val), debug_level::DEBUG);
	return shader_prog->setUniform(name, val);
};






