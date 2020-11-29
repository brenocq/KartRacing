//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "cubemap.hpp"
#include "helpers/log.hpp"
#include "stb_image.h"

Cubemap::Cubemap(std::string cubemapName, std::string extension)
{
	std::vector<std::string> images = {"right", "left", "top", "bottom", "front", "back"};
	for(auto& image : images)
	{
		image = cubemapName+"/"+image+"."+extension;
	}
	loadCubemap(images);
}

Cubemap::~Cubemap()
{

}

void Cubemap::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texId);
}

void Cubemap::loadCubemap(std::vector<std::string> faces)
{
	// Bind cubemap
	glGenTextures(1, &_texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texId); 

	int width;
	int height;
	int channels;
	for(unsigned int i=0; i<faces.size(); i++)
	{
		// Load image
		unsigned char *data = stbi_load(("assets/textures/"+faces[i]).c_str(), &width, &height, &channels, 0); 
		if(data)
		{
			Log::info("Cubemap", faces[i] + " loaded successfully: " 
					+ std::to_string(width) + "px X "
					+ std::to_string(height) + "px X "
					+ std::to_string(channels) + ".");

			// Generate cubemap
			if(channels == 3)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if(channels == 4)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else
			{
				Log::error("Cubemap", "Strange number of channels!" + std::to_string(channels));
				exit(1);
			}
		}
		else
		{
			Log::error("Cubemap", "Failed to load " + faces[i]);

		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
