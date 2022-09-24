#ifndef MOVIESERVER_SOCKET_HELPER_H
#define MOVIESERVER_SOCKET_HELPER_H

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

#ifdef _WIN32
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

#define LOGERROR(funcstr) \
  fprintf(stderr, funcstr "() failed. (%d)\n", GETSOCKETERRNO());


int SocketApiInitiate(void);
void SocketApiCleanUp(void);
void SetLocalAddressInfo(char const *const port, struct addrinfo **const address);
char *GetClientInfo(struct sockaddr *const client_address, socklen_t const client_len);

#endif /*MOVIESERVER_SOCKET_HELPER_H*/
