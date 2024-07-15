#ifndef H_HTTP_PARSER
#define H_HTTP_PARSER

#include "cfuhash.h"
#include <stdio.h>

typedef enum { HTTP_REQUEST = 0, HTTP_RESPONSE = 1 } http_message_type;

// may not need this
// typedef enum { REQUEST_LINE } State;

typedef struct {
    http_message_type type;
    cfuhash_table_t *http_message;
} http_parser;

http_parser *init_http_parser(http_message_type type, cfuhash_table_t *http_message);

int http_parser_execute(char *data, size_t data_len, http_parser *parser);

void free_http_parser(http_parser* parser);

/* state machine code */
/*typedef enum {*/
/*  // REQUEST*/
/*  START,*/
/*  METHOD,*/
/*  REQUEST_TARGET,*/
/*  END,*/
/*} HTTP_PARSER_STATE;*/

typedef enum { SUCCESS, REPEAT, FAIL } STATE_RETURN_CODE;

/*extern STATE_RETURN_CODE (*http_parser_state_functions[])(*/
/*    char *cur_str, char *c, cfuhash_table_t *http_message);*/

/*typedef struct {*/
/*  HTTP_PARSER_STATE source_state;*/
/*  STATE_RETURN_CODE return_code;*/
/*  HTTP_PARSER_STATE destination_state;*/
/*} Transition;*/

STATE_RETURN_CODE start(char *cur_str, char *c, cfuhash_table_t *http_message);
STATE_RETURN_CODE method(char *cur_str, char *c, cfuhash_table_t *http_message);
STATE_RETURN_CODE request_target(char *cur_str, char *c,
                                 cfuhash_table_t *http_message);
STATE_RETURN_CODE end(char *cur_str, char *c, cfuhash_table_t *http_message);


/* HTTP semantics */
extern char *http_methods[];


#endif


