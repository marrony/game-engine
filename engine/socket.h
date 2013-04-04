/*
 * socket.h
 *
 *  Created on: 03/04/2013
 *      Author: marrony
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stddef.h>

int create_server_socket(short port);
int accept_socket(int serversock);

int connect_socket(const char* host, short port);
int close_socket(int sock);

int send_socket(int sock, const void* buffer, size_t size);
int recv_socket(int sock, void* buffer, size_t size);

bool has_data_socket(int sock);
int set_non_blocking_socket(int sock);

class Socket {
	int sock;
public:
	Socket(int sock = 0);

	bool connect(const char* host, short port);
	bool close();

	int send(const void* buffer, size_t size);
	int recv(void* buffer, size_t size);
};

class ServerSocket {
	int sock;
public:
	ServerSocket(short port);

	bool close();

	Socket accept();
};

#endif /* SOCKET_H_ */
