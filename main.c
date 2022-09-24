#include "Server.h"

#define STBDS_NO_SHORT_NAMES
#define STB_DS_IMPLEMENTATION

#include "util/stb_ds.h"


int main(void) {
    struct Server *server = Server_Create();
    Server_ConfigureLocalAddress(server);
    Server_CreateListeningSocket(server);
    Server_BindToAddress(server);
    Server_Listen(server);
    Server_PrepareMainLoop(server);
    while (1) {
        Server_MainLoop(server);
    }
    Server_Destroy(server);
    return EXIT_SUCCESS;
}
