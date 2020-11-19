//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-19                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef CLIENT_H
#define CLIENT_H
#include <sys/socket.h>
#include <arpa/inet.h>
#include "kart.hpp"

class Client
{
	public:
		Client();
		~Client();

		void connectToServer();
		void updateServerState(std::vector<Kart*>& karts);
	private:
		int _sock;
		bool _connected;
		int _clientId;
		struct sockaddr_in _serverAddr;
};

#endif// CLIENT_H
