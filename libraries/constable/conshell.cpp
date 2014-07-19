#include "conshell.h"

conshell_status
cons_init_line( cons_line* nline, Stream* port )
{
    nline->delim = bfromcstr( " :," );
    nline->line = bfromcstralloc ( CONSHELL_CLI_BUFFER_LEN, "" );
    nline->port = port;
}

/** Grab a line from the serial line within a time limit
 *
 * Place new characters as they are found into the cons_line structure passed
 * to the function. If the time limit expires, return the timeout type return
 * code.
 */
conshell_status
cons_poll_line( cons_line* cline, uint32_t timeout )
{
    static char c;
    c = '\0';
    uint32_t time;
    bstring buffer = cline->line;

    //if( buf->len >= ASHCON_CLI_MAX_LENGTH ) return -1;

    if( cline->port->available() == 0 ) {
        return CONSHELL_LINE_TIMEOUT;
    }

    time = millis();
    timeout += time;

//#ifdef DEBUG
    //Serial.print("Current time: ");
    //Serial.println( time );
    //Serial.print("Stop time: ");
    //Serial.println( timeout );
//#endif

    while( time < timeout ) {
        if( cline->port->available() != 0 ) {
            c = cline->port->read();
            bconchar( buffer, c );
//#ifdef DEBUG
            //Serial.print("Got char ");
            //Serial.println( c, HEX );
//#endif
        }

        if( c == CONSHELL_CLI_EOL ) {
            return CONSHELL_LINE_END;
        }

        time = millis();
    }
    
    return CONSHELL_LINE_TIMEOUT;
}

int
cons_search_exec( cons_line* cline, cmdlist* cmds )
{
    int returncode = 0;
    conshfunc func;
    blist args;

    args = bsplits( cline->line, cline->delim );

#ifdef DEBUG
    char* buffer;

    Serial.print("Got command name ");

    buffer = bstr2cstr( args->entry[0], '8' );
    Serial.println( buffer );
    free(buffer);

    //Serial.print("And second entry ");

    //buffer = bstr2cstr( args->entry[1], '8' );
    //Serial.println( buffer );
    //free(buffer);

    Serial.println("Values within list as such...");
    Serial.print("Addr: 0x");
    Serial.println( (uint16_t) args, HEX );
    Serial.print("qty: ");
    Serial.println( args->qty );
    Serial.print("mlen: ");
    Serial.println( args->mlen );

    if( args->entry[0] ) {
        Serial.print("0: ");
        Serial.println( (uint16_t) args->entry[0], HEX );
    }
    if( args->entry[1] ) {
        Serial.print("1: ");
        Serial.println( (uint16_t) args->entry[1], HEX );
    }
    if( args->entry[2] ) {
        Serial.print("2: ");
        Serial.println( (uint16_t) args->entry[2], HEX );
    }
#endif

    // Check if its added a newline for some reason
    if( args->qty == 1 ) {
        uint8_t cpos = bstrchr( args->entry[0], '\n' );
        if( cpos != -1 ) bdelete( args->entry[0], cpos, 1 );
    }

#ifdef DEBUG
    int charpos = bstrchr( args->entry[0], '\n' );
    if( charpos ) {
        Serial.print("Newline found at ");
        Serial.println( charpos );
    }
#endif

    func = (conshfunc) cons_exec_cmd( cmds, args->entry[0] );

    if( func != NULL ) 
        returncode = func( args );
    else 
        returncode = CONSHELL_FUNCTION_NOT_FOUND;

    bstrListDestroy( args );

    return returncode;
}

/** Reset the string to contain no characters
 *
 * Dosn't delete the memory associated with the string in any way
 */
conshell_status
cons_clear_line( cons_line* cline )
{
    bassigncstr( cline->line, "" );
}
