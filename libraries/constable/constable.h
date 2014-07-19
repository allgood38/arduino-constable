#ifndef _CONSTABLE_H
#define _CONSTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bstrlib.h>
#include <stdint.h>

#ifndef CONSTABLE_LIST_MAX
#define CONSTABLE_LIST_MAX 15
#endif

typedef struct bstrList* blist;

typedef enum {
    CONSTABLE_GENERAL_SUCCESS,
    CONSTABLE_GENERAL_ERROR = -1
} constable_status ;

typedef struct cmdlet {
    bstring name;

    void* func;
} cmdlet;

typedef struct cmdlist {
    uint8_t index;
    cmdlet cmds[CONSTABLE_LIST_MAX];
} cmdlist;

constable_status cons_cmdlist_init( cmdlist* );
constable_status cons_cmdlet_init( cmdlet* );

constable_status cons_reg_cmd( cmdlist*, const char*, void* );
void* cons_exec_cmd( cmdlist*, const_bstring );
void* cons_exec_strcmd( cmdlist*, const char* );

#ifdef __cplusplus
}
#endif
#endif
