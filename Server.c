#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/socket_helper.h"

#define STBDS_NO_SHORT_NAMES

#include "util/stb_ds.h"

#include "info/LoginInfo.h"
#include "info/SessionInfo.h"
#include "info/TransactionInfo.h"
#include "info/CreateInfo.h"

static void AcceptConnection(struct Server *self);

static void ReadFromClient(struct Server *self, SOCKET socket_client);

static void ProcessLoginInfo(struct Server *self, SOCKET socket_client, struct LoginInfo const *login_info);

static void ProcessTransactionInfo(struct Server *self, SOCKET socket_client, struct TransactionInfo const
*transaction_info);

static void ProcessCreateInfo(struct Server *self, SOCKET socket_client, struct CreateInfo const *create_info);

struct Server *Server_Create(void) {
    struct Server *self = (struct Server *) malloc(sizeof(struct Server));
    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for Server");
        return NULL;
    }
    if (SocketApiInitiate() != 0) {
        LOGERROR("SocketInitiate");
        exit(EXIT_FAILURE);
    }
    self->database = MovieDatabase_Create("movies.csv");
    self->name_to_socket_map = NULL;
    return self;
}

void Server_Destroy(struct Server *self) {
    MovieDatabase_DumpToFile(self->database);
    MovieDatabase_Destroy(self->database);
            stbds_shfree(self->name_to_socket_map);
    freeaddrinfo(self->bind_address);
    CLOSESOCKET(self->socket_listen);
    SocketApiCleanUp();
}

void Server_ConfigureLocalAddress(struct Server *self) {
    SetLocalAddressInfo(PORT, &(self->bind_address));
}

void Server_CreateListeningSocket(struct Server *self) {
    int af = self->bind_address->ai_family;
    int type = self->bind_address->ai_socktype;
    int protocol = self->bind_address->ai_protocol;
    self->socket_listen = socket(af, type, protocol);
    if (!ISVALIDSOCKET(self->socket_listen)) {
        LOGERROR("socket");
        exit(EXIT_FAILURE);
    }
}

void Server_BindToAddress(struct Server *self) {
    if (bind(self->socket_listen, self->bind_address->ai_addr, self->bind_address->ai_addrlen) != 0) {
        LOGERROR("bind");
        exit(EXIT_FAILURE);
    }
}


void Server_Listen(struct Server *self) {
    if (listen(self->socket_listen, 10) != 0) {
        LOGERROR("listen");
        exit(EXIT_FAILURE);
    }
}

void Server_PrepareMainLoop(struct Server *self) {
    FD_ZERO(&(self->master));
    FD_SET(self->socket_listen, &(self->master));
    self->max_socket = self->socket_listen;
}


void Server_MainLoop(struct Server *self) {
    fd_set reads;
    SOCKET i;
    reads = self->master;
    if (select(self->max_socket + 1, &reads, NULL, NULL, 0) < 0) {
        LOGERROR("select");
        exit(EXIT_FAILURE);
    }
    for (i = 1; i <= self->max_socket; ++i) {
        if (FD_ISSET(i, &reads)) {
            if (i == self->socket_listen) {
                AcceptConnection(self);
            } else {
                ReadFromClient(self, i);
            }
        }
    }
}


static void AcceptConnection(struct Server *self) {
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(self->socket_listen, (struct sockaddr *) &client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client)) {
        LOGERROR("accept");
        exit(EXIT_FAILURE);
    }

    FD_SET(socket_client, &(self->master));
    if (socket_client > self->max_socket) {
        self->max_socket = socket_client;
    }

    puts(GetClientInfo((struct sockaddr *) &client_address, client_len));
}


static void ReadFromClient(struct Server *self, SOCKET socket_client) {
    char read[READ_BUFFER_LEN];
    cJSON *root;
    char *command;

    int bytes_received = recv(socket_client, read, READ_BUFFER_LEN, 0);
    if (bytes_received < 1) {
        size_t i;
        for (i = 0; i < stbds_shlenu(self->name_to_socket_map); ++i) {
            if (self->name_to_socket_map[i].value == socket_client) {
                char *key = self->name_to_socket_map[i].key;
                stbds_shdel(self->name_to_socket_map, key);
                break;
            }
        }
        FD_CLR(socket_client, &(self->master));
        CLOSESOCKET(socket_client);
        return;
    }
    root = cJSON_Parse(read);
    command = cJSON_GetStringValue(cJSON_GetObjectItem(root, "command"));
    if (strcmp(command, "login") == 0) {
        cJSON const *login_json = cJSON_GetObjectItem(root, "content");
        char *json = cJSON_Print(login_json);
        struct LoginInfo *login_info = LoginInfo_FromJSON(json);
        ProcessLoginInfo(self, socket_client, login_info);
        free(json);
        LoginInfo_Destroy(login_info);
    } else if (strcmp(command, "transaction") == 0) {
        cJSON const *transaction_json = cJSON_GetObjectItem(root, "transaction");
        char *json = cJSON_Print(transaction_json);
        struct TransactionInfo *transaction_info = TransactionInfo_FromJSON(json);
        ProcessTransactionInfo(self, socket_client, transaction_info);
        free(json);
        TransactionInfo_Destroy(transaction_info);
    } else if (strcmp(command, "create") == 0) {
        cJSON const *create_json = cJSON_GetObjectItem(root, "transaction");
        char *json = cJSON_Print(create_json);
        struct CreateInfo *create_info = CreateInfo_FromJSON(json);
        ProcessCreateInfo(self, socket_client, create_info);
        free(json);
        CreateInfo_Destroy(create_info);
    }
    cJSON_Delete(root);
}

static void ProcessLoginInfo(struct Server *self, SOCKET socket_client, struct LoginInfo const *login_info) {
    struct SessionInfo *session_info;
    cJSON *session_json;
    cJSON *root;
    char const *response;

    session_info = SessionInfo_Create(login_info->username, self->database);
    if (LoginInfo_IsValid(login_info) == 0) {
        puts("Invalid credentials from client");
        if (session_info->database->movie_list != NULL) {
            stbds_arrfree(session_info->database->movie_list);
        }
    } else {
        stbds_shput(self->name_to_socket_map, strdup(login_info->username), socket_client);
    }
    session_json = SessionInfo_ToJSON(session_info);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "command", cJSON_CreateString("session"));
    cJSON_AddItemToObject(root, "content", session_json);
    response = cJSON_Print(root);
    send(socket_client, response, strlen(response), 0);
    cJSON_Delete(session_json);
}

static void ProcessTransactionInfo(struct Server *self, SOCKET socket_client, struct TransactionInfo const
*transaction_info) {
    struct Movie *movie = transaction_info->movie;
    struct CreateInfo *create_info;
    cJSON *create_json;
    cJSON *root;
    char const *response;
    SOCKET receiver;

    free(movie->production_company);
    movie->production_company = strdup(transaction_info->new_owner);

    MovieDatabase_ChangeOwnership(self->database, movie->title, transaction_info->new_owner);
    MovieDatabase_DumpToFile(self->database);

    create_info = CreateInfo_Create(movie);
    create_json = CreateInfo_ToJSON(create_info);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "command", cJSON_CreateString("create"));
    cJSON_AddItemToObject(root, "content", create_json);
    response = cJSON_Print(root);
    receiver = stbds_shget(self->name_to_socket_map, transaction_info->new_owner);
    send(receiver, response, strlen(response), 0);
    cJSON_Delete(create_json);
    CreateInfo_Destroy(create_info);
}

static void ProcessCreateInfo(struct Server *self, SOCKET socket_client, struct CreateInfo const *create_info) {
    MovieDatabase_AddMovie(self->database, Movie_Clone(create_info->movie));
    MovieDatabase_DumpToFile(self->database);
}
