//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef GARAGE_H
#define GARAGE_H
#include <vector>
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Garage
{
	public:
		Garage(Shader* shader);
		~Garage();

		void draw();

		static Mesh* mesh;
		static Texture* texture;

	private:
		Shader* _shader;
};

#endif// GARAGE_H
