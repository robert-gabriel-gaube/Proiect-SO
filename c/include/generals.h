#pragma once

#include <stdbool.h>

bool is_same_extension(char *file_name, char* expected_extension);
bool are_args_valid(int argc, char** argv);