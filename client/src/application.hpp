//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef APPLICATION_H
#define APPLICATION_H
#include <vector>
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "userInterface.hpp"
#include "kart.hpp"

class Application
{
	public:
		Application();
		~Application();

		void run();

	private:
		void createShaders();
		void loadAssets();

		//---------- Callbacks ----------//
		void onKey(int key, int scancode, int action, int mods);
		void onMouse(double xpos, double ypos);
		void onDraw(double dt);

		//---------- Objects ----------//
		Window* _window;
		Camera* _camera;
		UserInterface* _ui;
		std::vector<Shader*> _shaders;
		std::vector<Mesh*> _meshes;
		std::vector<Texture*> _textures;

		std::vector<Kart*> _karts;

		//---------- Game state ----------//
		Scene _scene;
		bool _freeCamera;
};

#endif// APPLICATION_H
