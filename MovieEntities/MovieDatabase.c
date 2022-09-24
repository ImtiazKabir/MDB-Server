#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MovieDatabase.h"

#include "Movie.h"

#define STBDS_NO_SHORT_NAMES
#include "../util/stb_ds.h"

#include "../util/cJSON.h"

struct MovieDatabase *MovieDatabase_Create(char const *const csv_file_name) {
    struct MovieDatabase *self = (struct MovieDatabase *) malloc(sizeof(struct MovieDatabase));
    char csv_line[FILE_LINE_MAX];
    FILE *csv_file = fopen(csv_file_name, "r");

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for MovieDatabase\n");
        return NULL;
    }

    self->csv_file_name = strdup(csv_file_name);
    self->movie_list = NULL;

    if (csv_file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", csv_file_name);
        return NULL;
    }
    while (fgets(csv_line, sizeof(csv_line), csv_file) != NULL) {
        stbds_arrput(self->movie_list, Movie_Create(csv_line));
    }
    fclose(csv_file);
    return self;
}

void MovieDatabase_Destroy(struct MovieDatabase *self) {
    int i;

    if (self->csv_file_name != NULL) {
        free(self->csv_file_name);
    }
    for (i = 0; i < stbds_arrlen(self->movie_list); ++i) {
        Movie_Destroy(self->movie_list[i]);
    }
    stbds_arrfree(self->movie_list);

    free(self);
}

int MovieDatabase_DumpToFile(struct MovieDatabase *self) {
    FILE *csv_file = fopen(self->csv_file_name, "w");
    int i;
    if (csv_file == NULL) {
        fprintf(stderr, "Failed to write to file\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < stbds_arrlen(self->movie_list); ++i) {
        struct Movie *movie = self->movie_list[i];
        Movie_PrintToFile(movie, csv_file);
        fputc('\n', csv_file);
    }
    fclose(csv_file);
    return EXIT_SUCCESS;
}

struct MovieDatabase *MovieDatabase_FromList(struct Movie **movie_list) {
    struct MovieDatabase *self = (struct MovieDatabase *) malloc(sizeof(struct MovieDatabase));
    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for MovieDatabase\n");
        return NULL;
    }
    self->csv_file_name = NULL;
    self->movie_list = movie_list;
    return self;
}

void MovieDatabase_AddMovie(struct MovieDatabase *self, struct Movie *movie) {
    stbds_arrput(self->movie_list, movie);
}

struct MovieDatabase *MovieDatabase_ByCompany(struct MovieDatabase const *const db, char const *const company_name) {
    struct Movie **movie_list = NULL;
    int i;
    for (i = 0; i < stbds_arrlen(db->movie_list); ++i) {
        struct Movie *movie = db->movie_list[i];
        if (Movie_IsProducedBy(movie, company_name) == 1) {
            stbds_arrput(movie_list, Movie_Clone(movie));
        }
    }
    return MovieDatabase_FromList(movie_list);
}

void MovieDatabase_PrintToFile(struct MovieDatabase *self, FILE *file) {
    size_t i;
    for (i = 0; i < stbds_arrlenu(self->movie_list); ++i) {
        struct Movie *movie = self->movie_list[i];
        Movie_PrintToFile(movie, file);
        fputc('\n', file);
    }
}

cJSON *MovieDatabase_ToJSON(struct MovieDatabase *self) {
    cJSON *root = cJSON_CreateObject();
    cJSON *movies_json = cJSON_CreateArray();
    size_t i;

    for (i = 0; i < stbds_arrlenu(self->movie_list); ++i) {
        struct Movie const *movie = self->movie_list[i];
        cJSON_AddItemToArray(movies_json, Movie_ToJSON(movie));
    }

    cJSON_AddItemToObject(root, "movieList", movies_json);
    return root;
}

char **MovieDatabase_CompanyList(struct MovieDatabase const* self) {
    char **company_list = NULL;
    size_t i, j;
    for (i = 0; i < stbds_arrlenu(self->movie_list); ++i) {
        struct Movie *movie = self->movie_list[i];
        char *company = movie->production_company;
        for (j = 0; j < stbds_arrlenu(company_list); ++j) {
            if (strcmp(company_list[j], company) == 0) {
                break;
            }
        }
        if (j == stbds_arrlenu(company_list)) {
            stbds_arrput(company_list, strdup(company));
        }
    }
    return company_list;
}

void MovieDatabase_ChangeOwnership(struct MovieDatabase *self, char const *const title, char const *const new_owner) {
    size_t i;
    for (i = 0; i < stbds_arrlenu(self->movie_list); ++i) {
        struct Movie *movie = self->movie_list[i];
        if (strcmp(movie->title, title) == 0) {
            movie->production_company = strdup(new_owner);
            break;
        }
    }
}

