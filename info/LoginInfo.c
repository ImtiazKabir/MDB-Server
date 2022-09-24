#include "LoginInfo.h"
#include "../util/cJSON.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct LoginInfo *LoginInfo_FromJSON(char const *const json) {
    struct LoginInfo *self = (struct LoginInfo *) malloc(sizeof(struct LoginInfo));
    cJSON *root = cJSON_Parse(json);
    cJSON const *username_item = cJSON_GetObjectItem(root, "username");
    cJSON const *password_item = cJSON_GetObjectItem(root, "password");

    if (self == NULL) {
        fprintf(stderr, "Failed to allocate memory for LoginInfo\n");
        return NULL;
    }

    self->username = strdup(cJSON_GetStringValue(username_item));
    self->password = strdup(cJSON_GetStringValue(password_item));

    cJSON_Delete(root);
    return self;
}

void LoginInfo_Destroy(struct LoginInfo *self) {
    free(self->username);
    free(self->password);
    free(self);
}

int LoginInfo_IsValid(struct LoginInfo const *self) {
    /* This function is supposed to authenticate the login information */
    /* For now just checking if username and password are equal */
    if (strcmp(self->username, self->password) == 0) {
        return 1;
    }
    return 0;
}
