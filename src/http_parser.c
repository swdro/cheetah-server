#include "http_parser.h"
#include "cfuhash.h"

/*
initialize http_parser
*/
http_parser *init_http_parser(http_message_type type) {
  cfuhash_table_t *http_hashmap = cfuhash_new();
  http_parser parser = {type, http_hashmap};
  http_parser *parser_ptr = &parser;
  return parser_ptr;
}

/*
parse the http data. Return 0 on success, > 0 on error
*/
int http_parser_execute(char *data, size_t data_len, http_parser *parser) {

  // alert http message type
  if (parser->type == HTTP_REQUEST) {
    printf("created Request parser\n");
  } else if (parser->type == HTTP_RESPONSE) {
    printf("created Response parser\n");
  } else {
    printf("invalid parsert type\n");
  }

  // initialize state to beginning
  char *char_ptr;
  for (char_ptr = data; char_ptr < data + data_len; char_ptr++) {
    // do something based on current state
    printf("char: %c, ", *char_ptr);
  }

  printf("\n");

  return 0;
}
