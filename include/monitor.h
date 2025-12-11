#pragma once

#include <idt.h>

// Activate the kernel monitor,
// optionally providing a trap frame indicating the current state
// (NULL if none).
void monitor(registers_t* tf);

// Functions implementing monitor commands.
int mon_help(int argc, char** argv, registers_t* tf);
int mon_kerninfo(int argc, char** argv, registers_t* tf);
int mon_cls(int argc, char** argv, registers_t* tf);
