#ifndef MOVIESERVER_SESSIONINFO_H
#define MOVIESERVER_SESSIONINFO_H

#include "../MovieEntities/MovieDatabase.h"
#include "../util/cJSON.h"

struct SessionInfo {
    char *client_name;
    struct MovieDatabase *database;
    char **company_list;
};

struct SessionInfo *SessionInfo_Create(char const *const client_name, struct MovieDatabase *database);
void SessionInfo_Destroy(struct SessionInfo *self);

cJSON *SessionInfo_ToJSON(struct SessionInfo *self);

#endif /*MOVIESERVER_SESSIONINFO_H*/
