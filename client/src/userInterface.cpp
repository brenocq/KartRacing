//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "userInterface.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

UserInterface::UserInterface(Scene* scene, Shader* shader, float ratio, Kart* kart):
	_scene(scene), _shader(shader), _ratio(ratio), _kart(kart)
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
	_textures.push_back(new Texture("red.png"));//2

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

				//---------- Select intern texture ----------//
				{
					float size = 0.1;
					float offset = 0.02;
					float topX = 0.5;
					float topY = 0.8;

					for(int y=0;y<4;y++)
					{
						for(int x=0;x<4;x++)
						{
							if(_kart->getInternTexIndex() == x+4*y)
							{
								glm::mat4 mat = glm::mat4(1.0f);
								mat = glm::translate(mat, glm::vec3(topX+x*(size/_ratio+offset), topY-y*(size+offset), -0.0f));
								mat = glm::scale(mat, glm::vec3(1.15*size/(2*_ratio), 1.15*size/2, 1.0f));
								mat = glm::transpose(mat);
								glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
								_textures[2]->bind();
								_mesh->draw();
							}

							glm::mat4 mat = glm::mat4(1.0f);
							mat = glm::translate(mat, glm::vec3(topX+x*(size/_ratio+offset), topY-y*(size+offset), -0.1f));
							mat = glm::scale(mat, glm::vec3(size/(2*_ratio), size/2, 1.0f));
							mat = glm::transpose(mat);
							glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
							Kart::internTextures[x+y*4]->bind();
							_mesh->draw();
						}
					}
				}

				//---------- Select wheel texture ----------//
				{
					float size = 0.1;
					float offset = 0.02;
					float topX = 0.5;
					float topY = 0.2;

					for(int y=0;y<4;y++)
					{
						for(int x=0;x<4;x++)
						{
							if(_kart->getWheelTexIndex() == x+4*y)
							{
								glm::mat4 mat = glm::mat4(1.0f);
								mat = glm::translate(mat, glm::vec3(topX+x*(size/_ratio+offset), topY-y*(size+offset), -0.0f));
								mat = glm::scale(mat, glm::vec3(1.15*size/(2*_ratio), 1.15*size/2, 1.0f));
								mat = glm::transpose(mat);
								glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
								_textures[2]->bind();
								_mesh->draw();
							}

							glm::mat4 mat = glm::mat4(1.0f);
							mat = glm::translate(mat, glm::vec3(topX+x*(size/_ratio+offset), topY-y*(size+offset), -0.1f));
							mat = glm::scale(mat, glm::vec3(size/(2*_ratio), size/2, 1.0f));
							mat = glm::transpose(mat);
							glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
							Kart::wheelTextures[x+y*4]->bind();
							_mesh->draw();
						}
					}
				}

				//---------- Select extern texture ----------//
				{
					float height = 0.1;
					float offset = 0.03;
					float topX = 0.63;
					float topY = -0.45;

					for(int y=0;y<4;y++)
					{
						if(_kart->getExternTexIndex() == y)
						{
							glm::mat4 mat = glm::mat4(1.0f);
							mat = glm::translate(mat, glm::vec3(topX, topY-y*(height+offset), -0.0f));
							mat = glm::scale(mat, glm::vec3(1.015*height*10/(2*_ratio), 1.15*height/2, 1.0f));
							mat = glm::transpose(mat);
							glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
							_textures[2]->bind();
							_mesh->draw();
						}

						glm::mat4 mat = glm::mat4(1.0f);
						mat = glm::translate(mat, glm::vec3(topX, topY-y*(height+offset), -0.1f));
						mat = glm::scale(mat, glm::vec3(height*10/(2*_ratio), height/2, 1.0f));
						mat = glm::transpose(mat);
						glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
						Kart::externTextures[y]->bind();
						_mesh->draw();
					}
				}


				//---------- Mouse ----------//
				{
					glm::mat4 mat = glm::mat4(1.0f);
					mat = glm::translate(mat, glm::vec3(_mousePos.x, _mousePos.y, -0.2f));
					mat = glm::scale(mat, glm::vec3(0.01f/_ratio, 0.01f, 1.0f));
					mat = glm::transpose(mat);
					glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
					_textures[1]->bind();
					_mesh->draw();
				}
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

void UserInterface::updateOnMouseClick(int button, int action, int mods)
{
	switch(*_scene)
	{
		case START_SCENE:
			{
			}
			break;
		case GARAGE_SCENE:
			{
				if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				{
					//---------- Intern texture click ----------//
					{
						float size = 0.1;
						float offset = 0.02;
						float topX = 0.5;
						float topY = 0.8;

						for(int y=0;y<4;y++)
						{
							for(int x=0;x<4;x++)
							{
								float w = 1.15*size/(2*_ratio);
								float h = 1.15*size/2;
								if(_mousePos.x>=topX+x*(size/_ratio+offset)-w &&
									_mousePos.x<=topX+x*(size/_ratio+offset)+w &&
									_mousePos.y>=topY-y*(size+offset)-h &&
									_mousePos.y<=topY-y*(size+offset)+h)
									{
										_kart->setInternTexIndex(x+4*y);
									}
							}
						}
					}

					//---------- Wheel texture click ----------//
					{
						float size = 0.1;
						float offset = 0.02;
						float topX = 0.5;
						float topY = 0.2;

						for(int y=0;y<4;y++)
						{
							for(int x=0;x<4;x++)
							{
								float w = 1.15*size/(2*_ratio);
								float h = 1.15*size/2;
								if(_mousePos.x>=topX+x*(size/_ratio+offset)-w &&
									_mousePos.x<=topX+x*(size/_ratio+offset)+w &&
									_mousePos.y>=topY-y*(size+offset)-h &&
									_mousePos.y<=topY-y*(size+offset)+h)
									{
										_kart->setWheelTexIndex(x+4*y);
									}
							}
						}
					}

					//---------- Extern texture click ----------//
					{
						float height = 0.1;
						float offset = 0.03;
						float topX = 0.63;
						float topY = -0.45;

						for(int y=0;y<4;y++)
						{
							float w = 1.015*height*10/(2*_ratio);
							float h = 1.15*height/2;
							if(_mousePos.x>=topX-w &&
								_mousePos.x<=topX+w &&
								_mousePos.y>=topY-y*(height+offset)-h &&
								_mousePos.y<=topY-y*(height+offset)+h)
								{
									_kart->setExternTexIndex(y);
								}
						}
					}

				}
			}
			break;
		case GAME_SCENE:
			{

			}
			break;
	}
}
