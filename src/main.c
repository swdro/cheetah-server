#include "cfuhash.h"
#include "http_parser.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

int main() {

  printf("Cheetah initialization \n");

  http_parser *parser = init_http_parser(HTTP_REQUEST);

  char *data = "Hello world!!";
  size_t data_len = strlen(data);

  http_parser_execute(data, data_len, parser);

  return 0;
}
