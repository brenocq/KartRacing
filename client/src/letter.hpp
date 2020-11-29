//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef LETTER_H
#define LETTER_H
#include <GL/glew.h>
#include <vector>
#include "texture.hpp"
#include "mesh.hpp"

class Letter
{
	public:
		Letter();
		~Letter();

		static std::vector<Texture*> letters;
		static Mesh* mesh;// Square mesh to show 2D images
	private:
};

#endif// LETTER_H
