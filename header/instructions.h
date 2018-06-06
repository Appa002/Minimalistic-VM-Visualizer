#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H

#include <stdint.h>
#include <memory.h>

#include "program.h"
#include "utils.h"

typedef uint8_t* (*instruction_t)(uint8_t * ip, line_t* out);

uint8_t* add_to_ip(uint8_t* ip, uint32_t amount){
    while (amount > 0){
        ip++;
        if(*ip == ';'){
            ip++;
            while (*ip != ';')
                ip++;
            ip++;
        }
        amount--;
    }
    return ip;
}

uint8_t* opt_nop(uint8_t* ip, line_t* out){
    out->instruction_name = create_string("nop");
    out->instruction_args_amount = 0;
    return add_to_ip(ip, 1);
}

uint8_t* opt_push_8_bit(uint8_t *ip, line_t* out){
    out->instruction_name = create_string("push8");
    out->instruction_args_amount = 1;
    out->instruction_args = malloc(sizeof(uint8_t));
    out->instruction_args[0] = *add_to_ip(ip, 1);
    return add_to_ip(ip, 2);
}

uint8_t* opt_push_16_bit(uint8_t* ip, line_t* out){}

uint8_t* opt_push_32_bit(uint8_t* ip, line_t* out){
    out->instruction_name = create_string("push32");
    out->instruction_args_amount = 4;
    out->instruction_args = malloc(sizeof(uint8_t));
    out->instruction_args[0] = *add_to_ip(ip, 1);
    out->instruction_args[1] = *add_to_ip(ip, 2);
    out->instruction_args[2] = *add_to_ip(ip, 3);
    out->instruction_args[3] = *add_to_ip(ip, 4);
    return add_to_ip(ip, 5);
}

uint8_t* opt_emit(uint8_t* ip, line_t* out){
    out->instruction_name = create_string("nop");
    out->instruction_args_amount = 0;
    return add_to_ip(ip, 1);
}

uint8_t* opt_emit_char(uint8_t* ip, line_t* out){}

uint8_t* opt_jump(uint8_t* ip, line_t* out){}

uint8_t* opt_push_string(uint8_t* ip, line_t* out){}

uint8_t* opt_emit_string(uint8_t* ip, line_t* out){}

uint8_t* opt_compare(uint8_t* ip, line_t* out){}

uint8_t* opt_jump_not_equal(uint8_t* ip, line_t* out){}

uint8_t* opt_jump_equal(uint8_t* ip, line_t* out){}

uint8_t* opt_jump_less(uint8_t* ip, line_t* out){}

uint8_t* opt_jump_grater(uint8_t* ip, line_t* out){}

uint8_t* opt_sum(uint8_t* ip, line_t* out){}

uint8_t* opt_sub(uint8_t* ip, line_t* out){}

uint8_t* opt_multiply(uint8_t* ip, line_t* out){}

uint8_t* opt_divide (uint8_t* ip, line_t* out){}

uint8_t* opt_call(uint8_t* ip, line_t* out){}

uint8_t* opt_return(uint8_t* ip, line_t* out){}

uint8_t* opt_read_from_stack(uint8_t* ip, line_t* out){}

uint8_t* opt_write_to_stack(uint8_t* ip, line_t* out){}

uint8_t* opt_pop_from_stack(uint8_t *ip, line_t* out){}

uint8_t* opt_halt(uint8_t *ip, line_t* out){
    out->instruction_name = create_string("halt");
    out->instruction_args_amount = 0;
    return add_to_ip(ip, 1);
}

void register_instructions(instruction_t* opt){
    for(size_t i = 0; i < 256; i++){
        opt[i] = opt_nop;
    }

    opt['.'] = opt_nop; // No operation. I move nothing and nothing moves me.

    ////// Opt codes pushing raw data onto the stack or prints raw data//////
    opt['b'] = opt_push_8_bit; // Pushes its argument as unsigned byte to the stack.
    opt['q'] = opt_push_16_bit; // Pushes its argument as 16 bit unsigned num on to the stack.
    opt['l'] = opt_push_32_bit; // Pushes its argument as 32 bit unsigned num on to the stack.
    opt['s'] = opt_push_string; // Pushes the amount of bytes specified by its first arguments onto the stack (The bytes following the first argument are pushed).
    opt['e'] = opt_emit; // Pops one byte from the stack an writes it to standard out.
    opt['i'] = opt_emit_char; // Pops a element from the stack and prints es contents to standard as a ascii character.
    opt['p'] = opt_emit_string; // Pops the amount of bytes specified by its argument of the stack and writes them to standard out.
    opt['$'] = opt_read_from_stack;
    opt['x'] = opt_pop_from_stack;
    opt['#'] = opt_write_to_stack;

    ////// Opt codes which control the flow //////
    opt['j'] = opt_jump; // Jumps to the address specified on the stack. Expects 32bit num
    opt['<'] = opt_jump_less; // Executes a jump if the last compare determent that the first element is smaller then the second element of the stack. Expects 32bit num as first element on stack.
    opt['>'] = opt_jump_grater; //  Executes a jump if the last compare determent that the first element is grater then the second element of the stack. Expects 32bit num as first element on stack.
    opt['='] = opt_jump_equal; // Executes a jump if the last compare determent that the first and second element of the stack are equal. Expects 32bit num as first element on stack.
    opt['!'] = opt_jump_not_equal; // Executes a jump if the last compare determent that the first and second element of the stack are not equal. Expects 32bit num as first element on stack.
    opt['g'] = opt_call; // Executes a jump and pushes an pointer with the calls address on to the stack
    opt['r'] = opt_return; // Goes through the stack top-down and jumps to the first address found. (Returns from a 'call' call)
    opt['h'] = opt_halt;

    ////// Logic controlling opt codes //////
    opt['c'] = opt_compare; // Pushes a Flag onto the stack which specifies if the two first objects on the stack are equal or if the first one is smaller/grater then the second.
    opt['+'] = opt_sum; // Sums the next two bytes and pushes the result onto the stack.
    opt['-'] = opt_sub; // Subtracts the next two bytes and pushes the result onto the stack.
    opt['*'] = opt_multiply; // Multiples the next two bytes and pushes the result onto the stack.
    opt['/'] = opt_divide; // Divides the next two bytes and pushes the result onto the stack.
}

#endif //VIRTUAL_MACHIEN_INSTRUCTIONS_H
