//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-19                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef SERVER_H
#define SERVER_H

#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

#include "nlohmann/json.hpp"
using namespace nlohmann;

class Server
{
	public:
		Server();
		~Server();

		void start();
		void loop();
	private:
		void checkMasterSocket();
		void checkClientSockets();

		std::vector<int> _clientSockets;
		int _maxClients;

		int _socket;
		struct sockaddr_in _address; 
		fd_set _fds; 

		std::vector<json> _clientInfos;
};

#endif// SERVER_H
