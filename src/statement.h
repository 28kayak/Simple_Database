//
// Created by Kaya Ota on 2018-10-17.
//

#ifndef SIMPLE_BATABASE_STATEMENT_H
#define SIMPLE_BATABASE_STATEMENT_H

#endif //SIMPLE_BATABASE_STATEMENT_H

enum StatementType_t {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};
typedef enum StatementType_t StatementType;

struct Statement_t {
    StatementType type;
};
typedef struct Statement_t Statement;

