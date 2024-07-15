#include "cfuhash.h"
#include "http_parser.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    printf("Cheetah initialization \n");
    // cfuhash_table_t *http_hashmap = cfuhash_new_with_flags(CFUHASH_NO_LOCKING);
    cfuhash_table_t *http_hashmap = cfuhash_new();
    http_parser *parser = init_http_parser(HTTP_REQUEST,  http_hashmap);

    // initialize data variabels
    char *data = "HEAD ";
    size_t data_len = strlen(data);

    // test everything


    int rc = http_parser_execute(data, data_len, parser);
    printf("cheetah finished!\n");

    // free all memory
    char *methodStr = (char *) cfuhash_get(http_hashmap, "method");
    free(methodStr);
    cfuhash_destroy(http_hashmap);
    free_http_parser(parser);

    if (rc != 0) {
        return 1;
    }
    return 0;
}
