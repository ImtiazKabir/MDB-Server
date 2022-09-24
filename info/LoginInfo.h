#ifndef MOVIESERVER_LOGININFO_H
#define MOVIESERVER_LOGININFO_H

struct LoginInfo {
    char *username;
    char *password; /* DANGER: password being stored as plain text! */
};


struct LoginInfo *LoginInfo_FromJSON(char const *const json);
void LoginInfo_Destroy(struct LoginInfo *self);
int LoginInfo_IsValid(struct LoginInfo const *self);

#endif /*MOVIESERVER_LOGININFO_H*/
