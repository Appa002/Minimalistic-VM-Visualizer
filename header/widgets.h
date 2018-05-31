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
    write_colored("q(quit)", 8);
    write_colored(" ", 0);
    write_colored("up arrow(scroll up)", 8);
    write_colored(" ", 0);
    write_colored("down arrow(scroll down)", 8);
    write_colored(" ", 0);
    write_colored("left arrow(select left)", 8);
    write_colored(" ", 0);
    write_colored("right arrow(select right)", 8);
    attrset(COLOR_PAIR(0));
    move(0, 0);
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

int get_key_press(){
    int x_pos;
    int y_pos;
    getyx(stdscr, y_pos, x_pos);
    chtype old_c = inch();
    int c = getch();
    move(y_pos, x_pos);
    addch(old_c);
    move(y_pos, x_pos);
    return c;
}

void scroll_up(int y_pos, int x_pos, int64_t *fist_displayed_line, program_t *program){
    //Scroll the view up if necessary
    if(y_pos <= 0 && *fist_displayed_line - 1 >= 0){
        clear();
        *fist_displayed_line = *fist_displayed_line - 1;
        write_program(program, (uint32_t)*fist_displayed_line);
        write_keymap_line();
        move(0, 0);
    }else
        move(y_pos -1, x_pos);
}

void scroll_down(int y_pos, int x_pos, int64_t *scroll_pos, program_t *program){
    //Scroll the view down if necessary
    if(y_pos >= LINES - 2){
        clear();
        *scroll_pos = *scroll_pos + 1;
        write_program(program, (uint32_t)(*scroll_pos));
        write_keymap_line();
        move(LINES - 2, x_pos);
    }else
        move(y_pos + 1, x_pos);
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