/**
    Bitcoin / Bitcoin Cash / Dogecoin - Address existence checker
    All rights reserved
    Elia Karrer, 2021
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define S0(i) (((i >> 7 ) | (i << 25)) ^ ((i >> 18) | (i << 14)) ^ (i >> 3))
#define S1(i) (((i >> 17) | (i << 15)) ^ ((i >> 19) | (i << 13)) ^ (i >> 10))
#define E0(i) (((i >> 2 ) | (i << 30)) ^ ((i >> 13) | (i << 19)) ^ ((i >> 22) | (i << 10)))
#define E1(i) (((i >> 6 ) | (i << 26)) ^ ((i >> 11) | (i << 21)) ^ ((i >> 25) | (i << 7 )))
#define CH(a, b, c) ((a&b) ^ ((~a)&c))
#define MJ(a, b, c) ((a&b) ^ (a&c) ^ (b&c))
#define e32 0xFFFFFFFF

typedef unsigned int WRD;
const WRD sqrts[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
const WRD cons[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
unsigned char HEX[] = "0123456789abcdef";
unsigned char HEX2[] = "0123456789ABCDEF";
unsigned char BASE58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

typedef struct Block BLOCK;
struct Block
{
    WRD b[64];
    WRD content;
};

typedef struct uint256_t uint256;
struct uint256_t {unsigned int word[8];};


void sha256(unsigned char *s, WRD *ret, int len)
{
    int i, j, n, x, blocklen;
    WRD out[8], bb[8], T1, T2;

    //Calculate blocklength
    blocklen = ((len*8 + 64) % 512 == 0) ? ((len*8 + 64) / 512) : (((len*8 + 64) + (512-((len*8 + 64) % 512))) / 512);

    //Initialize blocks
    BLOCK a[blocklen];
    for(i = 0; i < blocklen; i++)
    {
        for(int j = 0; j < 64; j++)
            a[i].b[j] = 0;
        a[i].content = 512;
    }

    //Initialize content length
    a[blocklen-1].content = (((len*8 + 64) % 512) == 0) ? 448 : (len*8) % 512;

    //Copy string to blocks
    for(i = 0; i < blocklen; i++)
        for(j = 0; j < (a[i].content / 8); j++)
            a[i].b[j/4] += ((j%4) == 3) ? s[i*64+j] : (s[i*64+j] * (1 << (24 - ((j%4)*8))));

    //Padding bit
    x = a[blocklen-1].content;
    if(x != 448)
        a[blocklen-1].b[x/32] += (1 << (31 - (x % 32)));

    //Write length
    a[blocklen-1].b[15] = (len*8);

    //Extend message schedule
    for(j = 0; j < blocklen; j++)
        for(i = 16; i < 64; i++)
           a[j].b[i] = (S1(a[j].b[i-2]) + a[j].b[i-7] + S0(a[j].b[i-15]) + a[j].b[i-16]) & e32;

    //Set roots as initial hash values
    for(i = 0; i < 8; i++)
    {
        bb[i] = sqrts[i];
        out[i] = sqrts[i];
    }

    //Core
    for(i = 0; i < blocklen; i++)
    {
        for(n = 0; n < 64; n++)
        {
            T1 = (E1(out[4]) + CH(out[4], out[5], out[6]) + out[7] + (a[i].b[n]) + cons[n]) & e32;
            T2 = (E0(out[0]) + MJ(out[0], out[1], out[2])) & e32;
            for(j = 7; j > 0; j--)
                out[j] = out[j-1];
            out[0] = (T1 + T2) & e32;
            out[4] = (out[4] + T1) & e32;
        }
        for(j = 0; j < 8; j++)
        {
            out[j] = (out[j] + bb[j]) & e32;
            bb[j] = out[j];
        }
    }

    //Output
    for(i = 0; i < 8; i++)
        ret[i] = out[i];
}

void printhash(WRD *a)
{
    for(int i = 0; i < 8; i++)
        printf("%08x", a[i]);
    printf("\n");
}

int get_hex(unsigned char a)
{
    for(int i = 0; i < 16; i++)
        if(a == HEX[i])
            return i;
    for(int i = 0; i < 16; i++)
        if(a == HEX2[i])
            return i;
}

int get_base58(unsigned char c)
{
    for(int i = 0; i < 58; i++)
        if(c == BASE58[i])
            return i;
}

void WRDtostr(WRD *a, unsigned char *c)
{
    WRD t;
    int i, j;

    for(i = 0; i < 64; i++)
        c[i] = "0";

    for(i = 0; i < 8; i++)
    {
        t = a[i];
        for(j = 0; j < 8; j++)
        {
            c[i*8+7-j] = HEX[t&15];
            t >>= 4;
        }
    }
}

void WRDtochar(WRD *a, unsigned char *c)
{
    WRD t;
    int i, j;

    for(i = 0; i < 32; i++)
        c[i] = 0;

    for(i = 0; i < 8; i++)
    {
        t = a[i];
        for(j = 0; j < 4; j++)
        {
            c[i*4+3-j] = t&255;
            t >>= 8;
        }
    }
    c[32] = NULL;
}

void address_base58_to_bin(unsigned char *address, bool *array)
{
    bool bits[256];
    int i, j;
    uint256 a;
    uint256 t;
    uint256_reset(&a);
    uint256_reset(&t);

    //Decode base58
    for(i = 0; i < 34; i++)
    {
        uint256_reset(&t);
        t.word[7] = get_base58(address[33-i]);
        for(j = 0; j < i; j++)
            uint256_multiplyself(&t, 58);

        uint256_add(&a, &t, &a);
    }

    uint256_get(&a, &bits);
    for(i = 0; i < 200; i++)
        array[i] = bits[i+56];
}

bool check_address(unsigned char *address)
{
    WRD w[8];
    unsigned char in[21];
    unsigned char temp_sha256[32];

    unsigned char address_char[25] = {0};
    unsigned int checksum;
    bool address_bin[200] = {0};
    int i, j;

    address_base58_to_bin(address, &address_bin);

    //Write binary to char
    for(i = 0; i < 25; i++)
        for(j = 0; j < 8; j++)
            address_char[i] += address_bin[i*8+j]*(1<<(7-j));
    checksum = address_char[21]*256*256*256 + address_char[22]*256*256 + address_char[23]*256 + address_char[24];

    for(i = 0; i < 22; i++)
        in[i] = address_char[i];

    //Double hash 4 byte of address
    sha256(&in, &w, 21);
    WRDtochar(&w, &temp_sha256);
    sha256(&temp_sha256, &w, 32);

    return(checksum == w[0]);
}

///********************************256 bit INTEGER********************************

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

///********************************256 bit INTEGER********************************


int main()
{
    unsigned char * input[34];

    printf("Enter address: ");
    gets(input);

    if(check_address(&input))
        printf("--> address exists\n");
    else
        printf("--> address does not exist\n");
}
