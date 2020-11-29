//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef CUBE_MESH_H
#define CUBE_MESH_H
#include <vector>
#include <string>
#include <GL/glew.h>
#include "helpers/common.hpp"

class CubeMesh
{
	public:
		CubeMesh();
		~CubeMesh();

		void setupCubeMesh();
		void draw();

	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		GLuint _VAO, _VBO, _EBO;
};

#endif// CUBE_MODEL_H
