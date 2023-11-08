#pragma once

#include <stdbool.h>

// Prints the msg given as parameter then shows stderror content and then terminates the program with code -1
void throw_error(const char* msg);
bool are_args_valid(int argc, char** argv);