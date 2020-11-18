//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "application.hpp"
#include "defines.hpp"

Application::Application():
	_scene(GAME_SCENE), _freeCamera(true)
{
	_window = new Window("Kart Racing - by Brenocq");
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onMouse = [this](double xpos, double ypos) { onMouse(xpos, ypos); };
	_window->onDraw = [this](double dt) { onDraw(dt); };
	_window->init();

	_camera = new Camera(_window->getRatio());

	createShaders();
	loadAssets();
}

Application::~Application()
{
	delete Kart::externMesh;
	delete Kart::internMesh;
	delete Kart::wheelMesh;

	for(auto& texture : Kart::externTextures)
	{
		delete texture;
		texture = nullptr;
	}

	for(auto& texture : Kart::internTextures)
	{
		delete texture;
		texture = nullptr;
	}

	for(auto& texture : Kart::wheelTextures)
	{
		delete texture;
		texture = nullptr;
	}

	for(auto& kart : _karts)
	{
		delete kart;
		kart = nullptr;
	}

	for(auto& shader : _shaders)
	{
		delete shader;
		shader = nullptr;
	}

	for(auto& mesh : _meshes)
	{
		delete mesh;
		mesh = nullptr;
	}

	for(auto& texture : _textures)
	{
		delete texture;
		texture = nullptr;
	}

	if(_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}

	if(_window != nullptr)
	{
		delete _window;
		_window = nullptr;
	}
}

void Application::createShaders()
{
	Shader* shader = new Shader();
	shader->createFromFiles("main.vert.glsl", "main.frag.glsl");
	shader->useShader();
	_shaders.push_back(shader);
}

void Application::loadAssets()
{
	Kart::externMesh = new Mesh("kart-extern.obj");
	Kart::internMesh = new Mesh("kart-intern.obj");
	Kart::wheelMesh = new Mesh("kart-wheel.obj");

	Kart::externTextures.push_back(new Texture("kart-extern0.png"));
	Kart::internTextures.push_back(new Texture("kart-intern0.png"));
	Kart::wheelTextures.push_back(new Texture("kart-wheel0.png"));
}

void Application::run()
{
	_karts.push_back(new Kart(_shaders[0]));

	_window->loop();
}

void Application::onKey(int key, int scancode, int action, int mods)
{
	if(_freeCamera)
		_camera->updateOnKey(key, scancode, action, mods);
	
	if(action == GLFW_RELEASE)
		return;
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
	}
}

void Application::onMouse(double xpos, double ypos)
{
	if(_freeCamera)
		_camera->updateOnMouse(xpos/_window->getWidth(), ypos/_window->getHeight());
}

void Application::onDraw(double dt)
{
	// Clear window
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->update(dt);

	// Use shader 0
	_shaders[0]->useShader();

	// View matrix
	glUniformMatrix4fv(_shaders[0]->getViewLocation(), 1, GL_FALSE, _camera->getView());

	// Projection matrix
    glUniformMatrix4fv(_shaders[0]->getProjectionLocation(), 1, GL_FALSE, _camera->getProjection());

	// Draw models
	for(auto kart : _karts)	
	{
		kart->draw();
	}

	// Swap buffers
	_window->swapBuffers();
}
