#include "Movie.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../util/cJSON.h"

static void AddField(struct Movie *self, size_t index, char const *const field_str);

struct Movie *Movie_Create(char const *const csv) {
    struct Movie *self = (struct Movie *) malloc(sizeof(struct Movie));
    size_t i = 0;
    char const *curr = (char *) csv;
    size_t sz = 0;

    char field_str[MOVIE_MAX_FIELD_LEN];

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for Movie\n");
        return NULL;
    }

    self->genres = (char **) malloc(3 * sizeof(char *));
    if (self->genres == NULL) {
        fprintf(stderr, "Failed to allocate memory for genres array");
        free(self);
        return NULL;
    }

    for (i = 0; i < MOVIE_NUM_OF_FIELDS; ++i) {
        sz = strcspn(curr, ",");
        strncpy(field_str, curr, sz);
        if (i == MOVIE_NUM_OF_FIELDS - 1) {
            field_str[sz-1] = '\0';
        } else {
            field_str[sz] = '\0';
        }
        AddField(self, i, field_str);
        curr += sz + 1;
    }

    return self;
}

struct Movie *Movie_Clone(struct Movie const *const movie) {
    struct Movie *self = (struct Movie *) malloc(sizeof(struct Movie));

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for Movie\n");
        return NULL;
    }

    self->genres = (char **) malloc(3 * sizeof(char *));

    if (self->genres == NULL) {
        fprintf(stderr, "Failed to allocate memory for genres array");
        free(self);
        return NULL;
    }

    self->title = strdup(movie->title);
    self->year_of_release = movie->year_of_release;
    self->genres[0] = strdup(movie->genres[0]);
    self->genres[1] = strdup(movie->genres[1]);
    self->genres[2] = strdup(movie->genres[2]);
    self->running_time = movie->running_time;
    self->production_company = strdup(movie->production_company);
    self->budget = movie->budget;
    self->revenue = movie->revenue;

    return self;
}

struct Movie *Movie_FromJSON(char const *const json) {
    struct Movie *self = (struct Movie *) malloc(sizeof(struct Movie));
    cJSON *root = cJSON_Parse(json);
    cJSON const *title_item = cJSON_GetObjectItem(root, "title");
    cJSON const *year_of_release_item = cJSON_GetObjectItem(root, "yearOfRelease");
    cJSON const *genres_item = cJSON_GetObjectItem(root, "genres");
    cJSON const *running_time_item = cJSON_GetObjectItem(root, "runningTime");
    cJSON const *production_company_item = cJSON_GetObjectItem(root, "productionCompany");
    cJSON const *budget_item = cJSON_GetObjectItem(root, "budget");
    cJSON const *revenue_item = cJSON_GetObjectItem(root, "revenue");

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for Movie\n");
        return NULL;
    }

    self->genres = (char **) malloc(3 * sizeof(char *));

    if (self->genres == NULL) {
        fprintf(stderr, "Failed to allocate memory for genres array");
        free(self);
        return NULL;
    }

    self->title = strdup(cJSON_GetStringValue(title_item));
    self->year_of_release = strtol(cJSON_GetStringValue(year_of_release_item), NULL, 10);
    self->genres[0] = strdup(cJSON_GetStringValue(cJSON_GetArrayItem(genres_item, 0)));
    self->genres[1] = strdup(cJSON_GetStringValue(cJSON_GetArrayItem(genres_item, 1)));
    self->genres[2] = strdup(cJSON_GetStringValue(cJSON_GetArrayItem(genres_item, 2)));
    self->running_time = strtol(cJSON_GetStringValue(running_time_item), NULL, 10);
    self->production_company = strdup(cJSON_GetStringValue(production_company_item));
    self->budget = strtoul(cJSON_GetStringValue(budget_item), NULL, 10);
    self->revenue = strtoul(cJSON_GetStringValue(revenue_item), NULL, 10);

    cJSON_Delete(root);
    return self;
}

void Movie_Destroy(struct Movie *self) {
    free(self->title);
    free(self->genres[0]);
    free(self->genres[1]);
    free(self->genres[2]);
    free(self->genres);
    free(self->production_company);
    free(self);
}

static void AddField(struct Movie *self, size_t index, char const *const field_str) {
    switch (index) {
        case MOVIE_TITLE: {
            self->title = strdup(field_str);
            break;
        }
        case MOVIE_YEAR_OF_RELEASE: {
            self->year_of_release = strtol(field_str, NULL, 10);
            break;
        }
        case MOVIE_GENRE1: {
            self->genres[0] = strdup(field_str);
            break;
        }
        case MOVIE_GENRE2: {
            self->genres[1] = strdup(field_str);
            break;
        }
        case MOVIE_GENRE3: {
            self->genres[2] = strdup(field_str);
            break;
        }
        case MOVIE_RUNNING_TIME: {
            self->running_time = strtol(field_str, NULL, 10);
            break;
        }
        case MOVIE_PRODUCTION_COMPANY: {
            self->production_company = strdup(field_str);
            break;
        }
        case MOVIE_BUDGET: {
            self->budget = strtoul(field_str, NULL, 10);
            break;
        }
        case MOVIE_REVENEUE: {
            self->revenue = strtoul(field_str, NULL, 10);
            break;
        }
        default: {
            fprintf(stderr, "Invalid field index\n");
            break;
        }
    }
}

void Movie_PrintToFile(struct Movie *self, FILE *file) {
    fprintf(file, "%s,%d,%s,%s,%s,%d,%s,%lu,%lu", self->title, self->year_of_release, self->genres[0],
            self->genres[1], self->genres[2], self->running_time, self->production_company, self->budget,
            self->revenue);
}

int Movie_IsProducedBy(struct Movie const *self, char const *const production_company) {
    if (strcmp(self->production_company, production_company) == 0) {
        return 1;
    } else {
        return 0;
    }
}

cJSON *Movie_ToJSON(struct Movie const *self) {
    cJSON *root;
    cJSON *genres;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", self->title);
    cJSON_AddNumberToObject(root, "yearOfRelease", self->year_of_release);
    genres = cJSON_CreateArray();
    cJSON_AddItemToArray(genres, cJSON_CreateString(self->genres[0]));
    cJSON_AddItemToArray(genres, cJSON_CreateString(self->genres[1]));
    cJSON_AddItemToArray(genres, cJSON_CreateString(self->genres[2]));
    cJSON_AddItemToObject(root, "genres", genres);
    cJSON_AddNumberToObject(root, "runningTime", self->running_time);
    cJSON_AddStringToObject(root, "productionCompany", self->production_company);
    cJSON_AddNumberToObject(root, "budget", self->budget);
    cJSON_AddNumberToObject(root, "revenue", self->revenue);

    return root;
}
