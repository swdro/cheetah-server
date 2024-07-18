#include "util.h"
#include <string.h>
#include <stdlib.h>

int contains_string(char * const * const string_array, const char *string) {
    // int len = sizeof(string_array) / sizeof(string_array[0]);
    int len = 9;
    for (int i = 0; i < len; i++) {
        if (strcmp(string_array[i], string) == 0) {
            return  0;
        }
    }
    return  1;
}

char *removeSpaces(char *input) {
    size_t input_len = strlen(input);
    char *output = calloc(input_len + 1, sizeof(char)); // account for null terminator
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] != ' ') {
            output[j++] = input[i];
        }
    }
    return output;
}

char *duplicateString(char *str) {
    size_t curr_str_len = strlen(str);
    char *new_str = calloc(curr_str_len + 1, sizeof(char)); // account for null terminator
    strcpy(new_str, str);
    str[curr_str_len] = '\0';
    return new_str;
}
