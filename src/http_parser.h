#ifndef H_HTTP_PARSER
#define H_HTTP_PARSER

#include "cfuhash.h"
#include <stdio.h>

typedef enum { HTTP_REQUEST = 0, HTTP_RESPONSE = 1 } http_message_type;

typedef enum { REQUEST_LINE } State;

typedef struct {
  http_message_type type;
  cfuhash_table_t *http_message;
} http_parser;

http_parser *init_http_parser(http_message_type type);

int http_parser_execute(char *data, size_t data_len, http_parser *parser);

/* state machine code */

#endif
