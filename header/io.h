//
// Created by a_mod on 02.06.2018.
//

#ifndef VM_VISUALIZER_IO_H
#define VM_VISUALIZER_IO_H

#include <stdlib.h>
#include <stdio.h>
#include "program.h"

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

int save_file(const char *filename, program_t *program){
    FILE* f = fopen(filename, "wb");
    if(f == NULL)
        return  - 1;

    uint8_t *raw_data = malloc(sizeof(uint8_t) * program->raw_size);
    line_t *line = NULL;
    uint32_t data_index = 0;

    for(size_t i = 0; i < program->line_amount; i++){
        line = program->lines[i];

        raw_data[data_index] = line->instruction_opt;
        for(size_t j = 0; j < line->instruction_args_amount; j++){
            raw_data[data_index + j + 1] = line->instruction_args[j];
        }
        data_index += line->line_raw_size;
    }

    if(fwrite(raw_data, sizeof(uint8_t), program->raw_size, f) < program->raw_size){
        free(raw_data);
        return  -2;
    }

    free(raw_data);
    fclose(f);
    return 0;
}

void replace_arg(uint32_t line_index, int element, program_t *program, uint8_t new_val){
    if(element < 2)
        return;
    program->lines[line_index]->instruction_args[element - 2] = new_val;
}

#endif //VM_VISUALIZER_IO_H
