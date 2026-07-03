#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "chip8.h"
#include "instruction.h"
#include "common.h"

void setup(Chip8 *chip8) {
    RESET_POINTER(chip8);
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

int main(void) {
    printf("========== 執行 CHIP-8 核心單元測試 ==========\n");
    test_jump_instruction();
    // test_call_and_return();
    printf("==============================================\n");
    printf("🎉 所有獨立單元測試全部通過！\n");
    return 0;
}