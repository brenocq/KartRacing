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
#include "letter.hpp"

Mesh* Kart::externMesh = nullptr;
Mesh* Kart::internMesh = nullptr;
Mesh* Kart::wheelMesh = nullptr;
std::vector<Texture*> Kart::externTextures = std::vector<Texture*>();
std::vector<Texture*> Kart::internTextures = std::vector<Texture*>();
std::vector<Texture*> Kart::wheelTextures = std::vector<Texture*>();

Kart::Kart(Shader* shader):
	_shader(shader), _name("ANONYMOUS")
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
	_frontWheelPos = {4.0f, 3.3f, 0.7f};
	_rearWheelPos = {5.0f, 3.3f, 0.7f};

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

	//---------- Draw rear wheels ----------//
	for(int i=-1;i<=1;i+=2)
	{
		mat = glm::mat4(1.0f);
		mat = glm::translate(mat, _position);
		mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(_rearWheelPos.y*i, _rearWheelPos.z, _rearWheelPos.x));
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
		mat = glm::translate(mat, glm::vec3(_frontWheelPos.y*i, _frontWheelPos.z, -_frontWheelPos.x));
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

	//---------- Draw name ----------//
	{
		int size = (int)_name.size();
		float w = 0.4;
		float h = 0.8;
		float y = 8;
		for(int i=0; i<size; i++)
		{
			char letter = _name[i];
			if(letter==' ')
				continue;
			mat = glm::mat4(1.0f);
			mat = glm::translate(mat, _position);
			mat = glm::rotate(mat, glm::radians(-_angle) ,glm::vec3(0, 1, 0));
			mat = glm::translate(mat, glm::vec3(-w*size+i*w*2, y, -0.2f));
			mat = glm::scale(mat, glm::vec3(w, h, 1.0f));
			mat = glm::transpose(mat);
			glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
			if(Letter::letters[letter]!=nullptr)
				Letter::letters[letter]->bind();
			if(Letter::mesh!=nullptr)
				Letter::mesh->draw();
		}
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
glm::vec3 Kart::calcTurnCenter()
{
	if(abs(_frontWheelAngle)<=.1)
		return {0,0,0};

	float L = _frontWheelPos.x + _rearWheelPos.x;
	float angTan = tan(glm::radians((float)90-abs(_frontWheelAngle)));
	if(_frontWheelAngle>0)
		return glm::vec3(_frontWheelPos.y + angTan*L,-_rearWheelPos.x,0);
	else
		return glm::vec3(-(_frontWheelPos.y + angTan*L),-_rearWheelPos.x,0);
}

void Kart::updatePhysics(float dt)
{
	if(_accelerate)
		_forceAccum += getFront()*15.0f;
	if(_brake)
		_forceAccum += getFront()*-15.0f;

	glm::vec3 turnCenter = calcTurnCenter();
	float turnRadius = glm::length(turnCenter);
	// Add centripetal force
	//_forceAccum += glm::normalize(glm::vec3(turnCenter.y, turnCenter.x, turnCenter.z))*(_velocity*dt)/turnRadius;

	// Calculate velocity
	_position += getFront()*length(_velocity)*dt;
	_acceleration = _forceAccum*_inverseMass;
	_velocity += _acceleration*dt;
	_velocity *= pow(_damping, dt);
	_velocity = getFront()*dot(_velocity, getFront());
	if(glm::length(_velocity)>100)
		_velocity = glm::normalize(_velocity)*100.0f;
	_forceAccum = {0,0,0};

	//std::cout << "with " << glm::degrees((glm::length(_velocity)*dt)/turnRadius)*(glm::dot(_acceleration, getFront())>0?-1:1)
	//			<< "without " << glm::degrees((glm::length(_velocity)*dt)/turnRadius) << std::endl;

	// Rotate kart
	//glm::vec3 turnCenter = calcTurnCenter();
	//float turnRadius = glm::length(turnCenter);
	if(turnRadius>0)
		_angle+=glm::degrees((std::min(glm::length(_velocity), 100.0f)*dt)/(turnRadius*2))*(_frontWheelAngle>0?1:-1);

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
	messageJson["kart"]["angle"] = _angle;
	messageJson["kart"]["name"] = _name;
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
	_angle = state["kart"]["angle"];
	_name = state["kart"]["name"];
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
