#pragma once
#include <stdbool.h>

#define ERROR -1

int create_process();
void throw_error(const char *msg);
bool are_args_valid(int argc, char **argv);