#include <stdio.h>
#include <string.h>

int strcmp_karrer(const char *a, const char *b)
{
    for(int i = 0; a[i] != '\0'; i++)
        if(a[i] != b[i])
            return 1;
    return 0;
}

int main()
{
    const char * str1 = "TEXT";
    const char * str2 = "TEXT";

    printf("string.h --> str1 %c= str2\n", strcmp(str1, str2) ? '!' : '=');
    printf("karrer   --> str1 %c= str2\n", strcmp_karrer(str1, str2) ? '!' : '=');
}
