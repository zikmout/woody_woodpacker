# Woody Woodpacker

## Description:
This project creates a program that encrypts 64-bit ELF binaries. When the encrypted program is executed, it decrypts itself in memory and behaves exactly like the original binary.

## Files:
- `woody_woodpacker.c`: Main C program that handles ELF manipulation and encryption.
- `assembly.S`: Assembly code to handle decryption at runtime.
- `elf_utils.c`: Utility functions to parse and manage ELF files.
- `Makefile`: For building the project.

## How to Compile:
Run the following command to compile the project: