/*
 * stream.cpp
 *
 *  Created on: 08/04/2013
 *      Author: marrony
 */

#include "stream.h"
#include "socket.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static StreamError refill_zeros(Stream* stream) {
	static const char zeros[256] = {0};

	stream->cursor = zeros;
	stream->start = zeros;
	stream->end = zeros + 256;

	return stream->error;
}

static StreamError fail_stream(Stream* stream, StreamError reason) {
	stream->error = reason;
	stream->refill = refill_zeros;

	return stream->refill(stream);
}

static StreamError refill_memory_stream(Stream* stream) {
	return fail_stream(stream, EofError);
}

static void destroy_memory_stream(Stream* stream) {
	stream->cursor = NULL;
	stream->start = NULL;
	stream->end = NULL;
}

void create_memory_stream(Stream* stream, const char* buffer, size_t size) {
	stream->cursor = buffer;
	stream->start = buffer;
	stream->end = buffer + size;
	stream->refill = refill_memory_stream;
	stream->destroy = destroy_memory_stream;
	stream->error = NoError;
}

const int FILE_BUFFER_SIZE = 256;

static StreamError refill_file(FileStream* stream) {
	int nbytes;

	do {
		nbytes = read(stream->fd, (void*)stream->start, FILE_BUFFER_SIZE);
	} while(nbytes == -1 && errno == EINTR);

	if(nbytes > 0) {
		stream->cursor = stream->start;
		stream->end = stream->start + nbytes;

		return stream->error;
	} else {
		return fail_stream(stream, EofError);
	}
}

static void destroy_file_stream(FileStream* stream) {
	close(stream->fd);
	free((void*)stream->buffer);

	stream->buffer = NULL;
	stream->cursor = NULL;
	stream->start = NULL;
	stream->end = NULL;
	stream->fd = 0;
}

void create_file_stream(FileStream* stream, const char* filename) {
	stream->buffer = (const char*)malloc(FILE_BUFFER_SIZE);
	stream->cursor = stream->buffer;
	stream->start = stream->buffer;
	stream->end = stream->start + FILE_BUFFER_SIZE;

	stream->refill = (RefillFunc)refill_file;
	stream->destroy = (DestroyFunc)destroy_file_stream;

	stream->fd = open(filename, O_RDONLY);

	stream->refill(stream);
}

static StreamError refill_socket(SocketStream* stream) {
	size_t bytes_recv;
	if(socket_has_data(stream->sock) &&
			socket_recv_all(stream->sock, (void*)stream->start, FILE_BUFFER_SIZE, &bytes_recv) >= 0) {
		stream->cursor = stream->start;
		stream->end = stream->start + bytes_recv;

		return stream->error;
	} else {
		return fail_stream(stream, EofError);
	}
}

static void destroy_socket_stream(SocketStream* stream) {
	socket_close(stream->sock);
	free((void*)stream->buffer);

	stream->buffer = NULL;
	stream->cursor = NULL;
	stream->start = NULL;
	stream->end = NULL;
	stream->sock = 0;
}

void create_socket_stream(SocketStream* stream, int sock) {
	stream->buffer = (const char*)malloc(FILE_BUFFER_SIZE);
	stream->cursor = stream->buffer;
	stream->start = stream->buffer;
	stream->end = stream->start + FILE_BUFFER_SIZE;

	stream->refill = (RefillFunc)refill_socket;
	stream->destroy = (DestroyFunc)destroy_socket_stream;

	stream->sock = sock;

	stream->refill(stream);
}
