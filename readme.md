
# How to use constable command line functions

Constable is a really simple shell that lets you type in the name of a
function at the serial line, and have it execute immediately. Making a new
function is simple and acessing arguments given to the command is also made
possible.

First you need to intialise two structures, one for storing functions and
their names, and the other for an instance of a serial terminal.

    cons_line cli;
    cmdlist functions;

Before they can be used, some default values need to be set, so you call an
initialiser function for each one.

    cons_cmdlist_init( &functions );
    cons_init_line( &cli, &Serial );

Note that the initialiser functions need the memory address of the structures,
so you need to include the `&`. Now you add the functions you want to be able
to access using the registration command.

    cons_reg_cmd( &functions, "test", (void*) basic );
    cons_reg_cmd( &functions, "ctest", (void*) ctest );

Now you just need to get a line of input from the user, and check to see if
they have pressed the enter key. To do that, you run the following function
your polling loop.

    res = cons_poll_line( &cli, CONSHELL_CLI_TIMEOUT );

The function checks for new input on the serial line within the allowed time
period. Its not allowed to remain for any longer than the timeout (in
milliseconds). The `int` "`res`", will have the value `CONSHELL_LINE_END` when
the user hase pressed the enter key. So you will want to have a condition in
your polling loop to check for this, then attempt to search for a registered
function using the contents of the command line buffer.

    if( res == CONSHELL_LINE_END ) {
        res = cons_search_exec( &cli, &functions );
        cons_clear_line( &cli );
    }

This will attempt to execute the function if it finds it registered under the
name entered by the user. The function is passed a list of bstrings (blist),
which is from the bstrlib c string library, see its documentation for details.

An example command line function taking advantage of the argument passing
would be:


    int ctest( blist list )
    {
        char* buffer;

        Serial.print("Called ctest, has arguments: ");
        Serial.println( list->qty );
        
        if( list == NULL || list->qty <= 1 ) {
            Serial.println("Too few args");
            return -1;
        }

        buffer = bstr2cstr( list->entry[1], '\0' );
        Serial.print(buffer);

        free(buffer);

        return 0;
    }

Note that any functions you define should have the same type of arguments and
return, i.e. returns an integer, takes blist as an argument.

