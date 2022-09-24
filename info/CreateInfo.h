#ifndef MOVIESERVER_CREATEINFO_H
#define MOVIESERVER_CREATEINFO_H

#include "../MovieEntities/Movie.h"
#include "../util/cJSON.h"

struct CreateInfo {
    struct Movie *movie;
};

struct CreateInfo *CreateInfo_Create(struct Movie *movie);
struct CreateInfo *CreateInfo_FromJSON(char const *const json);
void CreateInfo_Destroy(struct CreateInfo *self);
cJSON *CreateInfo_ToJSON(struct CreateInfo *self);


#endif /*MOVIESERVER_CREATEINFO_H*/
