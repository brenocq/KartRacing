//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef KART_H
#define KART_H
#include <vector>
#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "nlohmann/json.hpp"
using namespace nlohmann;

class Kart
{
	public:
		Kart(Shader* shader);
		~Kart();

		void draw();

		static Mesh* externMesh;
		static Mesh* internMesh;
		static Mesh* wheelMesh;
		static std::vector<Texture*> externTextures;
		static std::vector<Texture*> internTextures;
		static std::vector<Texture*> wheelTextures;

		//---------- Getters and Setters ----------//
		void setName(std::string name) { _name=name; }
		void setPosition(glm::vec3 pos) { _position=pos; }
		glm::vec3 getPosition() const { return _position; }
		glm::vec3 getFront();
		glm::vec3 getFrontWheelVector();
		void setAngle(float angle) { _angle=angle; }
		void setInternTexIndex(int index) { _internTexIndex=index; }
		int getInternTexIndex() const { return _internTexIndex; }
		void setExternTexIndex(int index) { _externTexIndex=index; }
		int getExternTexIndex() const { return _externTexIndex; }
		void setWheelTexIndex(int index) { _wheelTexIndex=index; }
		int getWheelTexIndex() const { return _wheelTexIndex; }
		Shader* getShader() const { return _shader; }

		json getKartJson();
		void setKartJson(json state);

		//---------- Callbacks ----------//
		void updatePhysics(float dt);
		void updateOnKey(int key, int scancode, int action, int mods);

	private:
		void rotateFrontWheel(float angle);
		glm::vec3 calcTurnCenter();

		Shader* _shader;
		std::string _name;

		// User control
		bool _accelerate;
		bool _brake;
		int _steeringWheel;
		
		// Physiscs
		glm::vec3 _position;
		glm::vec3 _velocity;
		glm::vec3 _acceleration;
		float _damping;

		glm::vec3 _forceAccum;
		float _inverseMass;

		// Body
		float _angle;

		// Wheels
		glm::vec3 _frontWheelPos;
		glm::vec3 _rearWheelPos;

		float _wheelAngularPosition;
		float _wheelAngularVelocity;
		float _frontWheelAngle;
		float _frontWheelAngularVelocity;
		float _frontWheelMaxAngle;

		// Textures
		int _internTexIndex;
		int _externTexIndex;
		int _wheelTexIndex;
};

#endif// KART_H
