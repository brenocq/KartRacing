//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef COMMON_H
#define COMMON_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

enum Scene
{
	START_SCENE,
	GARAGE_SCENE,
	GAME_SCENE
};

//glm::mat4 getTransform(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
//{
//	glm::mat4 mat = glm::mat4(1.0f);
//	mat = glm::translate(mat, pos);
//	mat = glm::rotate(mat, glm::radians(rot.z), glm::vec3(0, 0, 1));
//	mat = glm::rotate(mat, glm::radians(rot.y), glm::vec3(0, 1, 0));
//	mat = glm::rotate(mat, glm::radians(rot.x), glm::vec3(1, 0, 0));
//	mat = glm::scale(mat, scale);
//	return glm::transpose(mat);
//}

#endif// COMMON_H
