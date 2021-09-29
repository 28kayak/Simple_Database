//
// Created by Kaya Ota on 2018-10-17.
//

#ifndef SIMPLE_BATABASE_STATEMENT_H
#define SIMPLE_BATABASE_STATEMENT_H
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255 
#endif //SIMPLE_BATABASE_STATEMENT_H

enum StatementType_t {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};
typedef enum StatementType_t StatementType;

typedef struct{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

struct Statement_t {
    StatementType type;
    Row row_to_insert; // only used by insert statement
};
typedef struct Statement_t Statement;

