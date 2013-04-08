/*
 * stream.h
 *
 *  Created on: 08/04/2013
 *      Author: marrony
 */

#include <stddef.h>

enum StreamError {
	NoError,
	EofError,
};

struct Stream;

typedef StreamError (*RefillFunc)(Stream* stream);
typedef void (*DestroyFunc)(Stream* stream);

struct Stream {
	const char* start;
	const char* end;
	const char* cursor;

	StreamError error;

	RefillFunc refill;
	DestroyFunc destroy;
};

struct FileStream : public Stream {
	const char* buffer;
	int fd;
};

struct SocketStream : public Stream {
	const char* buffer;
	int sock;
};

void create_memory_stream(Stream* stream, const char* buffer, size_t size);
void create_file_stream(FileStream* stream, const char* filename);
void create_socket_stream(SocketStream* stream, int sock);
