#include "AdminFunctions.h"
#include "CommonFunctions.h"

///Displays the Functions of Admin
void DisplayAdminFunctions()
{
    char username[MAX_LENGTH_USERNAME], password[MAX_LENGTH_PASSWORD];
    char choice;

    ClearScreen();

    label_login:
    if(login_attempt != 0)
    {
        printf("Enter your username: ");
        scanf(" %[^\n]s", &username);

        printf("Enter your password: ");
        scanf(" %[^\n]s", &password);

        if(VerifyLoginCredentials(username, password))
        {
            while(1)
            {
                label_crud_dish:
                ClearScreen();

                printf("Press 1 to Add Dish.\n");
                printf("Press 2 to Update Dish.\n");
                printf("Press 3 to Delete Dish.\n");
                printf("Press 4 to exit.\n");
                printf("Enter your choice: ");
                scanf(" %c", &choice);

                switch(choice)
                {
                    case '1': AddDishDetails();
                            break;

                    case '2': UpdateDishDetails();
                            break;

                    case '3': DeleteDish();
                            break;

                    case '4': goto label_function_exit;

                    default: printf("Invalid Choice! Do you want to try again? [Y/N]: ");
                            scanf(" %c", &choice);

                            if(choice != 'Y' || choice != 'y')
                            {
                                goto label_crud_dish;
                            }
                }
            }
        }
        else
        {
            ClearScreen();
            printf("Invalid Username or password! Please try again. You have %u attempts left!", login_attempt--);

            printf("\nDo you want to try again? [Y/N]: ");
            scanf(" %c", &choice);

            if(choice == 'Y' || 'y')
            {
                ClearScreen();
                goto label_login;
            }
        }
    }
    else
    {
        printf("You can't try again as you've exhausted the maximum login attempts.");
        exit(EXIT_SUCCESS);
    }

    label_function_exit:
        ClearScreen();
}

///Verifies the Admin login Credentials
int VerifyLoginCredentials(const char *username, const char *password)
{
    FILE *fptr = fopen(PATH_ADMIN_CREDENTIALS_PATH, "rb");
    AssertFileState(fptr);

    char temp_username[MAX_LENGTH_USERNAME], temp_password[MAX_LENGTH_PASSWORD];

    if(fread(temp_username, MAX_LENGTH_USERNAME, 1, fptr) != 1)
    {
        printf("\nError Occurred while reading Username from file\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }

    if(fread(temp_password, MAX_LENGTH_PASSWORD, 1, fptr) != 1)
    {
        printf("\nError Occurred while reading Password from file!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }

    fclose(fptr);

    if((strcmp(username, temp_username) == 0) && (strcmp(password, temp_password) == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
