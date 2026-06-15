
#include "screen.hpp"
#include "mesh.hpp"
#include "logger.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <memory>
#include <string>
using std::string;

namespace OKengine {
	void screen::init(int w, int h, const string& title){
		OKengine::logger::GetInstance().log("[screen::init] begin", debug_level::DEBUG);

		if(!glfwInit()){
			OKengine::logger::GetInstance().log("[screen::init]: failed to initialize glfw", debug_level::ERROR);
			glfwTerminate();
			return;
		}

		glfwWindowHint(GLFW_SAMPLES, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window.reset(glfwCreateWindow(w, h, title.c_str(), NULL, NULL));

		if(window == nullptr){
			OKengine::logger::GetInstance().log("[screen::init] Failed to create window", debug_level::ERROR);
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window.get());

		glewExperimental = GL_TRUE;
		const GLenum err = glewInit();
		if(err != GLEW_OK){
			OKengine::logger::GetInstance().log("[screen::init] unable to initialize glew: " + string(reinterpret_cast<const char*>( glewGetErrorString(err) )), debug_level::ERROR);
			glfwTerminate();
			return;
		}

		glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);
		glClearColor(1, 1, 1, 1);
		glEnable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0,0,w,h);
	}


	screen::screen(int w, int h): drawn_objects(){
		OKengine::logger::GetInstance().log("[screen::screen] constructor entered with args w: " + std::to_string(w) + " and h: " + std::to_string(h), debug_level::DEBUG);
		init(w, h, "No Title");
	}

	screen::screen(int w, int h, string title): drawn_objects(){
		OKengine::logger::GetInstance().log("[screen::screen] constructor entered with args w: " + std::to_string(w) + ", h: " + std::to_string(h) + ", and title '"+ title +"'", debug_level::DEBUG);
		init(w, h, title);
	}

	void screen::mainLoop(){
		OKengine::logger::GetInstance().log("[screen::main loop] begin", debug_level::DEBUG);
		int i = 0;
		while(!glfwWindowShouldClose(window.get())){
			i += 1;
			OKengine::logger::GetInstance().log("[screen::mainLoop] iteration: " + std::to_string(i) + " top of loop.", debug_level::DEBUG);
			enterDrawState();
			draw();
			exitDrawState();
		}
	}

	void screen::enterDrawState(){
		OKengine::logger::GetInstance().log("[screen::enterDrawState] begin", debug_level::DEBUG);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void screen::draw(){
		OKengine::logger::GetInstance().log("[screen::draw] begin", debug_level::DEBUG);
		for(int i = 0; i < drawn_objects.size(); i += 1){
			drawn_objects[i]->draw();
		}
	}

	void screen::exitDrawState(){
		OKengine::logger::GetInstance().log("[screen::exitDrawState] begin", debug_level::DEBUG);
		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	void screen::addMesh(OKengine::mesh && m){
		OKengine::logger::GetInstance().log("[screen::addMesh] begin", debug_level::DEBUG);
		drawn_objects.push_back(std::make_unique<OKengine::mesh>(std::move(m)));
	}

	void screen::removeMesh(const OKengine::mesh & m){
		OKengine::logger::GetInstance().log("[screen::removeMesh] begin", debug_level::DEBUG);
		auto pos = std::find_if(drawn_objects.begin(), drawn_objects.end(), [&](auto& ptr){return *ptr == m;});
		if(drawn_objects.end() == pos){
			OKengine::logger::GetInstance().log("[screen::removeMesh] OKengine::mesh not found", debug_level::WARN);
			return;
		}
		drawn_objects.erase(pos);
	}
}