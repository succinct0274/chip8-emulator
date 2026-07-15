#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "chip8.h"
#include "instruction.h"
#include "common.h"

void setup(Chip8 *chip8) {
    memset(chip8, 0, sizeof(*chip8));
    chip8->pc = 0x200;
    chip8->sp = 0;
}

void test_jump_instruction(void) {
    Chip8 chip8;
    setup(&chip8);

    uint16_t opcode = 0x12F4; // JUMP 0x2F4
    handle_jump(&chip8, opcode);

    assert(chip8.pc == 0x2F4); 
    printf("[PASS] test_jump_instruction\n");

}

void test_call_and_return(void) {
    Chip8 chip8;
    setup(&chip8);

    chip8.memory[0x200] = 0x23;
    chip8.memory[0x201] = 0x00;
    uint16_t call_opcode = (chip8.memory[chip8.pc] << 8) | chip8.memory[chip8.pc + 1];
    instruction_execute(&chip8, call_opcode);

    assert(chip8.pc == 0x300);
    assert(chip8.sp == 1);
    
    assert(chip8.stack[0] == 0x200); 

    uint16_t ret_opcode = 0x00EE;
    chip8.opcode = ret_opcode;
    instruction_execute(&chip8, chip8.opcode);

    assert(chip8.sp == 0);
    assert(chip8.pc == 0x202);
    printf("[PASS] test_call_and_return\n");
}


int main(void) {
    printf("========== Executing CHIP-8 Instruction Unit Test ==========\n");
    test_jump_instruction();
    test_call_and_return();
    printf("==============================================\n");
    printf("🎉 Passed all instruction unit tests \n");
    return 0;
}