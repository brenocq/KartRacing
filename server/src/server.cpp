//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-19                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "server.hpp"
#include "defines.hpp"
#include "helpers/log.hpp"
#include <iostream>

Server::Server():
	_maxClients(20)
{
	_clientSockets.resize(_maxClients);
	_clientInfos.resize(_maxClients);
	start();
	loop();
}

Server::~Server()
{

}

void Server::start()
{
	// Create master socket
	if((_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		Log::error("Server","Failed to create socket!");
		exit(1);
	} 

	// Allow multiple connections (not necessary)
	int opt = 1;
	if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) 
	{ 
		Log::error("Server","Failed to set socket options!");
		exit(1); 
	} 

	// Socket type
	_address.sin_family = AF_INET; 
	_address.sin_addr.s_addr = INADDR_ANY; 
	_address.sin_port = htons( PORT ); 

	// Bind the socket to port
	if(bind(_socket, (struct sockaddr *)&_address, sizeof(_address))<0) 
	{ 
		Log::error("Server","Failed to bind socket!");
		exit(1); 
	} 
	Log::info("Server", "Listener on port "+std::to_string(PORT));

	// Specify maximum number of pending connections
	if(listen(_socket, 5) < 0) 
	{ 
		Log::error("Server", "Failed to set listen!"); 
		exit(1); 
	} 

	Log::success("Server", "Initialized!");
}

void Server::loop()
{
	while(true)
	{
		// Clear the socket set 
		FD_ZERO(&_fds); 

		//---------- Add sockets to set ----------//
		// Add master socket to set 
		FD_SET(_socket, &_fds); 
		
		// Maximum socket descriptor
		int maxSd = _socket;

		// Add child sockets to set (sd = socket descriptor)
		for(auto sd : _clientSockets) 
		{ 
			// If valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET(sd, &_fds); 
				
			// Highest file descriptor number, need it for the select function 
			if(sd > maxSd) 
				maxSd = sd; 
		} 

		//---------- Wait ----------//
		// Wait for an activity on one of the sockets
		// Timeout is NULL, so wait indefinitely 
		if(select(maxSd+1 , &_fds , NULL , NULL , NULL)<0)
		{
			Log::error("Server", "Failed to select socket");
			exit(1);
		}

		//---------- Check sockets ----------//
		checkMasterSocket();
		checkClientSockets();
	}
}

void Server::checkMasterSocket()
{
	if(FD_ISSET(_socket, &_fds)) 
	{ 
		int newSocket;
		int addrLen = sizeof(_address);
		if((newSocket = accept(_socket, (struct sockaddr *)&_address, (socklen_t*)&addrLen))<0) 
		{ 
			Log::error("Server", "Failed to accept connection"); 
			exit(1); 
		} 
		Log::success("Server", std::string("New client connection ->")+
				" df:"+std::to_string(newSocket)+
				" ip:"+std::string(inet_ntoa(_address.sin_addr))+
				" port:"+std::to_string(ntohs(_address.sin_port))
				); 
			
		int newSocketId = 0;
		//add new socket to array of sockets 
		for(int i = 0; i < (int)_clientSockets.size(); i++) 
		{ 
			//if position is empty 
			if(_clientSockets[i] == 0 ) 
			{ 
				_clientSockets[i] = newSocket; 
				newSocketId = i;
				Log::info("Server", "Adding to list of sockets as "+std::to_string(i)); 
				break; 
			} 
		} 

		// Send connection message
		std::string connectionMessage = "{\"id\":"+std::to_string(newSocketId)+"}"; 
		if(send(newSocket, connectionMessage.c_str(), connectionMessage.size(), 0) != (int)connectionMessage.size())
		{ 
			Log::error("Server", "Failed to send welcome message"); 
		} 
	} 
}

void Server::checkClientSockets()
{
	char buffer[10000];
	int valread;

	// Check IO operation on client sockets
	for(int i = 0; i < (int)_clientSockets.size(); i++) 
	{ 
		int sd = _clientSockets[i]; 
			
		if(FD_ISSET(sd, &_fds)) 
		{ 
			// Check if it was for closing, and also read the incoming message 
			if((valread = read(sd , buffer, 10000)) <= 0)
			{ 
				// Somebody disconnected, get his details and print 
				int addrLen = sizeof(_address);
				getpeername(sd, (struct sockaddr*)&_address, (socklen_t*)&addrLen); 
				Log::success("Server", std::string("Client disconnected ->")+
						" ip:"+std::string(inet_ntoa(_address.sin_addr))+
						" port:"+std::to_string(ntohs(_address.sin_port))
							);
					
				// Close the socket and mark as 0 in list for reuse 
				close(sd); 
				_clientSockets[i] = 0; 
			} 
			else
			{ 
				buffer[valread] = '\0';

				// Update client data
				std::cout << i << "("<<valread<<")" << " -> "<< std::string(buffer) << std::endl;
				_clientInfos[i] = json::parse(std::string(buffer));

				// Merge all clients data to one json
				json gameState;
				for(int j=0; j<_maxClients; j++)
				{
					if(_clientSockets[j]!=0)
					{
						gameState+=_clientInfos[j];
					}
				}

				// Responds with data from all active clients
				std::string message = gameState.dump();
				send(sd, message.c_str(), message.size(), 0); 
			} 
		} 
	} 
}
