//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "userInterface.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

UserInterface::UserInterface(Scene* scene, Shader* shader, float ratio):
	_scene(scene), _shader(shader), _ratio(ratio)
{
	loadAssets();
	_mousePos = {0,0};
	_firstMouse = true;
	_name = "";
}

UserInterface::~UserInterface()
{

}

void UserInterface::loadAssets()
{
	_mesh = new Mesh("square.obj");
	_textures.push_back(new Texture("start.png"));//0
	_textures.push_back(new Texture("cursor.png"));//1

	// Load letters
	_letters.resize(255);
	for(int i=0; i<=9; i++)
		_letters['0'+i] = new Texture(std::to_string(i)+".png");
	for(char i='A'; i<='Z'; i++)
	{
		std::string s = "x.png";s[0]=i;
		_letters[i] = new Texture(s);
	}
}

void UserInterface::draw()
{
	// View matrix
	glUniformMatrix4fv(_shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	// Projection matrix
	glUniformMatrix4fv(_shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	switch(*_scene)
	{
		case START_SCENE:
			{
				//---------- Background ----------//
				glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(glm::mat4(1)));
				_textures[0]->bind();
				_mesh->draw();

				//---------- Mouse ----------//
				glm::mat4 mat = glm::mat4(1.0f);
				mat = glm::translate(mat, glm::vec3(_mousePos.x, _mousePos.y, -0.1f));
				mat = glm::scale(mat, glm::vec3(0.01f/_ratio, 0.01f, 1.0f));
				mat = glm::transpose(mat);
				glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
				_textures[1]->bind();
				_mesh->draw();

				//---------- Name string ----------//
				{
					int size = (int)_name.size();
					float w = 0.05f/_ratio;
					float h = 0.10f;
					float y = 0.07f;
					for(int i=0; i<size; i++)
					{
						char letter = _name[i];
						if(letter==' ')
							continue;
						mat = glm::mat4(1.0f);
						mat = glm::translate(mat, glm::vec3(-w*size+i*w*2, y, -0.2f));
						mat = glm::scale(mat, glm::vec3(w, h, 1.0f));
						mat = glm::transpose(mat);
						glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
						_letters[letter]->bind();
						_mesh->draw();
					}
				}
			}
			break;
		case GARAGE_SCENE:
			{

			}
			break;
		case GAME_SCENE:
			{

			}
			break;
	}
}

void UserInterface::updateOnKey(int key, int scancode, int action, int mods)
{
	switch(*_scene)
	{
		case START_SCENE:
			{
				if(action == GLFW_RELEASE)
					return;
				if(key>='A' && key<='Z')
					_name += (char)key;
				else if(key>='a' && key<='a')
					_name += (char)(key-'a'+'A');
				else if(key>='0' && key<='9')
					_name += (char)(key);
				else if(key == ' ')
					_name += ' ';
				else if(key == GLFW_KEY_BACKSPACE)
					_name = _name.substr(0, _name.size()-1);
				else if(key == GLFW_KEY_ENTER)
					*_scene = GARAGE_SCENE;
			}
			break;
		case GARAGE_SCENE:
			{

			}
			break;
		case GAME_SCENE:
			{

			}
			break;
	}
}

void UserInterface::updateOnMouse(double xpos, double ypos)
{
	if(_firstMouse)
	{
		_firstMouse = false;
		_lastMousePos = {xpos, ypos};
	}

	_realMousePos = {xpos, ypos};
	float xOffset = _realMousePos.x-_lastMousePos.x;
	float yOffset = _realMousePos.y-_lastMousePos.y;

	float sensitivity = 3;
	xOffset*=sensitivity;
	yOffset*=-sensitivity;

	_mousePos.x+=xOffset;
	_mousePos.y+=yOffset;

	if(_mousePos.x>1) _mousePos.x=1;
	if(_mousePos.y>1) _mousePos.y=1;
	if(_mousePos.x<-1) _mousePos.x=-1;
	if(_mousePos.y<-1) _mousePos.y=-1;

	_lastMousePos = _realMousePos;
}
