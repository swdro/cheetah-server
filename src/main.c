#include "cfuhash.h"
#include "http_parser.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int test(char *http_message) {
    cfuhash_table_t *http_hashmap = cfuhash_new();
    http_parser *parser = init_http_parser(HTTP_REQUEST,  http_hashmap);
    int testHttpParser = http_parser_execute(http_message, strlen(http_message), parser);
    free_http_parser(parser);
    return testHttpParser;
}

int main() {
    printf("Cheetah initialization \n");

    // test cases
    assert(test("HEAD ") == 0);
    assert(test("GET /helloworld ") == 0);

    printf("All test cases passed!\n");
    return 0;
}
