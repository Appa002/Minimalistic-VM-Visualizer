//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_HELPER_H
#define VM_VISUALIZER_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#include "program.h"
#include "instructions.h"

int load_file(const char *filename, unsigned char **result, size_t* size) {
    *size = 0;
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        *result = NULL;
        return -1;
    }
    fseek(f, 0, SEEK_END);
    *size = (unsigned int)ftell(f);
    fseek(f, 0, SEEK_SET);
    *result = (unsigned char *)malloc(*size+1);
    if (*size != fread(*result, sizeof(char), *size, f))
    {
        free(*result);
        return -2; // -2 means file reading fail
    }
    fclose(f);
    (*result)[*size] = 0;
    return 0;
}

program_t* generate_program(const char* filename){
    unsigned char* raw_data;
    size_t size;
    if(load_file(filename, &raw_data, &size) != 0)
        exit(3);

    program_t* out = malloc(sizeof(program_t));
    out->raw_size = (uint32_t)size;
    out->lines = (line_t**)malloc(sizeof(line_t*) * size);

    uint8_t* ip = raw_data;
    instruction* opt = malloc(sizeof(instruction) * 256);
    register_instructions(opt);

    uint32_t i = 0;
    while (*ip != 'h'){
        line_t* line = malloc(sizeof(line_t));
        line->line_num = (uint32_t)(ip - raw_data);

        ip = opt[*ip](ip, line);
        out->lines[i] = line;
        i++;
    }
    out->line_amount = i;
    return out;
}

void delete_program(program_t* program){
    for(uint32_t i = 0; i < program->line_amount; i++){
        if(program->lines[i]->instruction_args_amount != 0)
            free(program->lines[i]->instruction_args);
        free(program->lines[i]->instruction_name);
        free(program->lines[i]);
    }
    free(program);
}

#endif //VM_VISUALIZER_HELPER_H
