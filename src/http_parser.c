#include "http_parser.h"
#include "cfuhash.h"
#include "util.h"
#include <ctype.h>
#include <linux/limits.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>

// hettp methods 
char *http_methods[] = { 
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH"
};

// define state functions
STATE_RETURN_CODE start_state(char *curr_str, char c, cfuhash_table_t *http_message) {
    return SUCCESS;
}

STATE_RETURN_CODE method_state(char *method_str, char c, cfuhash_table_t *http_message) {
    printf("method_state: curr_str: %s\n", method_str);
    if (c == ' ') { 
        int method_str_no_space_len= strlen(method_str);
        char *method_str_no_space = calloc(method_str_no_space_len, sizeof(char));
        strcpy(method_str_no_space, method_str);
        method_str_no_space[method_str_no_space_len - 1] = '\0';
        printf("METHOD STRING NO SPACE: %s\n", method_str_no_space);
        if (contains_string(http_methods, method_str_no_space) == 0) { // check if method is in valid http_methods array
            printf("finished finding string.. \n");
            // append to hashmap
            cfuhash_put(http_message, "method", method_str_no_space);
            // free(method_str_no_space);
            printf("hashmap: \n");
            cfuhash_pretty_print(http_message, stdout);
            return SUCCESS;
        }
        free(method_str_no_space);
        fprintf(stderr, "HTTP method not recognized\n");
        return FAIL;
    } else if (!isupper(c)) {
        fprintf(stderr, "Incorrect character in HTTP Request Method\n");
        return FAIL;
    } else if (strlen(method_str) == 7) { // longest method is 7 chars long
        fprintf(stderr, "HTTP Request Method is too long to be valid\n");
        return FAIL;
    } else {
        // copy old string into new string to append character
        // char *old_str = curr_str;
        // size_t old_str_length = strlen(old_str);
        // char new_str[old_str_length + 2];  // length will include new char and null terminator
        //
        // strcpy(new_str, old_str);
        // new_str[old_str_length] = c;
        // new_str[old_str_length + 1] = '\0';
        // printf("new_str: %s\n", new_str);
        // curr_str = new_str;
        return REPEAT;
    }
}

STATE_RETURN_CODE request_target_state(char *curr_str, char c, cfuhash_table_t *http_message) {
    /* We will return to this function and parse it into urls sub fields
    if (c == '/') {
    } else if (isalpha(c)) {
    } 
    */
    if (strlen(curr_str) > 100) {
        fprintf(stderr, "Request target strings is above 100 characters\n"); // we can change this later
        return FAIL;
    }
    if (c == ' ') {
        char *url_str_no_space = removeSpaces(curr_str);
        printf("URL STRING NO SPACE: %s\n", url_str_no_space);
        cfuhash_put(http_message, "requestTargetUrl", url_str_no_space); // append to hashmap
        cfuhash_pretty_print(http_message, stdout);
        return SUCCESS;
    }
    return REPEAT;
}


STATE_RETURN_CODE http_name(char *curr_str, char c, cfuhash_table_t *http_message) {
    if (c == '/') {
        size_t curr_str_len = strlen(curr_str);
        char *http_name_no_forward_slash = calloc(curr_str_len, sizeof(char));
        strcpy(http_name_no_forward_slash, curr_str);
        http_name_no_forward_slash[curr_str_len - 1] = '\0';
        printf("HTTP NAME STRING NO FORWARD SLASH: %s\n", http_name_no_forward_slash);
        if (strcmp(http_name_no_forward_slash, "HTTP") == 0) {
            return SUCCESS;
        }
    } else if (isupper(c) && strlen(curr_str) < 5) { // haven't reached full string "HTTP"
        return REPEAT;
    } 
    return FAIL;
}

STATE_RETURN_CODE http_version(char *curr_str, char c, cfuhash_table_t *http_message) {
    return SUCCESS;
}

STATE_RETURN_CODE new_line_before_headers(char *curr_str, char c, cfuhash_table_t *http_message) {
    return SUCCESS;
}

STATE_RETURN_CODE end_state(char *curr_str, char c, cfuhash_table_t *http_message) {
    return SUCCESS;
}

// states
typedef enum {
    // REQUEST
    START,
    METHOD,
    REQUEST_TARGET,
    HTTP_NAME,
    HTTP_VERSION,
    HTTP_HEADER,
    NEW_LINE_BEOFRE_HEADERS,
    END
} HTTP_PARSER_STATE;

// state functions 
typedef STATE_RETURN_CODE (*HttpParserStateFunction)(char *curr_str, char c, cfuhash_table_t *http_message);
HttpParserStateFunction http_parser_state_functions[] = {
    start_state, 
    method_state, 
    request_target_state, 
    http_name,
    http_version,
    new_line_before_headers,
    end_state
};

// transition declaration and array
typedef struct {
    HTTP_PARSER_STATE source_state;
    STATE_RETURN_CODE return_code;
    HTTP_PARSER_STATE destination_state;
} Transition;

Transition transition_states[][3] = {
{
    {START, SUCCESS, METHOD}, 
    {START, REPEAT, START},
        
    },
{
    {METHOD, SUCCESS, REQUEST_TARGET},
    {METHOD, REPEAT, METHOD},
    },
{
    {REQUEST_TARGET, SUCCESS, HTTP_NAME},
    {REQUEST_TARGET, REPEAT, REQUEST_TARGET},
    },
{
    {HTTP_NAME, SUCCESS, HTTP_VERSION},
    {HTTP_NAME, REPEAT, HTTP_NAME},
    },
    {
    {HTTP_VERSION, SUCCESS, NEW_LINE_BEOFRE_HEADERS},
    {HTTP_VERSION, REPEAT, HTTP_VERSION},
    },
};

// Transition **transition_states = {
// {
//         {START, SUCCESS, METHOD}, 
//         {START, REPEAT, START},
//         {START, FAIL, END}
// },
// {
//         {METHOD, SUCCESS, REQUEST_TARGET},
//         {METHOD, REPEAT, METHOD}
// },
// {
//         {REQUEST_TARGET, SUCCESS, END},
//         {REQUEST_TARGET, REPEAT, REQUEST_TARGET},
// }};

/*
initialize http_parser
*/
http_parser *init_http_parser(http_message_type type, cfuhash_table_t *http_hashmap) {
    http_parser *parser = (http_parser *) malloc(sizeof(http_parser));
    parser->type = type;
    parser->http_message = http_hashmap;
    return parser;
}

/*
free http parser
*/
void free_http_parser(http_parser* parser) {
    cfuhash_table_t *http_hashmap = parser->http_message;
    size_t num_keys = 0;
    char **keys_array_ptr = (char **) cfuhash_keys_data(http_hashmap, &num_keys, NULL, 0);
    for (size_t i = 0; i < num_keys; i++) {
        char *key = keys_array_ptr[i];
        char *value= (char *) cfuhash_get(http_hashmap, key);
        printf("free this: %s\n", value);
        free(key);
        free(value);
    }
    free(keys_array_ptr);
    cfuhash_destroy(parser->http_message);
    // void **cfuhash_keys_data(cfuhash_table_t *ht, size_t *num_keys, size_t **key_sizes,
    // 	int fast);
    free(parser);
}

/*
parse the http data. Return 0 on success, > 0 on error
*/
int http_parser_execute(char *data, size_t data_len, http_parser *parser) {
    char *http_message = data;

    // http message may start with \n
    if (*http_message == '\n') {
        http_message++;
    }

    // initialize http parser state based on parser type
    HTTP_PARSER_STATE curr_state;
    if (parser->type == HTTP_REQUEST) {
        curr_state = METHOD;
        printf("REQUEST parser. Begin parsing in METHOD state\n");
    } else if (parser->type == HTTP_RESPONSE) {
        curr_state = END; // response unimplemented
        printf("RESPONSE parser. Begin parsing in END state\n");
    } else {
        printf("invalid parsert type\n");
        fprintf(stderr, "invalid parser type. Must be REQUEST or RESPONSE");
        return 1;
    }

    // char curr_str[] = {'\0'};
    // insitialize empty string
    char *curr_str = calloc(1, sizeof(char));
    curr_str[0] = '\0';
    // initialize state to beginning
    for (http_message = data; http_message < data + data_len; http_message++) {
        // do something based on current state
        printf("char: %c\n", *http_message);
        char curr_char = *http_message;

        // append char to end of curr_str

        // create new str with strlen(old str) + 2 to accommodate for new char + null terminator
        // strcpy old str into new str
        // pass new str to function

        printf("old curr_str: %s\n", curr_str);
        char *new_str = realloc(curr_str, sizeof(char) * (strlen(curr_str) + 2));
        if (new_str == NULL) {
            free(curr_str);
            printf("could not realloc\n");
            return 1;
        } else {
            new_str[strlen(curr_str)] = curr_char;
            new_str[strlen(curr_str) + 1] = '\0';
            curr_str = new_str;
        }
        printf("new curr_str: %s\n", curr_str);
        HttpParserStateFunction state_function = http_parser_state_functions[curr_state];
        STATE_RETURN_CODE rc = state_function(curr_str, curr_char, parser->http_message);
        // go to next transition state
        for (int i = 0; i < 2; i++) { // change 2 to whatever the size of inner most keys_array_ptr
            Transition temp_state = transition_states[curr_state][i];
            if (temp_state.return_code == rc) {
                printf("%d -> %d\n", curr_state, temp_state.destination_state);
                curr_state = temp_state.destination_state;
                if (rc == SUCCESS) {
                    free(curr_str);
                    curr_str = calloc(1, sizeof(char));
                    curr_str[0] = '\0';
                }
                break;
            }
        }
        if (rc == FAIL) {
            printf("Failed, returning.. \n");
            return 1; // associated function printed to stderr already
        }
    }
    free(curr_str);
    return 0;
}
