
#include <bstrlib.h>
#include <constable.h>
#include <conshell.h>

cons_line cli;
cmdlist functions;

void setup()
{
    Serial.begin(9600);

    cons_cmdlist_init( &functions );
    cons_init_line( &cli, &Serial );

    cons_reg_cmd( &functions, "test", (void*) ctest );

    cli.port->print(">");
}

void loop()
{
    static int res = 0;

    res = cons_poll_line( &cli, CONSHELL_CLI_TIMEOUT );
    if( res == CONSHELL_LINE_END ) {
        res = cons_search_exec( &cli, &functions );
        cons_clear_line( &cli );
        cli.port->print("> ");
    }

}

int ctest( blist list )
{
    cli.port->print("Called ctest, has arguments: ");
    cli.port->println( list->qty );

    if( list == NULL || list->qty <= 1 ) {
        cli.port->println("Too few args");
        return -1;
    }

    cli.port->print("Argument is: ");
    cli.port->println( (char*) list->entry[1]->data );

    return 0;
}

