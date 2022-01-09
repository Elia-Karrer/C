#include <windows.h>
#include <time.h>

unsigned int ram_total(void)
{
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx (&mem);

    return mem.ullTotalPhys >> 20;
}

unsigned int ram_used(void)
{
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx (&mem);

    return (mem.ullTotalPhys-mem.ullAvailPhys) >> 20;
}

unsigned int ram_free(void)
{
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx (&mem);

    return mem.ullAvailPhys >> 20;
}

void ram_flood(unsigned int mb)
{
    for(unsigned int i = 0; i < 1024*mb; i++)
        if(malloc(1024) == NULL)
            break;
}

void pressKey(char key)
{
    keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void title(const char* input)
{
    int i, l;
    const char* prefix = "title ";

    for(l = 0; input[l] != 0; l++);
    char * fullCommand = (char*) malloc((l+6) * sizeof(char));

    for(i = 0; i < 6; i++)
        fullCommand[i] = prefix[i];
    for(i = 0; i < l; i++)
        fullCommand[i+6] = input[i];
    fullCommand[l+6] = 0;

    system(fullCommand);
}

void hideConsole(void)
{
    FreeConsole();
}

void cmd_admin(void)
{
    system("powershell -Command \"Start-Process cmd -Verb RunAs\"");
}



///******************** Data Types & functions for data types ********************

typedef char* string;

char* newstr(const char* init)
{
    unsigned int i, l;

    ///Get length of input
    for(l = 0; init[l] != 0; l++);

    ///Reserve Memory
    char* output = (char*) malloc((l+1) * sizeof(char));
    if(output == NULL)
    {
        printf("Error while reserving memory!");
        return NULL;
    }

    ///Write
    for(i = 0; i < l; i++)
        output[i] = init[i];
    output[l] = 0;

    return output;
}

char* addstr(const char* str1, const char* str2)
{
    unsigned int i, len1, len2, lensum;
    char *output;

    ///Exception for ""
    if(str1[0] == 0 && str2[0] == 0)
    {
        output = (char*) malloc(sizeof(char));
        *output = 0;
        return output;
    }
    if(str1[0] == 0)
        return newstr(str2);
    if(str2[0] == 0)
        return newstr(str1);

    ///Get length of input
    for(len1 = 0; str1[len1] != 0; len1++);
    for(len2 = 0; str2[len2] != 0; len2++);
    lensum = len1 + len2;

    ///Reserve Memory
    output = (char*) malloc((lensum+1) * sizeof(char));
    if(output == NULL)
    {
        printf("Error while reserving memory!");
        return NULL;
    }

    ///Write
    for(i = 0; i < len1; i++)
        output[i] = str1[i];
    for(i = 0; i < lensum; i++)
        output[i+len1] = str2[i];
    output[lensum] = 0;

    return output;
}



unsigned int stringlen(const char * str)
{
    int i;
    for(i = 0; str[i] != 0; i++);
    return i;
}

char* upper(char* str)
{
    unsigned int i;
    const char offset = 'a' - 'A';
    string newString = newstr(str);

    for(i = 0; ; i++)
    {
        if(str[i] == 0)
            break;
        if(newString[i] >= 'a' && newString[i] <= 'z')
            newString[i] -= offset;
    }
    newString[i] = 0;
    return newString;
}

char* lower(char* str)
{
    unsigned int i;
    const char offset = 'a' - 'A';
    string newString = newstr(str);

    for(i = 0; ; i++)
    {
        if(str[i] == 0)
            break;
        if(newString[i] >= 'A' && newString[i] <= 'Z')
            newString[i] += offset;
    }
    newString[i] = 0;
    return newString;
}
