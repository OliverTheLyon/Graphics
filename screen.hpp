
#pragma once
#include "scene.hpp"
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace OKengine {
	struct DestroyglfwWin{
		void operator()(GLFWwindow* ptr){
			glfwDestroyWindow(ptr);
		}
	};

	class screen{
	private:
		std::unique_ptr<GLFWwindow, DestroyglfwWin> window;

		std::unique_ptr<scene> curScene;

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

		/**
		 * @brief A method to set the current scene
		 *
		 * @param s: the new scene.
		 **/
		void setScene(scene s);
};
