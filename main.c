#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct InputBuffer_t{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
};
typedef struct InputBuffer_t InputBuffer;

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


void print_prompt(){
    printf("db > ");
}


int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(inputBuffer);

        if(strcmp(inputBuffer->buffer, ".exit") == 0){
            exit(EXIT_SUCCESS);
        }
        else{
            printf("Unrecognized command '%s' .\n", inputBuffer->buffer);
        }
    }//while

    return 0;
}//main
