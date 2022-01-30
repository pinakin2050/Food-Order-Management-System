#include "CommonFunctions.h"

///Clear the console screen
void ClearScreen()
{
    system("@cls||clear");
}

///Checks if file is opened successfully or not.
void AssertFileState(FILE *fptr)
{
    if(fptr == NULL)
    {
        printf("\nCan't open the file to add the dish details! Kindly check the provided file path.\n");
        exit(EXIT_FAILURE);
    }
}


