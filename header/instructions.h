#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H


#define ADD_INSTRUCTION(name, arg_amount) uint8_t* opt_ ## name (uint8_t* ip, line_t* out) { return general_instruction((ip), (out), #name, arg_amount); }


#include <stdint.h>
#include <memory.h>

#include "program.h"
#include "utils.h"

typedef uint8_t *(*instruction_t)(uint8_t *ip, line_t *out);

uint8_t *add_to_ip(uint8_t *ip, uint32_t amount) {
    while (amount > 0) {
        ip++;
        if (*ip == ';') {
            ip++;
            while (*ip != ';')
                ip++;
            ip++;
        }
        amount--;
    }
    return ip;
}

uint8_t *general_instruction(uint8_t *ip, line_t *out, const char* name, uint32_t arg_amount) {
    out->instruction_name = create_string(name);
    out->instruction_args_amount = arg_amount;
    out->instruction_args = malloc(sizeof(uint8_t) * arg_amount);
    for (uint32_t i = 0; i < arg_amount; i++){
        out->instruction_args[i] = *add_to_ip(ip, i + 1);
    }
    return add_to_ip(ip, 1 + arg_amount);
}

ADD_INSTRUCTION(nop, 0)
ADD_INSTRUCTION(push8, 1)
ADD_INSTRUCTION(push16, 2)
ADD_INSTRUCTION(push32, 4)
ADD_INSTRUCTION(emit, 0)
ADD_INSTRUCTION(emit_char, 0);
ADD_INSTRUCTION(jump, 0);
ADD_INSTRUCTION(compare, 0);
ADD_INSTRUCTION(jump_n_equal, 0);
ADD_INSTRUCTION(jump_equal, 0);
ADD_INSTRUCTION(jump_less, 0);
ADD_INSTRUCTION(jump_greater, 0);
ADD_INSTRUCTION(sum, 0);
ADD_INSTRUCTION(sub, 0);
ADD_INSTRUCTION(multiply, 0);
ADD_INSTRUCTION(divide, 0);
ADD_INSTRUCTION(call, 0);
ADD_INSTRUCTION(return, 0);
ADD_INSTRUCTION(read, 0);
ADD_INSTRUCTION(write, 0);
ADD_INSTRUCTION(pop, 0);
ADD_INSTRUCTION(halt, 0);
ADD_INSTRUCTION(emit_string, 1);

uint8_t *opt_push_string(uint8_t *ip, line_t *out) {
    uint8_t length_of_string = *add_to_ip(ip, 1);

    out->instruction_args_amount = 1 + length_of_string;
    out->instruction_name = create_string("push_string");
    out->instruction_args = malloc(sizeof(uint8_t) * length_of_string);
    for(uint8_t i = 0; i < length_of_string; i++){
        out->instruction_args[i] = *add_to_ip(ip, 2 + i);
    }
    return add_to_ip(ip, 2 + length_of_string);
}

void register_instructions(instruction_t *opt) {
    for (size_t i = 0; i < 256; i++) {
        opt[i] = opt_nop;
    }

    opt['.'] = opt_nop; // No operation. I move nothing and nothing moves me.

    ////// Opt codes pushing raw data onto the stack or prints raw data//////
    opt['b'] = opt_push8; //Pushes its argument as unsigned byte to the stack.
    opt['q'] = opt_push16; // Pushes its argument as 16 bit unsigned num on to the stack.
    opt['l'] = opt_push32; // Pushes its argument as 32 bit unsigned num on to the stack.
    opt['s'] = opt_push_string; // Pushes the amount of bytes specified by its first arguments onto the stack (The bytes following the first argument are pushed).
    opt['e'] = opt_emit; // Pops one byte from the stack an writes it to standard out.
    opt['i'] = opt_emit_char; // Pops a element from the stack and prints es contents to standard as a ascii character.
    opt['p'] = opt_emit_string; // Pops the amount of bytes specified by its argument of the stack and writes them to standard out.
    opt['$'] = opt_read;
    opt['x'] = opt_pop;
    opt['#'] = opt_write;

    ////// Opt codes which control the flow //////
    opt['j'] = opt_jump; // Jumps to the address specified on the stack. Expects 32bit num
    opt['<'] = opt_jump_less; // Executes a jump if the last compare determent that the first element is smaller then the second element of the stack. Expects 32bit num as first element on stack.
    opt['>'] = opt_jump_greater; //  Executes a jump if the last compare determent that the first element is grater then the second element of the stack. Expects 32bit num as first element on stack.
    opt['='] = opt_jump_equal; // Executes a jump if the last compare determent that the first and second element of the stack are equal. Expects 32bit num as first element on stack.
    opt['!'] = opt_jump_n_equal; // Executes a jump if the last compare determent that the first and second element of the stack are not equal. Expects 32bit num as first element on stack.
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
