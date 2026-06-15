
#pragma once
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "mesh.hpp"

namespace OKengine {
	struct DestroyglfwWin{
		void operator()(GLFWwindow* ptr){
			glfwDestroyWindow(ptr);
		}
	};

	class screen{
	private:
		std::unique_ptr<GLFWwindow, DestroyglfwWin> window;

		std::vector<std::unique_ptr<OKengine::mesh>> drawn_objects;

		void init(int w, int h, const std::string& title);

		void enterDrawState();
		void draw();
		void exitDrawState();

	public:
		screen(int w, int h);
		screen(int w, int h, std::string title);

		void addMesh(OKengine::mesh && m);
		void removeMesh(const OKengine::mesh & m);

		void mainLoop();
	};
}