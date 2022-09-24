#include "socket_helper.h"

int SocketApiInitiate(void) {
#ifdef _WIN32
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data);
#endif
    return 0;
}

void SocketApiCleanUp(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

void SetLocalAddressInfo(char const *const port, struct addrinfo **const address) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       /* IPV4 */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = AI_PASSIVE;     /* for binding */
    getaddrinfo(NULL, port, &hints, address);
}

char *GetClientInfo(struct sockaddr *const client_address, socklen_t const client_len) {
    char address_buffer[100];
    getnameinfo(client_address, client_len, address_buffer, sizeof(address_buffer), NULL, 0, NI_NUMERICHOST);
    return address_buffer;
}
