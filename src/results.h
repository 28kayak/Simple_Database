//
// Created by Kaya Ota on 2018-10-17.
//

#ifndef SIMPLE_BATABASE_RESULTS_H
#define SIMPLE_BATABASE_RESULTS_H

#endif //SIMPLE_BATABASE_RESULTS_H

enum MetaCommandResult_t{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum MetaCommandResult_t MetaCommandResult;

enum PrepareResult_t{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_COMMAND,
    PREPARE_SYNTAX_ERROR
};
typedef enum PrepareResult_t PrepareResult;

enum ExecuteResult_t{
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
};
typedef enum ExecuteResult_t ExecuteResult;

