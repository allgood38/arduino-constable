#ifndef _CONSHELL_H
#define _CONSHELL_H

#include <Arduino.h>
#include <bstrlib.h>
#include <memoryget.h>

// Assume the project has a pins definition
//#include <pins.h>

#include "constable.h"

#define CONSHELL_CLI_TIMEOUT 10
#define CONSHELL_CLI_BUFFER_LEN 64

#define CONSHELL_CLI_EOL '\n'

typedef enum {
    CONSHELL_FUNCTION_BAD_ARGS = -2,
    CONSHELL_FUNCTION_NOT_FOUND = -1,
    CONSHELL_FUNCTION_FOUND = 0,
    CONSHELL_LINE_TIMEOUT,
    CONSHELL_LINE_END,
} conshell_status;

typedef int (*conshfunc)(blist);

typedef struct cons_line {
    bstring delim;
    bstring line;
    Stream* port;
} cons_line;

conshell_status cons_init_line( cons_line*, Stream* );
conshell_status cons_poll_line( cons_line*, uint32_t );

int cons_search_exec( cons_line*, cmdlist* );

conshell_status cons_clear_line( cons_line* );
#endif
