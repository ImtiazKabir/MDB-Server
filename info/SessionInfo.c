#include "SessionInfo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../MovieEntities/MovieDatabase.h"

#define STBDS_NO_SHORT_NAMES
#include "../util/stb_ds.h"
#include "../util/cJSON.h"


struct SessionInfo *SessionInfo_Create(char const *const client_name, struct MovieDatabase *database) {
    struct SessionInfo *self = (struct SessionInfo *) malloc(sizeof(struct SessionInfo));
    size_t i;
    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for SessionInfo");
        return NULL;
    }

    self->client_name = strdup(client_name);
    self->database = MovieDatabase_ByCompany(database, client_name);
    self->company_list = MovieDatabase_CompanyList(database);

    return self;
}

void SessionInfo_Destroy(struct SessionInfo *self) {
    size_t i;
    free(self->client_name);
    MovieDatabase_Destroy(self->database);
    for (i = 0; i < stbds_arrlenu(self->company_list); ++i) {
        free(self->company_list[i]);
    }
    stbds_arrfree(self->company_list);
}

cJSON *SessionInfo_ToJSON(struct SessionInfo *self) {
    cJSON *root;
    cJSON *companies;
    size_t i;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "clientName", self->client_name);
    cJSON_AddItemToObject(root, "database", MovieDatabase_ToJSON(self->database));
    companies = cJSON_CreateArray();
    for (i = 0; i < stbds_arrlenu(self->company_list); ++i) {
        cJSON_AddItemToArray(companies, cJSON_CreateString(self->company_list[i]));
    }
    cJSON_AddItemToObject(root, "companyList", companies);

    return root;
}
