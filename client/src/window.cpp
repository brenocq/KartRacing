//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "window.h"
#include "defines.h"
#include "helpers/log.h"

Window::Window(std::string name):
	_name(name)
{
}

Window::~Window()
{

}

void Window::init()
{
	Log::verbose("Window", "Initializing window...");

    // Check if glfw was initialized
    if(!glfwInit())
    {
		Log::error("Window", "GLFW initialization failed!");
        glfwTerminate();
		exit(1);
    }

    //---------- GLFW config ----------//
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_width = mode->width;
	_height = mode->height;

    //---------- Create window ----------//
    _window = glfwCreateWindow(_width, _height, _name.c_str(), glfwGetPrimaryMonitor(), nullptr);

	// Check if window was created
    if(_window == nullptr)
    {
		Log::error("Window", "Failed to create main window!");
        glfwTerminate();
        exit(1);
    }

    //---------- Config context ----------//
    // Set to draw to this window
    glfwMakeContextCurrent(_window);

    //---------- Callbacks ----------//
	glfwSetWindowUserPointer(_window, this);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(_window, keyCallback);

	//---------- GLEW ----------//
	if(glewInit())
	{
		Log::error("Window", "Glew initialization failed!");
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

void Window::loop()
{
	// Run in loop until the window is closed
	while(!glfwWindowShouldClose(_window))
	{
        glfwPollEvents();

		if(onDraw)
			onDraw();
    }
}

void Window::close()
{
	if(_window != nullptr)
	{
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}
}

float Window::getRatio()
{
	return (float)_width/_height;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}

//---------- Callbacks ----------//
void Window::keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
	Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (_this->onKey)
	{
		_this->onKey(key, scancode, action, mods);
	}
}

