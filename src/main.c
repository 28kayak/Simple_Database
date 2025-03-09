#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "results.h"
#include "statement.h"
#include "input_buffer.h"

// Compact Representation of a Row 
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)-> Attribute)
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
//uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

/*
* A table structure that points to pages of rows and 
* keep tracking of how many rows there are:
*/
const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct{
    const uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
} Table;

void serialize_row(Row* source, void* destination){
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}
void deserialize_row(void* source, Row* destination){
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* row_slot(Table* table, uint32_t row_num){
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if(page == NULL){
        //Allocate memory only when we try to access page 
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    /*
     * NOTE: use strncmp to compare because insert keyword will be followed by data
     * (e.g. insert 1 cstack foo@bar.com)
     * */
    if(strncmp(input_buffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
            statement->row_to_insert.username, statement->row_to_insert.email);
        if(args_assigned < 3){
            //if arguments are more then 3
            return PREPARE_SYNTAX_ERROR;
        }
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
ExecuteResult execute_insert(Statement* statement, Table* table){
    if(table->num_rows >= TABLE_MAX_ROWS){
        return EXECUTE_TABLE_FULL;
    }
    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows +=1;
    return EXECUTE_SUCCESS;
}
ExecuteResult execute_select(Statement* statement, Table* table){
    Row row; 
    for(uint32_t i = 0; i < table->num_rows; i++){
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult exec_statement(Statement* statement, Table* table){
    switch(statement->type){
        case (STATEMENT_INSERT):
            //printf("This is where we would do an insert \n");
            return execute_insert(statement, table);
        case (STATEMENT_SELECT):
            //printf("This is where we would do an select\n");
            return execute_select(statement, table);
    }
}

InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}
Table* new_table(){
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++ ){
        table->pages[i] = NULL;
    }
    return table;
}
void free_table(Table* table){
    for(int i = 0; table->pages[i]; i++){
        free(table->pages[i]);
    }
    free(table);
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
    Table* table = new_table();
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
        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax Error: could not parse statement. \n");
            case (PREPARE_UNRECOGNIZED_COMMAND):
                printf("Unrecognized keyword at start of '%s'.\n " ,input_buffer->buffer);
                continue;
        }//switch
        switch(exec_statement(&statement, table)){
            case(EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case(EXECUTE_TABLE_FULL):
                printf("Error: Table is FULL");
                break;
        }//switch
        
        printf("Executed\n");
    }//while
    return 0;
}//main

