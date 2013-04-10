/*
 * protocol.cpp
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */
#include "protocol.h"

int protocol_send_packet(Socket socket, const void* packet, size_t size) {
	size_t nbytes;

	uint32_t size_network = htonl(size);
	socket.send_all(&size_network, sizeof(size_network), nbytes);
	return socket.send_all(packet, size, nbytes);
}

int protocol_recv_packet(Socket socket, void* packet, size_t max_size, size_t& bytes_recv) {
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
