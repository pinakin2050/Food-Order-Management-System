#ifndef STDIO_H_INCLUDED
    #define STDIO_H_INCLUDED
    #include <stdio.h>
#endif // STDIO_H_INCLUDED

#ifndef STDLIB_H_INCLUDED
    #define STDLIB_H_INCLUDED
    #include <stdlib.h>
#endif // STDLIB_H_INCLUDED

#ifndef STRING_H_INCLUDED
    #define STRING_H_INCLUDED
    #include <string.h>
#endif // STRING_H_INCLUDED

#ifndef COMMONFUNCTIONS_H_INCLUDED
#define COMMONFUNCTIONS_H_INCLUDED

///Clears the screen
void ClearScreen(void);

///Checks whether file is opened successfully or not
void AssertFileState(FILE *fptr);

#endif // COMMONFUNCTIONS_H_INCLUDED
