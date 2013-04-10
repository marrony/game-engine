/*
 * protocol.cpp
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */
#include "protocol.h"
#include "json.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int protocol_send_raw_packet(Socket socket, const void* packet, size_t size) {
	size_t nbytes;

	uint32_t size_network = htonl(size);
	socket.send_all(&size_network, sizeof(size_network), nbytes);
	return socket.send_all(packet, size, nbytes);
}

int protocol_recv_raw_packet(Socket socket, void* packet, size_t max_size, size_t& bytes_recv) {
	size_t nbytes;
	uint32_t size_network;

	socket.recv_all(&size_network, sizeof(size_network), nbytes);

	size_network = ntohl(size_network);

	uint32_t ignore_bytes = 0;
	if(size_network > max_size) {
		ignore_bytes = size_network - max_size;
		size_network = max_size;
	}

	int ret = socket.recv_all(packet, size_network, bytes_recv);

	if(ret > 0) {
		char ignore;

		for(uint32_t i = 0; i < ignore_bytes; i++) {
			if(socket.recv(&ignore, sizeof(ignore)) <= 0)
				break;
		}
	}

	return ret;
}

void protocol_send_window_message(Socket sock, intptr_t window, int width, int height) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];

	snprintf(buffer, sizeof(buffer), "{\"window\": %ld, \"width\": %d, \"height\": %d}", window, width, height);
	protocol_send_raw_packet(sock, buffer, strlen(buffer));
}

void protocol_send_load_mesh_message(Socket sock, const char* mesh) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];

	snprintf(buffer, sizeof(buffer), "{\"type\": \"load-mesh\", \"mesh\": \"%s\"}", mesh);
	protocol_send_raw_packet(sock, buffer, strlen(buffer));
}

void protocol_send_finish_message(Socket sock) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];

	snprintf(buffer, sizeof(buffer), "{\"type\": \"finish\"}");
	protocol_send_raw_packet(sock, buffer, strlen(buffer));
}

void protocol_send_resize_message(Socket sock, int width, int height) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];

	snprintf(buffer, sizeof(buffer), "{\"type\": \"resize\", \"width\": %d, \"height\": %d}", width, height);
	protocol_send_raw_packet(sock, buffer, strlen(buffer));
}

bool protocol_recv_message(Socket sock, Json& json) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	size_t nbytes;

	if(protocol_recv_raw_packet(sock, buffer, sizeof(buffer), nbytes) > 0) {
		buffer[nbytes] = 0;

		json = json_parse(buffer, nbytes);

		return true;
	}

	return false;
}
