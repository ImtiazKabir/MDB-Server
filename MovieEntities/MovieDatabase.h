#ifndef MOVIESERVER_MOVIEDATABASE_H
#define MOVIESERVER_MOVIEDATABASE_H

#include "Movie.h"

#define FILE_LINE_MAX 256

struct MovieDatabase {
    char *csv_file_name;
    struct Movie **movie_list;
};

struct MovieDatabase *MovieDatabase_Create(char const *const csv_file_name);
struct MovieDatabase *MovieDatabase_FromList(struct Movie **movie_list);
void MovieDatabase_Destroy(struct MovieDatabase *self);

void MovieDatabase_AddMovie(struct MovieDatabase *self, struct Movie *movie);
struct MovieDatabase *MovieDatabase_ByCompany(struct MovieDatabase const *const db, char const *const company_name);
int MovieDatabase_DumpToFile(struct MovieDatabase *self);
void MovieDatabase_PrintToFile(struct MovieDatabase *self, FILE *file);
cJSON *MovieDatabase_ToJSON(struct MovieDatabase *self);
char **MovieDatabase_CompanyList(struct MovieDatabase const* self);
void MovieDatabase_ChangeOwnership(struct MovieDatabase *self, char const *const title, char const *const new_owner);

#endif /*MOVIESERVER_MOVIEDATABASE_H*/
