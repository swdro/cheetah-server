#include "util.h"
#include <string.h>

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


