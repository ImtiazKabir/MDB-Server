#ifndef MOVIESERVER_SERVER_H
#define MOVIESERVER_SERVER_H

#include "MovieEntities/MovieDatabase.h"
#include "util/socket_helper.h"

#define STBDS_NO_SHORT_NAMES
#include "util/stb_ds.h"

#define PORT "8080"
#define READ_BUFFER_LEN 1024

struct map_entity {
    char *key;
    SOCKET value;
};

struct Server {
    struct MovieDatabase *database;
    struct addrinfo *bind_address;
    SOCKET socket_listen;
    fd_set master;
    SOCKET max_socket;
    struct map_entity *name_to_socket_map;
};

struct Server *Server_Create(void);
void Server_Destroy(struct Server *self);
void Server_ConfigureLocalAddress(struct Server *self);
void Server_CreateListeningSocket(struct Server *self);
void Server_BindToAddress(struct Server *self);
void Server_Listen(struct Server *self);
void Server_PrepareMainLoop(struct Server *self);
void Server_MainLoop(struct Server *self);

#endif /*MOVIESERVER_SERVER_H*/
