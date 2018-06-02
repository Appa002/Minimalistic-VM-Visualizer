//
// Created by a_mod on 29.05.2018.
//

#ifndef VM_VISUALIZER_WIDGETS_H
#define VM_VISUALIZER_WIDGETS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <ncurses/curses.h>
#include <locale.h>

#include "program.h"
#include "utils.h"

void clear_line(int y_pos){
    move(y_pos, 0);
    for(size_t i = 0; i < COLS; i++){
        write_colored(" ", 0);
    }
}

void write_keymap_line() {
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

void write_representation_line(uint32_t line_index, program_t* program, uint32_t element){
    line_t* line = program->lines[line_index];
    clear_line(LINES - 2);
    move(LINES - 2, 0);

    if(line_index >= program->line_amount)
        return;

    if(element - 2 > line->instruction_args_amount)
        element = 0;
    else
        element -= 2;

    if(line->instruction_args_amount == 0) {
        char instruction[256];
        sprintf(instruction, "Instruction: %s", line->instruction_name);
        write_colored(instruction, 9);
        write_colored(" ", 0);
        write_colored("No Args", 9);
        return;
    }

    char hex[256];
    char c[256];
    char dec[256];
    char arg[256];
    char instruction[256];

    sprintf(hex, "Hex: %#04x", line->instruction_args[element]);
    sprintf(c, "Char: %c", line->instruction_args[element]);
    sprintf(dec, "Dec: %u", line->instruction_args[element]);
    sprintf(arg, "Displaying for Arg: %u", element);
    sprintf(instruction, "Instruction: %s", line->instruction_name);

    write_colored(instruction, 9);
    write_colored(" ", 0);
    write_colored(arg, 9);
    write_colored(" ", 0);
    write_colored(hex, 9);
    write_colored(" ", 0);
    write_colored(c, 9);
    write_colored(" ", 0);
    write_colored(dec, 9);
    write_colored(" ", 0);

    attrset(COLOR_PAIR(0));
    refresh();
}

void write_line_at(uint32_t line_index, int y_pos, program_t *program) {
    if (line_index >= program->line_amount)
        return;

    move(y_pos, 0);

    char line_num_as_hex[32];
    sprintf(line_num_as_hex, "%#010x", program->lines[line_index]->line_num);

    write_colored(line_num_as_hex, 0);
    addstr(" ");
    write_colored(program->lines[line_index]->instruction_name, 0);
    addstr(" ");

    for (size_t j = 0; j < program->lines[line_index]->instruction_args_amount; j++) {
        char arg_as_hex[8];
        sprintf(arg_as_hex, "%#04x", program->lines[line_index]->instruction_args[j]);

        write_colored(arg_as_hex, 0);
        addstr(" ");
    }

    move(y_pos, 0);
}

void write_program(program_t *program, uint32_t start_line) {
    for (uint32_t i = start_line; i < program->line_amount && (i - start_line) < LINES - 2; i++) {
        write_line_at(i, (int)(i-start_line), program);
    }
    refresh();
}

int get_key_press() {
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

bool save_scroll_to(uint32_t line_index, program_t *program) {
    if (line_index >= program->line_amount)
        return false;
    move(0, 0);
    clear();
    write_program(program, line_index);
    write_representation_line(line_index, program, 0);
    write_keymap_line();
    return true;
}

void move_cursor_up(uint32_t *line_index, program_t *program, int *cur_y_pos, const int *cur_x_pos) {
    if (*cur_y_pos - 1 < 0) {
        *cur_y_pos = 0;
        if (save_scroll_to(*line_index - 1, program))
            (*line_index)--;
    } else{
        move(*cur_y_pos - 1, *cur_x_pos);
        *cur_y_pos -= 1;
    }
}

void move_cursor_down(uint32_t *line_index, program_t *program, int* cur_y_pos, const int* cur_x_pos) {
    if (*cur_y_pos + 1 >= LINES - 2) {
        if (save_scroll_to(*line_index + 1, program))
            (*line_index)++;
        *cur_y_pos = LINES - 3;
        move(*cur_y_pos, *cur_x_pos);

    } else if(*cur_y_pos + 1){
        *cur_y_pos += 1;
        move(*cur_y_pos, *cur_x_pos);
    }
}

void mark_line_part(program_t *program, int y_pos, uint32_t line_index, int32_t *element) {
    write_line_at(line_index - 1, y_pos - 1, program);
    if(y_pos + 1 < LINES - 2)
        write_line_at(line_index + 1, y_pos + 1, program);

    if (*element < 0)
        *element = program->lines[line_index]->instruction_args_amount + 1;

    if (*element >= program->lines[line_index]->instruction_args_amount + 2)
        *element = 0;

    move(y_pos, 0);

    char line_num_as_hex[32];
    sprintf(line_num_as_hex, "%#010x", program->lines[line_index]->line_num);

    if_write_colored(*element == 0, line_num_as_hex, 6);
    addstr(" ");
    if_write_colored(*element == 1, program->lines[line_index]->instruction_name, 6);
    addstr(" ");

    for (size_t j = 0; j < program->lines[line_index]->instruction_args_amount; j++) {
        char arg_as_hex[8];
        sprintf(arg_as_hex, "%#04x", program->lines[line_index]->instruction_args[j]);

        if_write_colored(*element == 2 + j, arg_as_hex, 6);
        addstr(" ");
    }

    move(y_pos, 0);
}


#endif