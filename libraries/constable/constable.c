#include "constable.h"


/** Initialise a function list
 *
 *  Starts the index at zero and calls the initialisation function for every
 *  statically allocated cmdlet.
 */
constable_status 
cons_cmdlist_init( cmdlist* list )
{
    list->index = 0;
    uint8_t i = 0;
    for( i = 0; i < CONSTABLE_LIST_MAX; i++ ) {
        cons_cmdlet_init( &( list->cmds[i]) );
    }
}

/** Explicitly make sure the function pointer is NULL by default
 */
constable_status
cons_cmdlet_init( cmdlet* entry )
{
    entry->func = NULL;
}

/** register command in function list
 *  
 *  Takes a command list, constant string and comepletely generic function
 *  pointer to generate a cmdlet entry and store it within the list.
 */
constable_status
cons_reg_cmd( cmdlist* list, const char* name, void* func )
{
    if( list->index >= CONSTABLE_LIST_MAX ) {
#ifdef DEBUG
        Serial.println(F("CANNOT REGISTER ANOTHER FUNCTION"));
#endif
        return CONSTABLE_GENERAL_ERROR;
    }

    cmdlet *newcmd = &(list->cmds[list->index]);

    newcmd->name = bfromcstr( name );
    newcmd->func = func;

    list->index++;

    return CONSTABLE_GENERAL_SUCCESS;
}

/** Return the address of a function if it exists
 *  
 *  Searchs the provided list for a function with a matching name. If it finds
 *  a function, it returns the memory address of the function. Returns NULL
 *  otherwise.
 */
void*
cons_exec_cmd( cmdlist* list, const_bstring name )
{
    uint8_t i = 0;
    cmdlet  *command = NULL;

    for( i = 0; i < list->index; i++ ) {
        command = &(list->cmds[i]);
        if( bstrcmp( name, command->name ) == 0 ) {
            return command->func;
        }
    }

    return NULL;
}

/** Return the address of a function given a string constant of its name
 *
 */
void*
cons_exec_strcmd( cmdlist* list, const char* str)
{
    void* result;

    bstring fname = bfromcstr( str );
    result = cons_exec_cmd( list, fname );
    bdestroy(fname);

    return result;
}
