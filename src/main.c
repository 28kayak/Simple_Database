#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "results.h"
#include "statement.h"
#include "input_buffer.h"

/*enum MetaCommandResult_t{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum MetaCommandResult_t MetaCommandResult;

enum PrepareResult_t{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_COMMAND
};
typedef enum PrepareResult_t PrepareResult; */

/*enum StatementType_t {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};
typedef enum StatementType_t StatementType;

struct Statement_t {
    StatementType type;
};
typedef struct Statement_t Statement; */

/*struct InputBuffer_t{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
};
typedef struct InputBuffer_t InputBuffer;*/

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    /*
     * NOTE: use strncmp to compare because insert keyword will be followed by data
     * (e.g. insert 1 cstack foo@bar.com)
     * */
    if(strncmp(input_buffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_COMMAND;
}//prepare_statement

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if (strcmp(input_buffer->buffer, ".exit") == 0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}
void exec_statement(Statement* statement){
    switch(statement->type){
        case (STATEMENT_INSERT):
            printf("This is where we would do an insert \n");
            break;

        case (STATEMENT_SELECT):
            printf("This is where we would do an select\n");
            break;
    }
}

InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}


void read_input(InputBuffer* inputBuffer){
    ssize_t bytes_read = getline(&(inputBuffer->buffer), &(inputBuffer->buffer_length), stdin);

    if(bytes_read <= 0 ){
        printf("Error reading input");
        exit(EXIT_FAILURE);
    }
    //ignore trainling new line
    inputBuffer->buffer_length = bytes_read - 1;
    inputBuffer->buffer[bytes_read - 1] = 0;
}
// frees the memoery allocated for an instance of InputBuffer
// and the buffer element of the respective structure 
// getline() allocate memory for input_buffer->buffer and read_input. 
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt(){
    printf("db > ");
}


int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.'){
            switch(do_meta_command(input_buffer)){
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s' .\n", input_buffer->buffer);
                    continue;

            }//switch
        }
        /*else{
            printf("Something wrong");

        }*/
        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_COMMAND):
                printf("Unrecognized keyword at start of '%s'.\n " ,input_buffer->buffer);
                continue;
        }//switch
        exec_statement(&statement);
        printf("Executed\n");

    }//while

    return 0;
}//main
