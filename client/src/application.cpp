//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "application.h"
#include "defines.h"

Application::Application()
{
	_window = new Window("Car Racing - by Brenocq");
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onDraw = [this]() { onDraw(); };
	_window->init();

	createShaders();
}

Application::~Application()
{

}

void Application::createShaders()
{
	Shader* shader = new Shader();
	shader->createFromFiles("main.vert.glsl", "main.frag.glsl");
	shader->useShader();
	_shaders.push_back(shader);
}

void Application::run()
{
	_window->loop();
}

void Application::onKey(int key, int scancode, int action, int mods)
{
	if(action == GLFW_RELEASE)
		return;
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
	}
}

void Application::onDraw()
{
	// Clear window
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Swap buffers
	_window->swapBuffers();
}
