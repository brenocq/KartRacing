//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <fstream>
#include <sstream>
#include <iostream>
#include "cubeMesh.hpp"
#include "helpers/log.hpp"

CubeMesh::CubeMesh()
{
	_vertices = {
		{{-1.0f,  1.0f, -1.0f}},
		{{-1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f,  1.0f, -1.0f}},
		{{-1.0f,  1.0f, -1.0f}},
                               
		{{-1.0f, -1.0f,  1.0f}},
		{{-1.0f, -1.0f, -1.0f}},
		{{-1.0f,  1.0f, -1.0f}},
		{{-1.0f,  1.0f, -1.0f}},
		{{-1.0f,  1.0f,  1.0f}},
		{{-1.0f, -1.0f,  1.0f}},
                               
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f,  1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{1.0f,  1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
                               
		{{-1.0f, -1.0f,  1.0f}},
		{{-1.0f,  1.0f,  1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{1.0f, -1.0f,  1.0f}},
		{{-1.0f, -1.0f,  1.0f}},
                               
		{{-1.0f,  1.0f, -1.0f}},
		{{1.0f,  1.0f, -1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{1.0f,  1.0f,  1.0f}},
		{{-1.0f,  1.0f,  1.0f}},
		{{-1.0f,  1.0f, -1.0f}},
                               
		{{-1.0f, -1.0f, -1.0f}},
		{{-1.0f, -1.0f,  1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{-1.0f, -1.0f,  1.0f}},
		{{1.0f, -1.0f,  1.0}}
	};

	for(auto vert : _vertices)
		_indices.push_back(_indices.size());

	setupCubeMesh();
}

CubeMesh::~CubeMesh()
{

}

void CubeMesh::setupCubeMesh()
{
	// Generate buffers
 	glGenVertexArrays(1, &_VAO);// Vertex array object
    glGenBuffers(1, &_VBO);// Vertex buffer object
    glGenBuffers(1, &_EBO);// Element buffer object
  
	// Operations on VAO
    glBindVertexArray(_VAO);

	// Add vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);  

	// Add indices to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Unbind VAO
    glBindVertexArray(0);
}

void CubeMesh::draw()
{
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
