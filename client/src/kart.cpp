//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "kart.hpp"

Mesh* Kart::externMesh = nullptr;
Mesh* Kart::internMesh = nullptr;
Mesh* Kart::wheelMesh = nullptr;
std::vector<Texture*> Kart::externTextures = std::vector<Texture*>();
std::vector<Texture*> Kart::internTextures = std::vector<Texture*>();
std::vector<Texture*> Kart::wheelTextures = std::vector<Texture*>();

Kart::Kart(Shader* shader):
	_shader(shader)
{
	_position = {0,0,0};
	_angle = 0;

	_wheelRotation = 0;
	_wheelFrontAngle = 0;

	_internTexIndex = 0;
	_externTexIndex = 0;
	_wheelTexIndex = 0;
}

Kart::~Kart()
{

}

void Kart::draw()
{
	//---------- Draw extern ----------//
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, 0.0f, 0.0f)+_position);
	mat = glm::rotate(mat, glm::radians(180.0f) ,glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, glm::radians(_angle) ,glm::vec3(0, 1, 0));
	mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	externTextures[_externTexIndex]->bind();
	externMesh->draw();

	//---------- Draw intern ----------//
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0.0f, 0.0f, 0.0f)+_position);
	mat = glm::rotate(mat, glm::radians(180.0f) ,glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, glm::radians(_angle) ,glm::vec3(0, 1, 0));
	mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	internTextures[_internTexIndex]->bind();
	internMesh->draw();

	//---------- Draw back wheels ----------//
	for(int i=-1;i<=1;i+=2)
	{
		mat = glm::mat4(1.0f);
		mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(3.3f*i, 0.7f, 5.0f)+_position);
		mat = glm::rotate(mat, glm::radians(180.0f*(i==1)) ,glm::vec3(0, 0, 1));
		mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
		mat = glm::transpose(mat);
		glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
		
		// Bind texture/VAO and draw
		wheelTextures[_wheelTexIndex]->bind();
		wheelMesh->draw();
	}

	//---------- Draw front wheels ----------//
	for(int i=-1;i<=1;i+=2)
	{
		mat = glm::mat4(1.0f);
		mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(3.3f*i, 0.7f, -4.8f)+_position);
		mat = glm::rotate(mat, glm::radians(180.0f*(i==1)) ,glm::vec3(0, 0, 1));
		mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
		mat = glm::transpose(mat);
		glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
		
		// Bind texture/VAO and draw
		wheelTextures[_wheelTexIndex]->bind();
		wheelMesh->draw();
	}
}
