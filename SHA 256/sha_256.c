/**
    SHA256 in C
    All rights reserved
    Elia Karrer, 2021
**/

#include <stdio.h>
#include <string.h>

#define S0(i) (((i >> 7 ) | (i << 25)) ^ ((i >> 18) | (i << 14)) ^ (i >> 3))
#define S1(i) (((i >> 17) | (i << 15)) ^ ((i >> 19) | (i << 13)) ^ (i >> 10))
#define E0(i) (((i >> 2 ) | (i << 30)) ^ ((i >> 13) | (i << 19)) ^ ((i >> 22) | (i << 10)))
#define E1(i) (((i >> 6 ) | (i << 26)) ^ ((i >> 11) | (i << 21)) ^ ((i >> 25) | (i << 7 )))
#define CH(a, b, c) ((a&b) ^ ((~a)&c))
#define MJ(a, b, c) ((a&b) ^ (a&c) ^ (b&c))
#define e32 0xFFFFFFFF
typedef unsigned int WRD;
typedef struct Block { WRD b[64]; WRD content; } BLOCK;
const WRD sqrts[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
const WRD cons[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void sha256(char *s)
{
    int i, j, n, x, blocklen, len = strlen(s);
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
        printf("%08x", out[i]);
}


int main()
{
    sha256("TEXT TO HASH");
}
