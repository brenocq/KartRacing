//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef APPLICATION_H
#define APPLICATION_H
#include <vector>
#include "window.h"
#include "shader.h"

class Application{
	public:
		Application();
		~Application();

		void run();

	private:
		void createShaders();

		//---------- Callbacks ----------//
		void onKey(int key, int scancode, int action, int mods);
		void onDraw();

		//---------- Objects ----------//
		Window* _window;
		std::vector<Shader*> _shaders;
};

#endif// APPLICATION_H
