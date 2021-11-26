/**
    256 bit unsigned integer
    All rights reserved
    Elia Karrer, 2021
**/

#include <stdio.h>
#include <stdlib.h>

typedef struct uint256_t uint256;
struct uint256_t {unsigned int word[8];};


void uint256_add(uint256 *num_a, uint256 *num_b, uint256 *out_c)
{
    bool a[256] = {0}; uint256_get(num_a, &a);
    bool b[256] = {0}; uint256_get(num_b, &b);
    bool c[256] = {0}; uint256_reset(&c);
    bool carry = 0;

    for(int i = 255; i >= 0; i--)
    {
        if(carry == 0)
        {
            carry = a[i] & b[i];
            c[i] = a[i] ^ b[i];
        }
        else
        {
            if(a[i] ^ b[i])
                c[i] = 0;

            if(a[i] & b[i])
                c[i] = 1;

            if(a[i] + b[i] == 0)
            {
                c[i] = 1;
                carry = 0;
            }
        }
    }

    uint256_set(out_c, &c);
}

void uint256_multiplyself(uint256 *var, unsigned int factor2)
{
    uint256 factor1;
    unsigned int i;
    factor2--;

    for(i = 0; i < 8; i++)
        factor1.word[i] = var -> word[i];

    for(i = 0; i < factor2; i++)
        uint256_add(var, &factor1, var);
}

void uint256_reset(uint256 *var)
{
    for(int i = 0; i < 8; i++)
        var -> word[i] = 0;
}

void uint256_get(uint256 *var, bool *output)
{
    unsigned int temp;
    for(int i = 0; i < 256; i++)
    {
        if((i & 31) == 0)
            temp = var -> word[7-(i>>5)];
        output[255-i] = temp & 1;
        temp >>= 1;
    }
}

void uint256_set(uint256 *var, bool *input)
{
    int j;
    unsigned int temp;
    for(int i = 0; i < 8; i++)
    {
        temp = 0;
        for(j = 0; j < 32; j++)
            temp += (input[(i<<5)+j] == 1) ? 1 << 31-j : 0;
        var -> word[i] = temp;
    }
}

void uint256_lsh(uint256 *var, int n)
{
    uint256_multiplyself(var, 2);
}

void uint256_rsh(uint256 *var, int n)
{
    bool temparr[256] = {0};
    unsigned int i, j;

    uint256_get(var, &temparr);

    //Shift n-times to the right
    for(j = 0; j < n; j++)
        for(i = 255; i >= 0; i--)
            temparr[i] = i == 0 ? 0 : temparr[i-1];

    uint256_set(var, &temparr);
}

void uint256_print_hex(uint256 *a)
{
    for(int i = 0; i < 8; i++)
        printf("%08x ", a -> word[i]);
    printf("\n");
}

void uint256_print_bin(uint256 *a)
{
    bool temparr[256] = {0};
    uint256_get(a, &temparr);
    for(int i = 0; i < 256; i++)
        printf("%s%u", ((i & 31) == 0 && i != 0) ? " " : "", temparr[i]);
    printf("\n");
}