#include "CreateInfo.h"

#include <stdio.h>
#include <stdlib.h>

#include "../MovieEntities/Movie.h"
#include "../util/cJSON.h"

struct CreateInfo *CreateInfo_Create(struct Movie *movie) {
    struct CreateInfo *self = (struct CreateInfo *) malloc(sizeof(struct CreateInfo));

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for CreateInfo\n");
        return NULL;
    }

    self->movie = Movie_Clone(movie);
    return self;
}

struct CreateInfo *CreateInfo_FromJSON(char const *const json) {
    struct CreateInfo *self = (struct CreateInfo *) malloc(sizeof(struct CreateInfo));

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for CreateInfo\n");
        return NULL;
    }

    self->movie = Movie_FromJSON(json);
    return self;
}

void CreateInfo_Destroy(struct CreateInfo *self) {
    Movie_Destroy(self->movie);
    free(self);
}

cJSON *CreateInfo_ToJSON(struct CreateInfo *self) {
    cJSON *root;
    root = Movie_ToJSON(self->movie);
    return root;
}
