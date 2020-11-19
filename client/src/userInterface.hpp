//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <GL/glew.h>
#include <vector>
#include "texture.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "helpers/common.hpp"
#include "kart.hpp"

class UserInterface
{
	public:
		UserInterface(Scene* scene, Shader* shader, float ratio, Kart* kart);
		~UserInterface();

		void loadAssets();
		void draw();

		//---------- Getters and Setters ----------//
		void setScene(Scene* scene) { _scene=scene; }

		//---------- Callbacks ----------//
		void updateOnKey(int key, int scancode, int action, int mods);
		void updateOnMouse(double xpos, double ypos);
		void updateOnMouseClick(int button, int action, int mods);

	private:
		Scene* _scene;
		Shader* _shader;
		Kart* _kart;
		std::vector<Texture*> _textures;
		std::vector<Texture*> _letters;
		Mesh* _mesh;// Square mesh to show 2D images

		// Window
		float _ratio;

		// Mouse
		bool _firstMouse;
		glm::vec2 _mousePos;
		glm::vec2 _realMousePos;
		glm::vec2 _lastMousePos;

		// Keyboard
		
		// Start Scene variables
		std::string _name;
};

#endif// USER_INTERFACE_H
