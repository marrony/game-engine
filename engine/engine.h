/*
 * engine.h
 *
 *  Created on: 02/04/2013
 *      Author: marrony
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <GL/gl.h>
typedef HWND WindowID;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <GL/gl.h>
#include <GL/glx.h>
typedef Window WindowID;
#endif

const WindowID WINDOW_NULL = (WindowID)-1;


#endif /* ENGINE_H_ */
