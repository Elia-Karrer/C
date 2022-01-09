/**
        Demo for KarrerLIB
*/

#include <stdio.h>
#include "karrerLIB.h"


int main()
{

    ///String data type demo:
    string x;

    x = newstr("tExT\n");
    printf("Source:     %s", x);

    x = upper(x);
    printf("Uppercase:  %s", x);

    x = lower(x);
    printf("Lowercase:  %s", x);

    x = newstr("New text");
    printf("New string: %s\n");

    x = addstr(x, ", added TEXT");
    printf("Add string: %s\n");

    ///Functions demo:
    int ram =     ram_total();
    int freeram = ram_free();
    int usedram = ram_used();

    printf("\nTotal ram: %d MB", ram);
    printf("\nFree ram:  %d MB", freeram);
    printf("\nUsed ram:  %d MB\n\n", usedram);

    getchar();

    ///Hides the console, but C could work further in the background ^^
    hideConsole();


    /*
        //Would just fill up your RAM 4GB more
        ram_flood(4000);
    */
}
