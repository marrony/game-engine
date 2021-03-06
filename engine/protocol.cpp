/*
 * protocol.cpp
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */
#include "protocol.h"
#include "json.h"
#include "json_io.h"

#include "quaternion.h"
#include "vector3.h"

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
	char data[MAX_PROTOCOL_PACKET_SIZE];

	Json json = json_initialize(data, MAX_PROTOCOL_PACKET_SIZE);

	json.root = json_create_object(json);
	json_set_attribute(json, json.root, "window", json_create_int(json, window));
	json_set_attribute(json, json.root, "width", json_create_int(json, width));
	json_set_attribute(json, json.root, "height", json_create_int(json, height));
	int nbytes = json_serialize(json, buffer, MAX_PROTOCOL_PACKET_SIZE);

	json_free(json);

	protocol_send_raw_packet(sock, buffer, nbytes);
}

void protocol_send_load_mesh_message(Socket sock, const char* entity, const char* mesh) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	char data[MAX_PROTOCOL_PACKET_SIZE];

	Json json = json_initialize(data, MAX_PROTOCOL_PACKET_SIZE);

	Value translation = json_create_array(json);
	Value orientation = json_create_array(json);
	Value scale = json_create_array(json);

	json_set_at(json, translation, 0, json_create_number(json, 0));
	json_set_at(json, translation, 1, json_create_number(json, -5));
	json_set_at(json, translation, 2, json_create_number(json, -150));

	json_set_at(json, scale, 0, json_create_number(json, 1));
	json_set_at(json, scale, 1, json_create_number(json, 1));
	json_set_at(json, scale, 2, json_create_number(json, 1));

	Quaternion qt = Quaternion::make(Vector3::make(0, 0, 1), 0);
	json_set_at(json, orientation, 0, json_create_number(json, qt.s));
	json_set_at(json, orientation, 1, json_create_number(json, qt.x));
	json_set_at(json, orientation, 2, json_create_number(json, qt.y));
	json_set_at(json, orientation, 3, json_create_number(json, qt.z));

	json.root = json_create_object(json);
	json_set_attribute(json, json.root, "type", json_create_string(json, "create-model"));
	json_set_attribute(json, json.root, "mesh", json_create_string(json, mesh));
	json_set_attribute(json, json.root, "entity", json_create_string(json, entity));
	json_set_attribute(json, json.root, "translation", translation);
	json_set_attribute(json, json.root, "orientation", orientation);
	json_set_attribute(json, json.root, "scale", scale);
	int nbytes = json_serialize(json, buffer, MAX_PROTOCOL_PACKET_SIZE);

	json_free(json);

	protocol_send_raw_packet(sock, buffer, nbytes);
}

void protocol_send_finish_message(Socket sock) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	char data[MAX_PROTOCOL_PACKET_SIZE];

	Json json = json_initialize(data, MAX_PROTOCOL_PACKET_SIZE);

	json.root = json_create_object(json);
	json_set_attribute(json, json.root, "type", json_create_string(json, "finish"));
	int nbytes = json_serialize(json, buffer, MAX_PROTOCOL_PACKET_SIZE);

	json_free(json);

	protocol_send_raw_packet(sock, buffer, nbytes);
}

void protocol_send_resize_message(Socket sock, int width, int height) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	char data[MAX_PROTOCOL_PACKET_SIZE];

	Json json = json_initialize(data, MAX_PROTOCOL_PACKET_SIZE);

	json.root = json_create_object(json);
	json_set_attribute(json, json.root, "type", json_create_string(json, "resize"));
	json_set_attribute(json, json.root, "width", json_create_int(json, width));
	json_set_attribute(json, json.root, "height", json_create_int(json, height));
	int nbytes = json_serialize(json, buffer, MAX_PROTOCOL_PACKET_SIZE);

	json_free(json);

	protocol_send_raw_packet(sock, buffer, nbytes);
}

bool protocol_recv_message(Socket sock, Json& json) {
	char buffer[MAX_PROTOCOL_PACKET_SIZE];
	size_t nbytes;

	if(protocol_recv_raw_packet(sock, buffer, sizeof(buffer), nbytes) > 0) {
		buffer[nbytes] = 0;

		json = json_parse(buffer, nbytes, true);

		return true;
	}

	return false;
}
