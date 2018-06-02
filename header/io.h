//
// Created by a_mod on 02.06.2018.
//

#ifndef VM_VISUALIZER_IO_H
#define VM_VISUALIZER_IO_H

#include <stdlib.h>
#include <stdio.h>

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



#endif //VM_VISUALIZER_IO_H
