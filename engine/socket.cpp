/*
 * socket.cpp
 *
 *  Created on: 03/04/2013
 *      Author: marrony
 */
#include "socket.h"

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

#ifndef WIN32
int closesocket(int sock) {
	return close(sock);
}
#endif

int create_server_socket(short port) {
#ifdef WIN32
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock < 0)
		return -1;

	int reuseaddr = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseaddr, sizeof(reuseaddr)) < 0) {
		closesocket(sock);
		return -1;
	}

	struct sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");
	service.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&service, sizeof(service)) < 0) {
		closesocket(sock);
		return -1;
	}

	if(listen(sock, SOMAXCONN) < 0) {
		closesocket(sock);
		return -1;
	}

	return sock;
}

int accept_socket(int serversock) {
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	return accept(serversock, (struct sockaddr*)&addr, &addrlen);
}

int connect_socket(const char* host, short port) {
#ifdef WIN32
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock < 0)
		return -1;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);

	if(connect(sock, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(sock);
		return -1;
	}

	return sock;
}

int close_socket(int sock) {
	return closesocket(sock);
}

int send_socket(int sock, const void* buffer, size_t size) {
#ifdef WIN32
	const char* ptr = (const char*)buffer;
#else
	const void* ptr = buffer;
#endif
	return send(sock, ptr, size, 0);
}

int recv_socket(int sock, void* buffer, size_t size) {
#ifdef WIN32
	char* ptr = (char*)buffer;
#else
	void* ptr = buffer;
#endif
	return recv(sock, ptr, size, 0);
}

bool has_data_socket(int sock) {
	fd_set rd_set;

	FD_ZERO(&rd_set);
	FD_SET(sock, &rd_set);

	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1;

	if(select(sock+1, &rd_set, NULL, NULL, &time) < 0)
		return false;

	return FD_ISSET(sock, &rd_set);
}

int set_non_blocking_socket(int sock) {
#ifdef WIN32
	u_long mode = 1;
	return ioctlsocket(sock, FIONBIO, &mode);
#else
	return fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
}

Socket::Socket(int sock) : sock(sock) {}

bool Socket::connect(const char* host, short port) {
	sock = connect_socket(host, port);
	return sock > 0;
}

bool Socket::close() {
	if(close_socket(sock) < 0)
		return false;

	sock = 0;
	return true;
}

int Socket::send(const void* buffer, size_t size) {
	return send_socket(sock, buffer, size);
}

int Socket::recv(void* buffer, size_t size) {
	return recv_socket(sock, buffer, size);
}

ServerSocket::ServerSocket(short port) {
	sock = create_server_socket(port);
}

bool ServerSocket::close() {
	return close_socket(sock) > 0;
}

Socket ServerSocket::accept() {
	return Socket(accept_socket(sock));
}
