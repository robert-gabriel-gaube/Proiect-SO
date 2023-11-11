#pragma once

#include <stdbool.h>

bool is_same_extension(const char *file_name, const char* expected_extension);
bool are_args_valid(int argc, char** argv);