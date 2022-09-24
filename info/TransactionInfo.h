#ifndef MOVIESERVER_TRANSACTIONINFO_H
#define MOVIESERVER_TRANSACTIONINFO_H

#include "../MovieEntities/Movie.h"

struct TransactionInfo {
    struct Movie *movie;
    char *previous_owner;
    char *new_owner;
};

struct TransactionInfo *TransactionInfo_FromJSON(char const *const json);
void TransactionInfo_Destroy(struct TransactionInfo *self);

#endif /*MOVIESERVER_TRANSACTIONINFO_H*/
