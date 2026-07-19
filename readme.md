# CHIP-8 Emulator

A lightweight CHIP-8 interpreter written in C++ using the modern **SDL3** framework for pixel-perfect hardware-accelerated rendering and low-latency input handling.

## 🚀 Features

*   **Complete CPU Core:** Accurate implementation of all 35 original CHIP-8 opcodes, including precise timer management and flag behavior.
*   **Modern Audio & Video:** Utilizes the new **SDL3 API** for windowing, custom texture updates, and audio generation.
*   **Configurable Clock Speed:** Smooth execution loops to perfectly recreate the feel of classic 70s and 80s arcade titles.
*   **Cross-Platform Build System:** Fully modular architecture powered by modern **CMake**.

## 🎮 Keypad Mapping

The original CHIP-8 used a hex-based 4x4 grid keypad (`0x0` through `0xF`). This emulator binds them to modern standard QWERTY keys as follows:

```text
Original Hex Layout         Modern Keyboard Layout
+---+---+---+---+           +---+---+---+---+
| 1 | 2 | 3 | C |           | 1 | 2 | 3 | 4 |
+---+---+---+---+           +---+---+---+---+
| 4 | 5 | 6 | D |   --->    | Q | W | E | R |
+---+---+---+---+           +---+---+---+---+
| 7 | 8 | 9 | E |           | A | S | D | F |
+---+---+---+---+           +---+---+---+---+
| A | 0 | B | F |           | Z | X | C | V |
+---+---+---+---+           +---+---+---+---+
```

## References
- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4)
- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)