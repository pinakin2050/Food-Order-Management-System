#ifndef STDIO_H_INCLUDED
    #define STDIO_H_INCLUDED
    #include <stdio.h>
#endif // STDIO_H_INCLUDED

#ifndef STRING_H_INCLUDED
    #define STRING_H_INCLUDED
    #include <string.h>
#endif // STRING_H_INCLUDED

#ifndef ADMINFUNCTIONS_H_INCLUDED
#define ADMINFUNCTIONS_H_INCLUDED

#define MAX_LENGTH_USERNAME 10
#define MAX_LENGTH_PASSWORD 10

#define PATH_ADMIN_CREDENTIALS_PATH "E:\\Project Work\\Food Order Management System\\Code\\Food-Order-Management-System\\Data Files\\Admin Credentials.txt"

///Displays the Functions of Admin
void DisplayAdminFunctions(void);

///Verifies the Admin login Credentials
int VerifyLoginCredentials(const char *username, const char *password);

static unsigned int login_attempt = 3;

#endif // ADMINFUNCTIONS_H_INCLUDED
