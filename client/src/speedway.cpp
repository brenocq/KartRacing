//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "speedway.hpp"

Mesh* Speedway::mesh = nullptr;
Texture* Speedway::texture = nullptr;

Speedway::Speedway(Shader* shader):
	_shader(shader)
{

}

Speedway::~Speedway()
{

}

void Speedway::draw()
{
	//---------- Draw speedway ----------//
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(0.0f) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(5.0f, 5.0f, 5.0f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();
}
