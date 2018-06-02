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

void clear_line(int y_pos){
    move(y_pos, 0);
    for(size_t i = 0; i < COLS; i++){
        write_colored(" ", 0);
    }
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

void write_repeated_char(char c, int amount, int color){
    for(int i = 0; i < amount; i++){
        char str[2];
        str[0] = c;
        str[1] = 0;
        write_colored(str, color);
    }
}

void write_center_box(char* upper_str, char* lower_str, int upper_color, int lower_color, int margin){
    char* larger_string = (strlen(upper_str) > strlen(lower_str)) ? (upper_str) : (lower_str);
    int larger_string_size = (int)strlen(larger_string);
    int upper_size = (int)strlen(upper_str);
    int lower_size = (int)strlen(lower_str);
    int length = margin * 2 + larger_string_size;
    int x_corner = COLS/2 - (length/2);
    int y_corner = LINES/2 - 2;


    move(y_corner, x_corner);
    write_colored("+", 0);
    write_repeated_char('-', length, 0);
    write_colored("+", 0);

    for(int i = 0; i < margin; i++){
        move(y_corner + i + 1, x_corner);
        write_colored("|", 0);
        write_repeated_char(' ', length, 0);
        write_colored("|", 0);
    }

    move(y_corner + 1 + margin, x_corner);
    write_colored("|", 0);
    write_repeated_char(' ', (length - upper_size) / 2, 0);
    write_colored(upper_str, upper_color);
    write_repeated_char(' ', (length - upper_size) / 2, 0);
    if((length - upper_size) % 2 != 0)
        write_colored(" ", 0);
    write_colored("|", 0);

    move(y_corner + 2 + margin, x_corner);
    write_colored("|", 0);
    write_repeated_char(' ', (length - lower_size) / 2, 0);
    write_colored(lower_str, lower_color);
    write_repeated_char(' ', (length - lower_size) / 2, 0);
    write_colored("|", 0);

    for(int i = 0; i < margin; i++){
        move(y_corner + i + 3 + margin, x_corner);
        write_colored("|", 0);
        write_repeated_char(' ', length, 0);
        write_colored("|", 0);
    }

    move(y_corner + 3 + 2 * margin, x_corner);
    write_colored("+", 0);
    write_repeated_char('-', length, 0);
    write_colored("+", 0);
}


#endif //VM_VISUALIZER_UTILS_H
