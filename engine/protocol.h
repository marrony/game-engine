/*
 * protocol.h
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "socket.h"

const int MAX_PROTOCOL_PACKET_SIZE = 512;

int protocol_send_packet(Socket socket, const void* packet, size_t size);
int protocol_recv_packet(Socket socket, void* packet, size_t max_size, size_t& bytes_recv);

#endif /* PROTOCOL_H_ */
