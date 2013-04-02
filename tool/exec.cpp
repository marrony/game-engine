/*
 * exec.c
 *
 *  Created on: 09/02/2013
 *      Author: marrony
 */
#include "exec.h"

//#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <string.h>
//#include <pthread.h>
//#include <signal.h>
//#include <errno.h>

#ifdef WIN32
#include <io.h>
#include <process.h>
#endif

#define READ 0
#define WRITE 1

#define PARENT_READ  read_pipe[READ]
#define CHILD_WRITE  read_pipe[WRITE]

#define CHILD_READ   write_pipe[READ]
#define PARENT_WRITE write_pipe[WRITE]

#ifdef WIN32
int pipe(int fd[2]) {
	return _pipe(fd, 256, O_BINARY | O_NOINHERIT);
}
#endif

int execute_program(const char* const argv[], FILE** fr, FILE** fw) {
	int read_pipe[2];
	int write_pipe[2];

	pipe(read_pipe);
	pipe(write_pipe);

	int fdstdin = dup(STDIN_FILENO);
	int fdstdout = dup(STDOUT_FILENO);

	if(dup2(CHILD_READ, STDIN_FILENO) < 0)
		return 1;

	if(dup2(CHILD_WRITE, STDOUT_FILENO) < 0)
		return 1;

	close(CHILD_READ);
	close(CHILD_WRITE);

#ifdef WIN32
	int id = _spawnvp(P_NOWAIT, argv[0], (const char* const*)argv);
#else
	int id = fork();

	if(id == 0) {
		execvp(argv[0], (char* const*)argv);
		exit(0);
	}
#endif

	if(id < 0)
		return 1;

	if(dup2(fdstdin, STDIN_FILENO) < 0)
		return 1;

	if(dup2(fdstdout, STDOUT_FILENO) < 0)
		return 1;

	close(fdstdin);
	close(fdstdout);

	if(fr)
		*fr = fdopen(PARENT_READ, "r");
	else
		close(PARENT_READ);

	if(fw)
		*fw = fdopen(PARENT_WRITE, "w");
	else
		close(PARENT_WRITE);

	return 0;
}
