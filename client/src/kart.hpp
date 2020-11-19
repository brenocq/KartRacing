//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef KART_H
#define KART_H
#include <vector>
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Kart
{
	public:
		Kart(Shader* shader);
		~Kart();

		void draw();

		static Mesh* externMesh;
		static Mesh* internMesh;
		static Mesh* wheelMesh;
		static std::vector<Texture*> externTextures;
		static std::vector<Texture*> internTextures;
		static std::vector<Texture*> wheelTextures;

		//---------- Getters and Setters ----------//
		void setPosition(glm::vec3 pos) { _position=pos; }
		void setAngle(float angle) { _angle=angle; }
		void setInternTexIndex(int index) { _internTexIndex=index; }
		int getInternTexIndex() const { return _internTexIndex; }
		void setExternTexIndex(int index) { _externTexIndex=index; }
		int getExternTexIndex() const { return _externTexIndex; }
		void setWheelTexIndex(int index) { _wheelTexIndex=index; }
		int getWheelTexIndex() const { return _wheelTexIndex; }

	private:
		Shader* _shader;

		// Body
		glm::vec3 _position;
		float _angle;

		// Wheels
		float _wheelRotation;
		float _wheelFrontAngle;

		// Textures
		int _internTexIndex;
		int _externTexIndex;
		int _wheelTexIndex;
};

#endif// KART_H
