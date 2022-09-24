#ifndef MOVIESERVER_MOVIE_H
#define MOVIESERVER_MOVIE_H

#include <stdint.h>
#include <stdio.h>

#include "../util/cJSON.h"

#define MOVIE_MAX_FIELD_LEN 100

struct Movie {
    char *title;
    int year_of_release;
    char **genres;
    int running_time;
    char *production_company;
    unsigned long budget;
    unsigned long revenue;
};

enum CSVIndex {
    MOVIE_TITLE, MOVIE_YEAR_OF_RELEASE, MOVIE_GENRE1, MOVIE_GENRE2, MOVIE_GENRE3, MOVIE_RUNNING_TIME,
    MOVIE_PRODUCTION_COMPANY, MOVIE_BUDGET, MOVIE_REVENEUE, MOVIE_NUM_OF_FIELDS
};

struct Movie *Movie_Create(char const *const csv);
struct Movie *Movie_Clone(struct Movie const *const movie);
struct Movie *Movie_FromJSON(char const *const json);
void Movie_Destroy(struct Movie *self);

void Movie_PrintToFile(struct Movie *self, FILE * file);
int Movie_IsProducedBy(struct Movie const *self, char const *const production_company);

cJSON *Movie_ToJSON(struct Movie const *self);

#endif /*MOVIESERVER_MOVIE_H*/
