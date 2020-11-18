//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.hpp"

Camera::Camera(float windowRatio):
	_ratio(windowRatio), _firstMouse(true)
{
	_position = {0,0,5};
	_up = {0,1,0};
	_front = {0,0,-1};

	_speed = 20;

	_yaw = -90;
	_pitch = 0;

	_movingForward = 0;
	_movingUp = 0;
	_movingLeft = 0;
}

Camera::~Camera()
{

}

void Camera::updateOnKey(int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
			if(action == GLFW_RELEASE)
				_movingForward = 0;
			else
				_movingForward = 1;
			//_position += _front*_speed;
			break;
		case GLFW_KEY_A:
			if(action == GLFW_RELEASE)
				_movingLeft = 0;
			else
				_movingLeft = -1;
			//_position -= glm::normalize(glm::cross(_front, _up)) * _speed;
			break;
		case GLFW_KEY_S:
			if(action == GLFW_RELEASE)
				_movingForward = 0;
			else
				_movingForward = -1;
			//_position -= _front*_speed;
			break;
		case GLFW_KEY_D:
			if(action == GLFW_RELEASE)
				_movingLeft = 0;
			else
				_movingLeft = 1;
			//_position += glm::normalize(glm::cross(_front, _up)) * _speed;
			break;
		case GLFW_KEY_E:
			if(action == GLFW_RELEASE)
				_movingUp = 0;
			else
				_movingUp = 1;
			//_position += _up*_speed;
			break;
		case GLFW_KEY_Q:
			if(action == GLFW_RELEASE)
				_movingUp = 0;
			else
				_movingUp = -1;
			//_position -= _up*_speed;
			break;
	}
}

void Camera::updateOnMouse(double xpos, double ypos)
{
	if(_firstMouse)
	{
		_firstMouse = false;
		_lastX = xpos;
		_lastY = ypos;
	}

	float xoffset = xpos-_lastX;
	float yoffset = ypos-_lastY;

	float sensitivity = 600;
	xoffset*=sensitivity;
	yoffset*=sensitivity;

	_yaw+=xoffset;
	_pitch-=yoffset;

	if(_pitch>=90)_pitch=90;
	if(_pitch<=-90)_pitch=-90;

	glm::vec3 front;
	front.x = cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _front = glm::normalize(front);

	_lastX = xpos;
	_lastY = ypos;
}

void Camera::update(double dt)
{
	_position += _front*_speed * float(_movingForward*dt);
	_position += glm::normalize(glm::cross(_front, _up))*_speed * float(_movingLeft*dt);
	_position += _up*_speed * float(_movingUp*dt);

	_view = glm::lookAt(_position, _position+_front, _up);
	_projection = glm::perspective(
			glm::radians(60.0f),
			_ratio,
			0.1f,
			1000.0f
		);
}

const float* Camera::getView()
{
	return glm::value_ptr(_view);
}

const float* Camera::getProjection()
{
	return glm::value_ptr(_projection);
}
