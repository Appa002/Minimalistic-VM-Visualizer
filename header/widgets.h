//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_WIDGETS_H
#define VM_VISUALIZER_WIDGETS_H

#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <ncurses/curses.h>
#include <locale.h>

#include "program.h"
#include "utils.h"

void write_keymap_line(){
    move(LINES - 1, 0);
    attrset(COLOR_PAIR(8));
    addstr("q(quit)");
    attrset(COLOR_PAIR(0));
    refresh();
}

void write_program(program_t* program, uint32_t start_line) {
    move(0, 0);
    attrset(COLOR_PAIR(7));
    for(size_t i = start_line; i < program->line_amount && (i - start_line) < LINES - 1; i++){
        char line_num_as_hex[32];
        sprintf(line_num_as_hex, "%#010x", program->lines[i]->line_num);

        addstr(line_num_as_hex);
        addstr(" ");
        addstr(program->lines[i]->instruction_name);
        addstr(" ");
        for(size_t j = 0; j < program->lines[i]->instruction_args_amount; j++){
            char arg_as_hex[8];
            sprintf(arg_as_hex, "%#04x", program->lines[i]->instruction_args[j]);
            addstr(arg_as_hex);
            addstr(" ");
        }
        move((int)(i - start_line) + 1, 0);
    }
    attrset(COLOR_PAIR(0));
    refresh();
}


void unmark_line_part(program_t* program, uint32_t start_line, uint32_t line, uint32_t element){
    move(line - start_line, 0);

    char line_num_as_hex[32];
    sprintf(line_num_as_hex, "%#010x", program->lines[line]->line_num);

    if_write_colored(element == 0, line_num_as_hex, 0);
    addstr(" ");
    if_write_colored(element == 1, program->lines[line]->instruction_name, 0);
    addstr(" ");

    for(size_t j = 0; j < program->lines[line]->instruction_args_amount; j++){
        char arg_as_hex[8];
        sprintf(arg_as_hex, "%#04x", program->lines[line]->instruction_args[j]);

        if_write_colored(element == 2 + j, arg_as_hex, 0);
        addstr(" ");
    }

    move(line - start_line, 0);
}

void rewrite_line(program_t* program, uint32_t start_line, uint32_t line){
    move(line - start_line, 0);

    char line_num_as_hex[32];
    sprintf(line_num_as_hex, "%#010x", program->lines[line]->line_num);

    write_colored(line_num_as_hex, 0);
    addstr(" ");
    write_colored(program->lines[line]->instruction_name, 0);
    addstr(" ");

    for(size_t j = 0; j < program->lines[line]->instruction_args_amount; j++){
        char arg_as_hex[8];
        sprintf(arg_as_hex, "%#04x", program->lines[line]->instruction_args[j]);

        write_colored(arg_as_hex, 0);
        addstr(" ");
    }

    move(line - start_line, 0);
}

void mark_line_part(program_t* program, uint32_t start_line, uint32_t line, int32_t* element){
    if((int64_t)line - 1 >= 0)
        rewrite_line(program, start_line, line - 1);
    if(line + 1 < program->line_amount && (line - start_line) + 1 < LINES - 1)
        rewrite_line(program, start_line, line + 1);

    if(*element < 0)
        *element = program->lines[line]->instruction_args_amount + 1;

    if(*element >= program->lines[line]->instruction_args_amount + 2)
        *element = 0;

    move(line - start_line, 0);

    char line_num_as_hex[32];
    sprintf(line_num_as_hex, "%#010x", program->lines[line]->line_num);

    if_write_colored(*element == 0, line_num_as_hex, 6);
    addstr(" ");
    if_write_colored(*element == 1, program->lines[line]->instruction_name, 6);
    addstr(" ");

    for(size_t j = 0; j < program->lines[line]->instruction_args_amount; j++){
        char arg_as_hex[8];
        sprintf(arg_as_hex, "%#04x", program->lines[line]->instruction_args[j]);

        if_write_colored(*element == 2 + j, arg_as_hex, 6);
        addstr(" ");
    }

    move(line - start_line, 0);
}

#endif