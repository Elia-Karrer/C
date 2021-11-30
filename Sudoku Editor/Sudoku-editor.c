/**
    Sudoku Editor
    All rights reserved
    Elia Karrer, 11/28/2021
**/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "console.c"

typedef unsigned char byte;
typedef struct Block block;
typedef struct Line line;

struct Block {byte a[3][3];};
struct Line {byte a[9];};

bool check_block_doubles(block);
bool check_line_doubles(line);
void input_field(byte *);
void print_field(byte *);
void convert_to_blocks(byte *, block[9]);
void convert_to_rows(byte *, line[9]);
void convert_to_cols(byte *, line[9]);
void autocheck(byte *);


int main()
{
    byte field[81] = {0};
    setCursorType(0);
    input_field(&field);

    gotoxy(0, 19);
    textbackground(BLACK);
    textcolor(WHITE);
}


void autocheck(byte *field)
{
    byte i, j, f;
    block blocks[9];
    bool block_doubles[9] = {0};
    line rows[9];
    bool rows_doubles[9] = {0};
    line cols[9];
    bool cols_doubles[9] = {0};

    ///PRINT DOUBLE BLOCKS
    convert_to_blocks(field, &blocks);
    for(i = 0; i < 9; i++)
        block_doubles[i] = check_block_doubles(blocks[i]);
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
        {
            textbackground(block_doubles[i*3+j] ? RED : GREEN);
            for(f = 0; f < 3; f++)
            {
                gotoxy(j*6, i*3+10+f);
                printf("      ");
            }
        }

    ///PRINT DOUBLE ROWS
    convert_to_rows(field, &rows);
    for(i = 0; i < 9; i++)
        rows_doubles[i] = check_line_doubles(rows[i]);
    for(i = 0; i < 9; i++)
    {
        textbackground(rows_doubles[i] ? RED : GREEN);
        gotoxy(20, i+10);
        printf("                  ");
    }

    ///PRINT DOUBLE COLS
    convert_to_cols(field, &cols);
    for(i = 0; i < 9; i++)
        cols_doubles[i] = check_line_doubles(cols[i]);
    for(i = 0; i < 9; i++)
    {
        textbackground(cols_doubles[i] ? RED : GREEN);
        for(f = 0; f < 9; f++)
        {
            gotoxy(40+i*2, f+10);
            printf("  ");
        }
    }
}

void input_field(byte *field)
{
    print_field(field);
    byte x = 0, y = 0, input;
    while(1)
    {
        ///CURSOR
        gotoxy(x*2, y);
        textcolor(((x/3 + y/3) & 1) ? WHITE : BLACK);
        textbackground(CYAN);
        if(field[y*9+x] != 0) printf("%d", field[y*9+x]); else printf(" ");
        textbackground(((x/3 + y/3) & 1) ? BLACK : WHITE);

        ///AUTOCHECK
        autocheck(field);

        input = getch();

        ///WRITE
        if(input > 47 && input < 58)
            field[y*9+x] = input - 48;
        if(input == 8)
            field[y*9+x] = 0;

        ///CLEAR LAST
        gotoxy(x*2, y);
        textcolor(((x/3 + y/3) & 1) ? WHITE : BLACK);
        textbackground(((x/3 + y/3) & 1) ? BLACK : WHITE);
        if(field[y*9+x] != 0) printf("%d", field[y*9+x]); else printf(" ");

        ///NEW COORDINATE
        if((input == 'W' || input == 'w') && y > 0) y--;
        if((input == 'A' || input == 'a') && x > 0) x--;
        if((input == 'S' || input == 's') && y < 8) y++;
        if((input == 'D' || input == 'd') && x < 8) x++;
        if(input == 224)///ARROW KEYS
        {
            input = getch();
            if(input == 72 && y > 0) y--;
            if(input == 75 && x > 0) x--;
            if(input == 80 && y < 8) y++;
            if(input == 77 && x < 8) x++;
        }
        if(input == 10)
            break;
    }
}

void print_field(byte *field)
{
    for(int j = 0; j < 9; j++)
        for(int i = 0; i < 9; i++)
        {
            textcolor(((j/3 + i/3) & 1) ? WHITE : BLACK);
            textbackground(((j/3 + i/3) & 1) ? BLACK : WHITE);

            if(field[j*9+i] != 0) printf("%d %s", field[j*9+i], (i == 8) ? "\n" : "");
            else printf("  %s", (i == 8) ? "\n" : "");
        }
}

void convert_to_blocks(byte *field, block blocks[9])
{
    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
            blocks[((y/3)*3)+(x/3)].a[x%3][y%3] = field[y*9+x];
}

void convert_to_rows(byte *field, line rows[9])
{
    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
            rows[y].a[x] = field[y*9+x];
}

void convert_to_cols(byte *field, line cols[9])
{
    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
            cols[x].a[y] = field[y*9+x];
}

bool check_block_doubles(block b)
{
    byte i, j;
    byte temp[9];
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            temp[i*3+j] = b.a[j][i];

    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if(i != j && (temp[i] != 0 && temp[j] != 0) && temp[i] == temp[j])
                return 1;
    return 0;
}

bool check_line_doubles(line l)
{
    for(byte i = 0; i < 9; i++)
        for(byte j = 0; j < 9; j++)
            if(i != j && (l.a[i] != 0 && l.a[j] != 0) && l.a[i] == l.a[j])
                return 1;
    return 0;
}
