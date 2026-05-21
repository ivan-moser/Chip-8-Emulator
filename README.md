# CHIP-8 Emulator

A modern CHIP-8 emulator written in C using SDL2, developed as a low-level programming and computer architecture study project.

![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)
![Graphics](https://img.shields.io/badge/graphics-SDL2-green)

---

# Preview

<p align="center">
  <img src="assets/preview.png" width="600">
</p>

---

# Features

- Complete CHIP-8 CPU implementation
- SDL2 graphics rendering
- Keyboard input handling
- Timers support (`delay_timer` and `sound_timer`)
- Sprite rendering with collision detection
- ROM loading support
- Accurate opcode execution
- Passing opcode test suites

---

# Implemented Opcodes

## Flow Control
- `00E0` - Clear screen
- `00EE` - Return from subroutine
- `1NNN` - Jump
- `2NNN` - Call subroutine
- `BNNN` - Jump with offset

## Conditional Instructions
- `3XNN`
- `4XNN`
- `5XY0`
- `9XY0`

## Registers & Arithmetic
- `6XNN`
- `7XNN`
- `8XY0`
- `8XY1`
- `8XY2`
- `8XY3`
- `8XY4`
- `8XY5`
- `8XY6`
- `8XY7`
- `8XYE`

## Memory
- `ANNN`
- `FX1E`
- `FX55`
- `FX65`

## Timers
- `FX07`
- `FX15`
- `FX18`

## Input
- `EX9E`
- `EXA1`
- `FX0A`

## Graphics
- `DXYN`

## Misc
- `CXNN`
- `FX29`
- `FX33`

---

# CHIP-8 Keypad Mapping

| CHIP-8 | Keyboard |
|--------|----------|
| 1 2 3 C | 1 2 3 4 |
| 4 5 6 D | Q W E R |
| 7 8 9 E | A S D F |
| A 0 B F | Z X C V |

---
