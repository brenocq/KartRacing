//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <string>
#include <vector>
#include <GL/glew.h>

class Cubemap
{
	public:
		Cubemap(std::string cubemapName, std::string extension);
		~Cubemap();

		void bind();

	private:
		void loadCubemap(std::vector<std::string> faces);

		GLuint _texId;
};

#endif// CUBEMAP_H
