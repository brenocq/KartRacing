//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-19                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "client.hpp"
#include <unistd.h>
#include <iostream>

#include "defines.hpp"
#include "helpers/log.hpp"
#include "nlohmann/json.hpp"

using namespace nlohmann;

Client::Client():
	_connected(false), _clientId(-1)
{

}

Client::~Client()
{

}

void Client::connectToServer()
{
	// Define socket type (TCP)
	if((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		Log::error("Client", "Socket creation error");
        exit(1);
    }

	// Populate server address
	_serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "0.0.0.0", &_serverAddr.sin_addr)<=0)
    {
		Log::error("Client", "Invalid address/ Address not supported.");
		exit(1);
    }

	// Connect to server
	if(connect(_sock, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0) 
	{
		Log::warning("Client", "Failed to connect to the server!"); 
		_connected = false;
	}
	else
	{
		Log::success("Client", "Connected to server.");
		_connected = true;
	}
}

void Client::updateServerState(std::vector<Kart*>& karts)
{
	if(_connected)
	{
		//---------- Send kart data ----------//
		json messageJson = karts[0]->getKartJson();
		messageJson["id"] = _clientId;
		std::string message = messageJson.dump();
		send(_sock , message.c_str() , message.size() , 0 ); 
		
		//---------- Receive game state ----------//
		char buffer[10000] = {0};
		int size = read(_sock, buffer, 10000); 

		if(size <= 0)
		{
			_connected = false;
			Log::warning("Client", "Server offline!");
			return;
		}
		json response = json::parse(std::string(buffer));
		if(buffer[0]!='[')
		{
			_clientId = response["id"];
			Log::verbose("Client", "Client id: "+std::to_string(_clientId));
		}
		else
		{
			std::cout << response << std::endl;
			while(karts.size()<response.size())
				karts.push_back(new Kart(karts[0]->getShader()));
			while(karts.size()>response.size())
			{
				delete karts.back();
				karts.back() == nullptr;
				karts.resize(karts.size()-1);
			}

			for(int i=0; i<(int)response.size(); i++)
			{
				int kartId = i;
				// Swap zero and clientId (I am 0)
				if(kartId == _clientId) continue;
				if(kartId == 0) kartId = _clientId;

				json kartState = response[i];
				std::cout << kartState << std::endl;
				karts[kartId]->setKartJson(kartState);
			}
		}
	}
}
