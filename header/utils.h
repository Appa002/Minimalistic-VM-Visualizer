//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_UTILS_H
#define VM_VISUALIZER_UTILS_H

#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>

char* create_string(const char* content){
    char* out = malloc(sizeof(char) * strlen(content));
    strcpy(out, content);
    return out;
}

void write_colored(const char* str, int color){
    attrset(COLOR_PAIR(color));
    addstr(str);
    attrset(COLOR_PAIR(0));
}

void if_write_colored(bool condition, const char* str, int color){
    if(condition)
        attrset(COLOR_PAIR(color));
    addstr(str);
    attrset(COLOR_PAIR(0));
}

#endif //VM_VISUALIZER_UTILS_H
