/*
 * protocol.h
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "socket.h"

struct Json;

const int MAX_PROTOCOL_PACKET_SIZE = 512;

int protocol_send_raw_packet(Socket socket, const void* packet, size_t size);
int protocol_recv_raw_packet(Socket socket, void* packet, size_t max_size, size_t& bytes_recv);

void protocol_send_window_message(Socket sock, intptr_t window, int width, int height);
void protocol_send_load_mesh_message(Socket sock, const char* mesh);
void protocol_send_finish_message(Socket sock);
void protocol_send_resize_message(Socket sock, int width, int height);
bool protocol_recv_message(Socket sock, Json& json);

#endif /* PROTOCOL_H_ */
