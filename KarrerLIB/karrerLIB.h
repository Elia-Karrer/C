/**
    KarrerLIB, a Library to make C easier
    All rights reserved
    Elia Karrer, 2022
*/

#ifndef KARRERLIB_H_
#define KARRERLIB_H_


///Size of total RAM in system (MB)
unsigned int ram_total(void);

///Size of used RAM in system (MB)
unsigned int ram_used(void);

///Size of free RAM in system (MB)
unsigned int ram_free(void);

///Makes the memory full, parameter: MB
void ram_flood(unsigned int);

///Presses a specific key
void pressKey(char);

///Sets the Title of console
void title(const char*);

///Hides the console window
void hideConsole(void);

///Opens a new cmd window as admin
void cmd_admin(void);



///**********String data type**********
typedef char* string;

///Returns a new string
char* newstr(const char*);

///Returns string of string1 + string2
char* addstr(const char*, const char*);

///Returns the uppercase of input
char* upper(char*);

///Returns the lowercase of input
char* lower(char*);


#endif // KARRERLIB_H_
