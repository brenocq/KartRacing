//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef APPLICATION_H
#define APPLICATION_H
#include <vector>
#include <thread>
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "cubemap.hpp"
#include "cubeMesh.hpp"
#include "userInterface.hpp"
#include "client.hpp"
#include "kart.hpp"
#include "garage.hpp"
#include "speedway.hpp"

class Application
{
	public:
		Application();
		~Application();

		void run();

	private:
		void createShaders();
		void loadAssets();
		static void communicateServer(std::vector<Kart*>& karts);

		//---------- Callbacks ----------//
		void onKey(int key, int scancode, int action, int mods);
		void onMouse(double xpos, double ypos);
		void onMouseClick(int button, int action, int mods);
		void onDraw(double dt);

		//---------- Objects ----------//
		Window* _window;
		Camera* _camera;
		UserInterface* _ui;
		std::vector<Shader*> _shaders;
		std::vector<Mesh*> _meshes;
		std::vector<Texture*> _textures;
		CubeMesh* _cubeMesh;
		Cubemap* _cubemap;

		std::vector<Kart*> _karts;
		Garage* _garage;
		Speedway* _speedway;

		//---------- Game state ----------//
		Scene _scene;
		bool _freeCamera;
		bool _showFrame;

		//---------- Server thread ----------//
		std::thread* _serverThread;
};

#endif// APPLICATION_H
