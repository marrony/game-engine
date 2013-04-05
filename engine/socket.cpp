/*
 * socket.cpp
 *
 *  Created on: 03/04/2013
 *      Author: marrony
 */
#include "socket.h"

#ifndef WIN32
static int closesocket(int sock) {
	return close(sock);
}
#endif

int socket_create_server(short port) {
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

int socket_accept(int serversock) {
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	return accept(serversock, (struct sockaddr*)&addr, &addrlen);
}

int socket_connect(const char* host, short port) {
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

int socket_close(int sock) {
	return closesocket(sock);
}

int socket_send(int sock, const void* buffer, size_t size) {
#ifdef WIN32
	const char* ptr = (const char*)buffer;
#else
	const void* ptr = buffer;
#endif
	return send(sock, ptr, size, 0);
}

int socket_recv(int sock, void* buffer, size_t size) {
#ifdef WIN32
	char* ptr = (char*)buffer;
#else
	void* ptr = buffer;
#endif
	return recv(sock, ptr, size, 0);
}

bool socket_has_data(int sock) {
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

int socket_set_non_blocking(int sock) {
#ifdef WIN32
	u_long mode = 1;
	return ioctlsocket(sock, FIONBIO, &mode);
#else
	return fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
}

Socket::Socket(int sock) : sock(sock) {}

bool Socket::connect(const char* host, short port) {
	sock = socket_connect(host, port);
	return sock > 0;
}

bool Socket::close() {
	if(socket_close(sock) < 0)
		return false;

	sock = 0;
	return true;
}

int Socket::send(const void* buffer, size_t size) {
	return socket_send(sock, buffer, size);
}

int Socket::recv(void* buffer, size_t size) {
	return socket_recv(sock, buffer, size);
}

ServerSocket::ServerSocket(short port) {
	sock = socket_create_server(port);
}

bool ServerSocket::close() {
	return socket_close(sock) > 0;
}

Socket ServerSocket::accept() {
	return Socket(socket_accept(sock));
}