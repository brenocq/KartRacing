//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "garage.hpp"

Mesh* Garage::mesh = nullptr;
Texture* Garage::texture = nullptr;

Garage::Garage(Shader* shader):
	_shader(shader)
{

}

Garage::~Garage()
{

}

void Garage::draw()
{
	//---------- Draw garage ----------//
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(0.0f) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();
}
