/*
 * engine.cpp
 *
 *  Created on: 01/04/2013
 *      Author: marrony
 */

#include "engine.h"
#include "swap_chain.h"

#include <cstring>
#include <stdlib.h>

int get_argument(int argc, char* argv[], const char* arg_name) {
	for(int i = 0; i < argc; i++) {
		if(!strcmp(arg_name, argv[i]) && i+1 < argc)
			return atoi(argv[i+1]);
	}

	return -1;
}

int create_server(short port) {
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

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

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

int main(int argc, char* argv[]) {
	WindowID handle = (WindowID)get_argument(argc, argv, "--window-id");
	int width = get_argument(argc, argv, "--width");
	int height = get_argument(argc, argv, "--height");

	int port = get_argument(argc, argv, "--port");
	int socket = create_server(port < 0 ? 9090 : port);

	if(handle == WINDOW_NULL)
		return 1;

	SwapChain swap_chain = create_swap_chain(handle, width, height);

	while(true) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1., 1., -1., 1., 1., 20.);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-.75, -.75, -1.0);
		glColor3f(0, 1, 0);
		glVertex3f(.75, -.75, -1.0);
		glColor3f(0, 0, 1);
		glVertex3f(.75, .75, -1.0);
		glEnd();

		swap_swap_chain(swap_chain);
	}

	return 0;
}
