/*
 * socket.h
 *
 *  Created on: 03/04/2013
 *      Author: marrony
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
typedef int socklen_t;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <sys/unistd.h>
#include <sys/fcntl.h>

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

int socket_create_server(short port);
int socket_accept(int serversock);

int socket_connect(const char* host, short port);
int socket_close(int sock);

int socket_send(int sock, const void* buffer, size_t size, size_t* bytes_send);
int socket_recv(int sock, void* buffer, size_t size, size_t* bytes_recv);

bool socket_has_data(int sock);
int socket_set_non_blocking(int sock);

class Socket {
	int sock;
public:
	Socket(int sock = 0);

	bool connect(const char* host, short port);
	bool close();

	int send(const void* buffer, size_t size, size_t& bytes_send) const;
	int recv(void* buffer, size_t size, size_t& byte_recv);

	bool has_data() const;
};

class ServerSocket {
	int sock;
public:
	ServerSocket(short port);

	bool close();

	Socket accept();

	bool can_accept() const;
};

#endif /* SOCKET_H_ */
