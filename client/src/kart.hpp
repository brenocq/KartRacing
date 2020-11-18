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

	private:
		Shader* _shader;
};

#endif// KART_H
