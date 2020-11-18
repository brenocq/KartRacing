//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <GL/glew.h>

class Texture
{
	public:
		Texture(std::string fileName);
		~Texture();

		void bind();

	private:
		void loadTexture(std::string fileName);

		int _width;
		int _height;
		int _channels;
		GLuint _texId;
};

#endif// TEXTURE_H
