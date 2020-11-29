//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "letter.hpp"

std::vector<Texture*> Letter::letters = std::vector<Texture*>(255);
Mesh* Letter::mesh = nullptr;
