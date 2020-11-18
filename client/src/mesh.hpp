//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <string>
#include <GL/glew.h>
#include "helpers/common.hpp"

class Mesh
{
	public:
		Mesh(std::string fileName);
		~Mesh();

		void loadFile(std::string fileName);
		void setupMesh();
		void draw();

	private:
		std::vector<std::string> splitLine(const std::string s, char delim);

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		GLuint _VAO, _VBO, _EBO;
};

#endif// MODEL_H
