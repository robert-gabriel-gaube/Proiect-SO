#pragma once

#include <stdbool.h>

#ifdef DEBUG
#define PRINT_DEBUG(TYPE, format, args...) printf("[" TYPE "] " format "\n", ##args)
#else
#define PRINT_DEBUG(TYPE, format, args...)
#endif

bool is_same_extension(const char *file_name, const char* expected_extension);
bool are_args_valid(int argc, char** argv);