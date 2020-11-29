//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "application.hpp"
#include "defines.hpp"

Application::Application():
	_scene(START_SCENE), _freeCamera(false), _showFrame(false)
{
	_window = new Window("Kart Racing - by Brenocq");
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onMouse = [this](double xpos, double ypos) { onMouse(xpos, ypos); };
	_window->onMouseClick = [this](int button, int action, int mods) { onMouseClick(button, action, mods); };
	_window->onDraw = [this](double dt) { onDraw(dt); };
	_window->init();

	_camera = new Camera(_window->getRatio());

	createShaders();
	loadAssets();

	_karts.push_back(new Kart(_shaders[0]));
	_garage = new Garage(_shaders[0]);
	_speedway = new Speedway(_shaders[0]);

	_ui = new UserInterface(&_scene, _shaders[0], _window->getRatio(), _karts[0]);
	_client = new Client();
	_client->connectToServer();
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
	// Main shader
	Shader* shader = new Shader();
	shader->createFromFiles("main.vert.glsl", "main.frag.glsl");
	shader->useShader();
	_shaders.push_back(shader);

	// Sky shader
	Shader* shaderSky = new Shader();
	shaderSky->createFromFiles("sky.vert.glsl", "sky.frag.glsl");
	_shaders.push_back(shaderSky);
}

void Application::loadAssets()
{
	Kart::externMesh = new Mesh("kart-extern.obj");
	Kart::internMesh = new Mesh("kart-intern.obj");
	Kart::wheelMesh = new Mesh("kart-wheel.obj");

	for(int i=0;i<4;i++)
		Kart::externTextures.push_back(new Texture("kart-extern"+std::to_string(i)+".png"));
	for(int i=0;i<16;i++)
		Kart::internTextures.push_back(new Texture("kart-intern"+std::to_string(i)+".png"));
	for(int i=0;i<16;i++)
		Kart::wheelTextures.push_back(new Texture("kart-wheel"+std::to_string(i)+".png"));

	Garage::mesh = new Mesh("garage.obj");
	Garage::texture = new Texture("garage.png");
	Garage::meshWardrobe = new Mesh("wardrobe.obj");
	Garage::textureWardrobe = new Texture("wardrobe.png");

	Speedway::mesh = new Mesh("speedway0.obj");
	Speedway::texture = new Texture("speedway0.png");
	Speedway::meshGrass = new Mesh("grass.obj");
	Speedway::textureGrass = new Texture("grass.png");
	Speedway::meshGrandstand = new Mesh("grandstand.obj");
	Speedway::textureGrandstand = new Texture("grandstand.png");
	Speedway::meshPost = new Mesh("post.obj");
	Speedway::texturePost = new Texture("post.png");

	_cubeMesh = new CubeMesh();
	_cubemap = new Cubemap("sky", "jpg");
}

void Application::run()
{
	_window->loop();
}

void Application::onKey(int key, int scancode, int action, int mods)
{
	if(_freeCamera)
		_camera->updateOnKey(key, scancode, action, mods);
	_ui->updateOnKey(key, scancode, action, mods);
	_karts[0]->updateOnKey(key, scancode, action, mods);
	
	if(action == GLFW_RELEASE)
		return;
	switch(key)
	{
		case GLFW_KEY_SPACE:
			//_camera->printInfo();
			break;
		case GLFW_KEY_ENTER:
			{
				if(action == GLFW_PRESS)
				{
					if(_scene == START_SCENE)
					{
						_scene = GARAGE_SCENE;
					}
					else if(_scene == GARAGE_SCENE)
					{
						//_karts[0]->setPosition({-230,1,101});
						_karts[0]->setPosition({-231,1,59});
						_karts[0]->setAngle(0);
						_scene = GAME_SCENE;
					}
				}
			}
			break;
		case GLFW_KEY_F:
			if(_scene != START_SCENE)
			{
				if(!_showFrame)
				{
					_showFrame=true;
					glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				}
				else
				{
					_showFrame=false;
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				}
			}
			break;
		case GLFW_KEY_C:
			if(_scene != START_SCENE)
			{
				if(_scene == GARAGE_SCENE)
				{
					_camera->setPosition({-6.990914, 10.844686, 19.464249});
					_camera->setFront({0.528657, -0.351115, -0.772813});
				}

				if(!_freeCamera)
					_freeCamera=true;
				else
					_freeCamera=false;
			}
			break;
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
	}
}

void Application::onMouse(double xpos, double ypos)
{
	if(_freeCamera)
		_camera->updateOnMouse(xpos/_window->getWidth(), ypos/_window->getHeight());
	_ui->updateOnMouse(xpos/_window->getWidth(), ypos/_window->getHeight());
}

void Application::onMouseClick(int button, int action, int mods)
{
	_ui->updateOnMouseClick(button, action, mods);
}

void Application::onDraw(double dt)
{
	static double time=0;
	time+=dt;

	// Clear window
	glClearColor(0.7f,0.7f,0.7f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_camera->update(dt);

	// Use shader 0
	_shaders[0]->useShader();

	switch(_scene)
	{
		case START_SCENE:
			{
				_camera->setPosition({-6.990914, 10.844686, 19.464249});
				_camera->setFront({0.528657, -0.351115, -0.772813});
			}
			break;
		case GARAGE_SCENE:
			{
				static float kartAngle=0;
				kartAngle+=dt*15;
				if(kartAngle>=360) kartAngle-=360;

				// View matrix
				glUniformMatrix4fv(_shaders[0]->getViewLocation(), 1, GL_FALSE, _camera->getView());

				// Projection matrix
				glUniformMatrix4fv(_shaders[0]->getProjectionLocation(), 1, GL_FALSE, _camera->getProjection());

				// Draw models
				_garage->draw();
				_karts[0]->setPosition({0,3,0});
				_karts[0]->setAngle(kartAngle);
				_karts[0]->draw();
			}
			break;
		case GAME_SCENE:
			{
				if(time>0.1)
				{
					_client->updateServerState(_karts);
					time = 0;
				}

				// Update karts
				for(auto& kart : _karts)
					kart->updatePhysics(dt);
				
				// Update camera
				glm::vec3 kartPos = _karts[0]->getPosition();
				glm::vec3 kartFront = _karts[0]->getFront();
				glm::vec3 camPos = (kartPos+kartFront*-20.0f)+glm::vec3(0,10,0);
				if(!_freeCamera)
				{
					_camera->setPosition(camPos);
					_camera->setFront(glm::normalize(kartPos-camPos));
				}


				//---------- Draw models ----------//
				_shaders[0]->useShader();
				// View matrix
				glUniformMatrix4fv(_shaders[0]->getViewLocation(), 1, GL_FALSE, _camera->getView());
				// Projection matrix
				glUniformMatrix4fv(_shaders[0]->getProjectionLocation(), 1, GL_FALSE, _camera->getProjection());

				// Draw models
				_speedway->draw();
				for(auto& kart : _karts)	
					kart->draw();
				//---------- Draw sky ----------//
				glDepthFunc(GL_LEQUAL);
				_shaders[1]->useShader();
				glUniformMatrix4fv(_shaders[1]->getViewLocation(), 1, GL_FALSE, _camera->getView());
				glUniformMatrix4fv(_shaders[1]->getProjectionLocation(), 1, GL_FALSE, _camera->getProjection());
				//glUniformMatrix4fv(_shaders[1]->getModelLocation(), 1, GL_FALSE, _camera->getModel());

				// Draw cube
				_cubemap->bind();
				_cubeMesh->draw();
				glDepthFunc(GL_LESS);
			}
			break;
	}
	_ui->draw();

	// Swap buffers
	_window->swapBuffers();
}
