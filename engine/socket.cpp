/*
 * socket.cpp
 *
 *  Created on: 03/04/2013
 *      Author: marrony
 */
#include "socket.h"
#include <errno.h>

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
		closesocket(sock);
		return -1;
	}

	return sock;
}

int socket_close(int sock) {
	return closesocket(sock);
}

int socket_send(int sock, const void* buffer, size_t size) {
	const char* ptr = (const char*)buffer;

	int nbytes;
	do {
		nbytes = send(sock, ptr, size, 0);
	} while(nbytes < 0 && errno == EINTR);

	return nbytes;
}

int socket_recv(int sock, void* buffer, size_t size) {
	char* ptr = (char*)buffer;

	int nbytes;
	do {
		nbytes = recv(sock, ptr, size, 0);
	} while(nbytes < 0 && errno == EINTR);

	return nbytes;
}

int socket_send_all(int sock, const void* buffer, size_t size, size_t* bytes_send) {
	const char* ptr = (const char*)buffer;

	size_t total = 0;
	size_t left = size;

	int nbytes;
	while(total < size) {
		nbytes = socket_send(sock, ptr+total, left);

		if(nbytes <= 0)
			break;

		total += nbytes;
		left -= nbytes;
	}

	if(bytes_send)
		*bytes_send = total;

	return nbytes;
}

int socket_recv_all(int sock, void* buffer, size_t size, size_t* bytes_recv) {
	char* ptr = (char*)buffer;

	size_t total = 0;
	size_t left = size;

	int nbytes;
	while(total < size) {
		nbytes = socket_recv(sock, ptr+total, left);

		if(nbytes <= 0)
			break;

		total += nbytes;
		left -= nbytes;
	}

	if(bytes_recv)
		*bytes_recv = total;

	return nbytes;
}

bool socket_has_data(int sock) {
	fd_set rd_set;

	FD_ZERO(&rd_set);
	FD_SET(sock, &rd_set);

	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1;

	if(select(sock+1, &rd_set, NULL, NULL, &time) <= 0)
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

int Socket::send_all(const void* buffer, size_t size, size_t& bytes_send) const {
	return socket_send_all(sock, buffer, size, &bytes_send);
}

int Socket::recv_all(void* buffer, size_t size, size_t& bytes_recv) const {
	return socket_recv_all(sock, buffer, size, &bytes_recv);
}

int Socket::send(const void* buffer, size_t size) const {
	return socket_send(sock, buffer, size);
}

int Socket::recv(void* buffer, size_t size) const {
	return socket_recv(sock, buffer, size);
}

bool Socket::has_data() const {
	return socket_has_data(sock);
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

bool ServerSocket::can_accept() const {
	return socket_has_data(sock);
}
