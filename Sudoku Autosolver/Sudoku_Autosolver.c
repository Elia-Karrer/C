/**
    Sudoku Autosolver
    All rights reserved
    Elia Karrer, 11/30/2021
**/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "console.c"

typedef unsigned char byte;
typedef struct Line line;

struct Line {byte a[9];};

void input_field(byte *);
void print_field(byte *);
void convert_to_blocks(byte *, line[9]);
void convert_to_rows(byte *, line[9]);
void convert_to_cols(byte *, line[9]);
void autocheck(byte *);
bool autofill(byte *);
bool get_empty(byte *, byte *, byte *);
bool check_coordinate(byte *, byte, byte, byte);
bool check_block_doubles(block);
bool check_line_doubles(line);


int main()
{
    setCursorType(0);
    system("title Sudoku Autosolver v1.0");
    byte field[81] = {0};

    input_field(&field);
    system("color 0f & cls");
    if(autofill(&field))
        print_field(&field);
    else
    {
        gotoxy(0, 0);
        textbackground(BLACK);
        textcolor(WHITE);
        printf("No solution found!");
    }
    gotoxy(0, 10);
    textbackground(BLACK);
    textcolor(WHITE);
}


bool autofill(byte *field)
{
    byte x, y, n;

    if(get_empty(field, &x, &y)) return 1;
    for(n = 1; n < 10; n++)
    {
        if(check_coordinate(field, x, y, n) == 0)
        {
            field[y*9+x] = n;
            if(autofill(field)) return 1;
            field[y*9+x] = 0;
        }
    }
    return 0;
}

bool get_empty(byte* field, byte* cx, byte* cy)
{
    byte x, y;
    for(y = 0; y < 9; y++)
        for(x = 0; x < 9; x++)
            if(field[y*9+x] == 0)
            {
                *cx = x;
                *cy = y;
                return 0;
            }
    return 1;
}

bool check_line_doubles(line l)
{
    for(byte i = 0; i < 9; i++)
        for(byte j = 0; j < 9; j++)
            if(i != j && (l.a[i] != 0 && l.a[j] != 0) && l.a[i] == l.a[j])
                return 1;
    return 0;
}

bool check_coordinate(byte* field, byte x, byte y, byte n)
{
    byte i, j, before = field[y*9+x];
    line block, row, col;

    ///WRITE
    field[y*9+x] = n;
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            block.a[j+i*3] = field[(i*9)+j + (x/3)*3+(y/3)*3*9];
    for(i = 0; i < 9; i++)
        row.a[i] = field[y*9+i];
    for(i = 0; i < 9; i++)
        col.a[i] = field[i*9+x];

    ///CHECK & RETURN
    field[y*9+x] = before;
    return(check_line_doubles(block) || check_line_doubles(row) || check_line_doubles(col));
}

void input_field(byte *field)
{
    print_field(field);
    byte x = 4, y = 4, input;
    while(1)
    {
        ///CURSOR
        gotoxy(x*2, y);
        textcolor(((x/3 + y/3) & 1) ? WHITE : BLACK);
        textbackground(CYAN);
        if(field[y*9+x] != 0) printf("%d ", field[y*9+x]); else printf("  ");
        textbackground(((x/3 + y/3) & 1) ? BLACK : WHITE);
        gotoxy(0, 20);
        textcolor(WHITE);
        textbackground(BLACK);
        printf("Press ENTER to solve automatically");

        ///AUTOCHECK
        autocheck(field);

        ///INPUT
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
        if(field[y*9+x] != 0) printf("%d ", field[y*9+x]); else printf("  ");

        ///NEW COORDINATE
        if((input == 'W' || input == 'w') && y > 0) y--;
        if((input == 'A' || input == 'a') && x > 0) x--;
        if((input == 'S' || input == 's') && y < 8) y++;
        if((input == 'D' || input == 'd') && x < 8) x++;
        if(input == 224) ///ARROW KEYS
        {
            input = getch();
            if(input == 72 && y > 0) y--;
            if(input == 75 && x > 0) x--;
            if(input == 80 && y < 8) y++;
            if(input == 77 && x < 8) x++;
        }
        if(input == 13)
            break;
    }
}

void print_field(byte *field)
{
    gotoxy(0, 0);
    for(int j = 0; j < 9; j++)
        for(int i = 0; i < 9; i++)
        {
            textcolor(((j/3 + i/3) & 1) ? WHITE : BLACK);
            textbackground(((j/3 + i/3) & 1) ? BLACK : WHITE);

            if(field[j*9+i] != 0) printf("%d %s", field[j*9+i], (i == 8) ? "\n" : "");
            else printf("  %s", (i == 8) ? "\n" : "");
        }
}

void autocheck(byte *field)
{
    byte i, j, f;
    line blocks[9], rows[9], cols[9];
    bool block_doubles[9] = {0};
    bool rows_doubles[9] = {0};
    bool cols_doubles[9] = {0};

    ///PRINT DOUBLE BLOCKS
    convert_to_blocks(field, &blocks);
    for(i = 0; i < 9; i++)
        block_doubles[i] = check_line_doubles(blocks[i]);
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

void convert_to_blocks(byte *field, line blocks[9])
{
    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
            blocks[((y/3)*3)+(x/3)].a[x%3+(y%3)*3] = field[y*9+x];
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
