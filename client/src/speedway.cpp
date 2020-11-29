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
Mesh* Speedway::meshGrass = nullptr;
Texture* Speedway::textureGrass = nullptr;
Mesh* Speedway::meshGrandstand = nullptr;
Texture* Speedway::textureGrandstand = nullptr;
Mesh* Speedway::meshPost = nullptr;
Texture* Speedway::texturePost = nullptr;

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
	mat = glm::scale(mat, glm::vec3(5.0f, 3.0f, 5.0f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();

	//---------- Draw grass ----------//
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, -1.5f, 0.0f));
	mat = glm::rotate(mat, glm::radians(0.0f) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(5.0f, 5.0f, 5.0f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));

	// Bind texture/VAO and draw
	textureGrass->bind();
	meshGrass->draw();

	//---------- Draw grandstand ----------//
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, -0.2f, 0.0f));
	mat = glm::rotate(mat, glm::radians(0.0f) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(5.0f, 0.5f, 5.0f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));

	// Bind texture/VAO and draw
	textureGrandstand->bind();
	meshGrandstand->draw();

	//---------- Draw post ----------//
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(-215.0f, -0.2f, 0.0f));
	mat = glm::rotate(mat, glm::radians(0.0f) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(.5,.2,.5));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));

	// Bind texture/VAO and draw
	texturePost->bind();
	meshPost->draw();
}
