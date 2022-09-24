#include "TransactionInfo.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../MovieEntities/Movie.h"


struct TransactionInfo *TransactionInfo_FromJSON(char const *const json) {
    struct TransactionInfo *self = (struct TransactionInfo *) malloc(sizeof(struct TransactionInfo));
    cJSON *root = cJSON_Parse(json);
    cJSON const *movie_item = cJSON_GetObjectItem(root, "movie");
    cJSON const *previous_owner_item = cJSON_GetObjectItem(root, "previousOwner");
    cJSON const *new_owner_item = cJSON_GetObjectItem(root, "newOwnerItem");
    char *movie_json;

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for TransactionInfo\n");
        return NULL;
    }

    movie_json = cJSON_Print(movie_item);

    self->movie = Movie_FromJSON(movie_json);
    self->previous_owner = strdup(cJSON_GetStringValue(previous_owner_item));
    self->new_owner = strdup(cJSON_GetStringValue(new_owner_item));

    free(movie_json);
    cJSON_Delete(root);
    return self;
}


void TransactionInfo_Destroy(struct TransactionInfo *self) {
    Movie_Destroy(self->movie);
    free(self->previous_owner);
    free(self->new_owner);
    free(self);
}


