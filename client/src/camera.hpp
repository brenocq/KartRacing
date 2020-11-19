//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
	public:
		Camera(float windowRatio);
		~Camera();

		void updateOnKey(int key, int scancode, int action, int mods);
		void updateOnMouse(double xpos, double ypos);
		void update(double dt);
		void printInfo();

		const float* getView();
		const float* getProjection();

		//---------- Getters and Setters ----------//
		void setPosition(glm::vec3 pos) { _position=pos; }
		void setFront(glm::vec3 front) { _front=front; }

	private:
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _front;
		float _speed;
		float _ratio;

		// Mouse handling
		bool _firstMouse;
		float _yaw, _pitch;
		float _lastX, _lastY;

		// Matrices
		glm::mat4 _view;
		glm::mat4 _projection;

		// Handling freeCamera
		int _movingForward;
		int _movingUp;
		int _movingLeft;
};

#endif// CAMERA_H
