//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_UTILS_H
#define VM_VISUALIZER_UTILS_H

#include <memory.h>
#include <stdlib.h>

char* create_string(const char* content){
    char* out = malloc(sizeof(char) * strlen(content));
    strcpy(out, content);
    return out;
}

#endif //VM_VISUALIZER_UTILS_H
