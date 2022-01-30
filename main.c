#ifndef COMMONFUNCTIONS_H_INCLUDED
    #define COMMONFUNCTIONS_H_INCLUDED
    #include "CommonFunctions.h"
#endif // COMMONFUNCTIONS_H_INCLUDED

#ifndef ADDDISH_H_INCLUDED
    #define ADDDISH_H_INCLUDED
    #include "AddDish.h"
#endif // ADDDISH_H_INCLUDED

#ifndef PLACEORDER_H_INCLUDED
    #define PLACEORDER_H_INCLUDED
    #include "PlaceOrder.h"
#endif // PLACEORDER_H_INCLUDED

#ifndef ADMINFUNCTIONS_H_INCLUDED
    #define ADMINFUNCTIONS_H_INCLUDED
    #include "AdminFunctions.h"
#endif // ADMINFUNCTIONS_H_INCLUDED

int main()
{
    printf("\nHello! Welcome to the Food Order Management System!\n");

    char choice;

    while(1)
    {
        ClearScreen();

        printf("Press 1 if you're the admin.\n");
        printf("Press 2 if you're a customer.\n");
        printf("Press 3 to exit.\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch(choice)
        {
            case '1': DisplayAdminFunctions();
                      break;

            case '2': DisplayCustomerFunctions();
                      break;

            case '3': exit(EXIT_SUCCESS);

            default: printf("Invalid Choice! Do you want to try again? [Y/N]: ");
                     scanf(" %c", &choice);

                     if(choice != 'Y' || choice != 'y')
                     {
                         exit(EXIT_SUCCESS);
                     }

                     ClearScreen();
        }
    }

    return 0;
}
