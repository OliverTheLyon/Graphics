
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

		/**
		 * @brief Code common to the screen constructors.
		 **/
		void init(int w, int h, const std::string& title);


	public:
		/**
		 * @brief minimal screen constructor. Will set title to 'No Title'.
		 *
		 * @param w: the width of the window. (w > 0)
		 * @param h: the height of the window. (h > 0)
		 **/
		screen(int w, int h);
		/**
		 * @brief minimal screen constructor. Will set title to 'No Title'.
		 *
		 * @param w: the width of the window. (w > 0)
		 * @param h: the height of the window. (h > 0)
		 **/
		screen(int w, int h, std::string title);

		/**
		 * @brief a method to add a mesh to the screen. Only needs to be called 
		 * once per instance of mesh. Uses move semantics.
		 *
		 * @param m: the mesh to be added to the screen.
		 **/
		void addMesh(OKengine::mesh && m);
		/**
		 * @brief A method to remove a mesh from the screen. Will remove the 
		 * first matching instance of m, so meshes of the same position will need
		 * subsequent calls. Matches exactly.
		 *
		 * @param m: the mesh to search for in the screen.
		 **/
		void removeMesh(const OKengine::mesh & m);

		/**
		 * @brief A method to prepare the screen to draw. Currently just clears
		 * the screen of any displayed objects.
		 **/
		void enterDrawState();
		/**
		 * @brief A method to have the gpu draw all objects on screen.
		 **/
		void draw();
		/**
		 * @brief A method to exit the draw state. Currently just swaps the drawn buffer
		 * with the old buffer, and polls keyboard events.
		 **/
		void exitDrawState();

		/**
		 * @brief A method to link keyboard events to functions. The method 
		 * should have parameters GLFWwindow*, int key, int sconcode, int action, 
		 * and int mods; where the key, scancode, and action can be compared 
		 * against a glfw enum. The callback should handle the logic of checking
		 * how a given key (key) is 'acted upon' (action), and any modifiers 
		 * keys (mods) or which screen (window) the event happened in.
		 *
		 * @param callback: the method to be called. 
		 **/
		void setKeyCallback(GLFWkeyfun callback);
	};
}
