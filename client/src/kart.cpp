//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "kart.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

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

	// Textures
	_internTexIndex = 0;
	_externTexIndex = 0;
	_wheelTexIndex = 0;

	// User control
	_accelerate = false;
	_brake = false;
	_steeringWheel = 0;

	// Wheels
	_wheelAngularPosition = 0;
	_wheelAngularVelocity = 0;
	_frontWheelAngle = 0;
	_frontWheelMaxAngle = 30;
	_frontWheelAngularVelocity = 30;

	// Physics
	_velocity = {0,0,0};
	_acceleration = {0,0,0};
	_forceAccum = {0,0,0};
	_inverseMass = 1/1.0f;
	_damping = 0.99;
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
		mat = glm::translate(mat, _position);
		mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(3.3f*i, 0.7f, 5.0f));
		//mat = glm::translate(mat, glm::vec3(3.3f*i*sin(glm::radians(-_angle)), 0.7f, 5.0f*cos(glm::radians(-_angle)))+_position);
		mat = glm::rotate(mat, glm::radians(180.0f*(i==1)) ,glm::vec3(0, 0, 1));
		mat = glm::rotate(mat, glm::radians(_wheelAngularPosition*i) ,glm::vec3(1, 0, 0));
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
		mat = glm::translate(mat, _position);
		mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(3.3f*i, 0.7f, -4.8f));
		mat = glm::rotate(mat, glm::radians(180.0f*(i==1)) ,glm::vec3(0, 0, 1));
		mat = glm::rotate(mat, glm::radians(_frontWheelAngle*i) ,glm::vec3(0, 1, 0));
		mat = glm::rotate(mat, glm::radians(_wheelAngularPosition*i) ,glm::vec3(1, 0, 0));
		mat = glm::scale(mat, glm::vec3(0.1f, 0.1f, 0.1f));
		mat = glm::transpose(mat);
		glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
		
		// Bind texture/VAO and draw
		wheelTextures[_wheelTexIndex]->bind();
		wheelMesh->draw();
	}
}

glm::vec3 Kart::getFront()
{
	float angle = glm::radians(_angle-90);
	return glm::normalize(glm::vec3(cos(angle), 0, sin(angle)));
}

glm::vec3 Kart::getFrontWheelVector()
{
	float angle = glm::radians(_angle-90+_frontWheelAngle);
	return glm::normalize(glm::vec3(cos(angle), 0, sin(angle)));
}

//------------------------------//
//----------- Physics ----------//
//------------------------------//
void Kart::updatePhysics(float dt)
{
	if(_accelerate)
		_forceAccum += getFront()*15.0f;
	if(_brake)
		_forceAccum += getFront()*-15.0f;

	// Calculate kart rotation
	//glm::vec3 f = getFront();	
	//glm::vec3 fw = getFrontWheelVector();	
	//float dAngle = atan2(f.x*fw.z-f.z*fw.x, f.x*fw.x+f.z*fw.z);
	_angle+=_frontWheelAngle*dt;

	// Calculate velocity
	_position += _velocity*dt;
	_acceleration = _forceAccum*_inverseMass;
	_velocity += _acceleration*dt;
	_velocity *= pow(_damping, dt);
	_forceAccum = {0,0,0};

	// Update wheel velocity
	float wheelPerimeter = 9.425f;
	_wheelAngularVelocity = (glm::length(_velocity)/wheelPerimeter)*360.0f*(glm::dot(_velocity, getFront())>0?1:-1);
	_wheelAngularPosition += _wheelAngularVelocity*dt;
	if(_wheelAngularPosition>360)_wheelAngularPosition-=360;
	if(_wheelAngularPosition<0)	 _wheelAngularPosition+=360;

	// Update front wheel angle
	_frontWheelAngle += _frontWheelAngularVelocity*dt*_steeringWheel;
	if(_frontWheelAngle>_frontWheelMaxAngle)	_frontWheelAngle=_frontWheelMaxAngle;
	if(_frontWheelAngle<-_frontWheelMaxAngle)	_frontWheelAngle=-_frontWheelMaxAngle;
}

void Kart::updateOnKey(int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			_accelerate = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			_brake = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			if(action == GLFW_RELEASE)
				_steeringWheel = 0;
			else
				_steeringWheel = -1;
			break;
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			if(action == GLFW_RELEASE)
				_steeringWheel = 0;
			else
				_steeringWheel = 1;
			break;
	}
}

void Kart::rotateFrontWheel(float angle)
{
	_frontWheelAngle+=angle;
	if(_frontWheelAngle>_frontWheelMaxAngle) _frontWheelAngle=_frontWheelMaxAngle;
	if(_frontWheelAngle<-_frontWheelMaxAngle) _frontWheelAngle=-_frontWheelMaxAngle;
}

json Kart::getKartJson()
{
	json messageJson;
	messageJson["kart"] = {};
	messageJson["kart"]["pos"] = {_position.x,_position.y,_position.z};
	messageJson["kart"]["vel"] = {_velocity.x,_velocity.y,_velocity.z};
	messageJson["kart"]["acc"] = {_acceleration.x,_acceleration.y,_acceleration.z};
	messageJson["kart"]["wheel"] = {};
	messageJson["kart"]["wheel"]["angPos"] = _wheelAngularPosition;
	messageJson["kart"]["wheel"]["angVel"] = _wheelAngularVelocity;
	messageJson["kart"]["wheel"]["ftAngle"] = _frontWheelAngle;
	messageJson["kart"]["wheel"]["ftAngVel"] = _frontWheelAngularVelocity;
	messageJson["kart"]["tex"] = {};
	messageJson["kart"]["tex"]["intern"] = _internTexIndex;
	messageJson["kart"]["tex"]["extern"] = _externTexIndex;
	messageJson["kart"]["tex"]["wheel"] = _wheelTexIndex;
	messageJson["kart"]["control"] = {};
	messageJson["kart"]["control"]["acc"] = _accelerate;
	messageJson["kart"]["control"]["brake"] = _brake;
	messageJson["kart"]["control"]["stWheel"] = _steeringWheel;

	return messageJson;
}

void Kart::setKartJson(json state)
{
	_position.x = state["kart"]["pos"][0];
	_position.y = state["kart"]["pos"][1];
	_position.z = state["kart"]["pos"][2];
	_velocity.x = state["kart"]["vel"][0];
	_velocity.y = state["kart"]["vel"][1];
	_velocity.z = state["kart"]["vel"][2];
	_acceleration.x = state["kart"]["acc"][0];
	_acceleration.y = state["kart"]["acc"][1];
	_acceleration.z = state["kart"]["acc"][2];
	_wheelAngularPosition = state["kart"]["wheel"]["angPos"];
	_wheelAngularVelocity = state["kart"]["wheel"]["angVel"];
	_frontWheelAngle = state["kart"]["wheel"]["ftAngle"];
	_frontWheelAngularVelocity = state["kart"]["wheel"]["ftAngVel"];
	_internTexIndex = state["kart"]["tex"]["intern"];
	_externTexIndex = state["kart"]["tex"]["extern"];
	_wheelTexIndex = state["kart"]["tex"]["wheel"];
	_accelerate = state["kart"]["control"]["acc"];
	_brake = state["kart"]["control"]["brake"];
	_steeringWheel = state["kart"]["control"]["stWheel"];
}
