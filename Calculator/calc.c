/**
    All rights reserved, Elia Karrer
    Calculator, possible operators: +, -, *, /, %, ^
    Brackets not supported yet...
*/

#include <stdio.h>


typedef struct Variable {
    char type;

    int i;
    float f;

} var_t;


void var_setInt(var_t *var, int value)
{
    var -> i = value;
    var -> type = 'i';
}


void var_setFloat(var_t *var, float value)
{
    var -> f = value;
    var -> type = 'f';
}


void var_print(var_t *var)
{
    ///Get data type
    char type = var -> type;

    ///Print variable with right data type
    if(type == 'i')
    {
        printf("%d", var -> i);
        return;
    }
    if(type == 'f')
    {
        printf("%f", var -> f);
        return;
    }

    printf("Error -> var_print, can't print variable without data type");
}


unsigned int len(const char * str)
{
    int i;

    for(i = 0; str[i] != 0; i++);
    return i;
}


void copyString(const char * src, char * dest)
{
    int i, l = len(src);

    for(i = 0; i < l; i++)
        dest[i] = src[i];
    dest[l] = 0;
}


int stringContains(const char * str, const char * set)
{
    int i, j, l = len(str), ll = len(set);

    for(i = 0; i < l; i++)
        for(j = 0; j < ll; j++)
            if(str[i] == set[j])
                return 1;
    return 0;
}


int charInSet(const char a, const char * set)
{
    int i, l = len(set);

    for(i = 0; i < l; i++)
        if(set[i] == a)
            return 1;
    return 0;
}


void shiftStringLeft(char * str)
{
    int i, l = len(str);

    for(i = 0; i < l-1; i++)
        str[i] = str[i+1];
    str[l-1] = 0;
}


void removeSpaces(char * input)
{
    int i, c = 0, l = len(input);
    char output[l];

    for(i = 0; i < l; i++)
        if(input[i] != ' ')
        {
            output[c] = input[i];
            c++;
        }

    output[c] = 0;
    copyString(output, input);
    input[c] = 0;
}


char getLowerChr(const char chr)
{
    if(chr > 64 && chr < 90)
        return chr + 32;
    return chr;
}


void lowerStr(char * str)
{
    int i, l = len(str);

    for(i = 0; i < l; i++)
        str[i] = getLowerChr(str[i]);
}


int getIntFromHexChar(const char chr)
{
    int i;
    const char * table = "0123456789abcdef";

    for(i = 0; i < 16; i++)
        if(getLowerChr(chr) == table[i])
            return i;
    return -1;
}


int getIntFromChar(const char chr)
{
    int i;
    const char * table = "0123456789";

    for(i = 0; i < 10; i++)
        if(chr == table[i])
            return i;
    return -1;
}


int getIntFromBinChar(const char chr)
{
    if(chr == '0')
        return 0;
    if(chr == '1')
        return 1;
    return -1;
}


int getIntFromHexStr(const char * str)
{
    int tmp, ret = 0, mult = 1, l = len(str);
    for(int i = 0; i < l; i++)
    {
        tmp = getIntFromHexChar(str[l-i-1]);
        ret += tmp * mult;
        mult <<= 4;
    }
    return ret;
}


float getFloatFromStr(const char * s)
{
    char buf1[16] = {0}, buf2[16] = {0};
    int i, temp, l = len(s), divisor = 10;
    float result = 0, currentDigit;

    ///If float number is an integer
    if(stringContains(s, ".") == 0)
        return (float) getIntFromStr(s);

    if(s[0] != '.')
    {
        for(i = 0; i < l && i < 16; i++)
        {
            if(s[i] == '.')
            {
                buf1[i] = 0;
                break;
            }

            buf1[i] = s[i];
        }
        temp = i+1;
        for(i = 0; i < l && i < 16; i++)
            buf2[i] = s[i+temp];
        buf2[i] = 0;
    }
    ///If "0.f" is written as ".f"
    else
    {
        buf1[0] = '0';
        for(i = 1; i < l && i < 16; i++)
            buf2[i-1] = s[i];
        buf2[i-1] = 0;
    }

    ///Add integer part
    result += (float) getIntFromStr(buf1);

    ///Add decimal part
    for(i = 0; ; i++)
    {
        if(buf2[i] == 0)
            break;
        currentDigit = (float) getIntFromChar(buf2[i]);
        result += currentDigit / divisor;
        divisor *= 10;
    }

    return result;
}


int getIntFromStr(const char * s)
{
    char str[len(s)];
    copyString(s, str);

    ///CHECK FOR HEX NOTATION
    if(len(str) > 2)
        if(str[0] == '0' && str[1] == 'x')
        {
            shiftStringLeft(str);
            shiftStringLeft(str);
            return(getIntFromHexStr(str));
        }

    int tmp, i, ret = 0, mult = 1, l, isNeg = str[0] == '-';
    if(isNeg)
        shiftStringLeft(str);

    l = len(str);
    for(i = 0; i < l; i++)
    {
        tmp = getIntFromChar(str[l-i-1]);
        ret += tmp * mult;
        mult *= 10;
    }
    if(isNeg)
        ret = 0 - ret;
    return ret;
}


int getIntFromBinStr(char * str)
{
    int i, mult = 1, l = len(str), ret = 0;

    for(i = 0; i < l; i++)
    {
        if(getIntFromBinChar(str[l-i-1]))
           ret += mult;
        mult <<= 1;
    }
    return ret;
}


char getDtypeOfNumber(char *str)
{
    return stringContains(str, ".") ? 'f' : 'i';
}


void intToString(int num, char * dest)
{
    int i, l, ctr = 0;
    const char lst[] = "0123456789";
    char tmp[14] = {0};

    for(i = 0; num != 0; i++)
    {
        tmp[ctr] = lst[num % 10];
        ctr++;
        num -= num % 10;
        num /= 10;
        i++;
    }

    l = len(tmp);
    for(i = 0; i < l; i++)
        dest[i] = tmp[l-i-1];
}


int power(int base, int exponent)
{
    int i, result = base;

    if(exponent == 0)
        return 1;
    if(exponent == 1)
        return base;

    for(i = 0; i < exponent-1; i++)
        result *= base;

    if(exponent < 0)
        return 1 / result;
    return result;
}


float floatPower(float base, int exponent)
{
    float result = base;

    if(exponent == 0)
        return 1;
    if(exponent == 1)
        return base;

    for(int i = 0; i < exponent-1; i++)
        result *= base;

    if(exponent < 0)
        return 1 / result;
    return result;
}


int hasDecimals(float x)
{
    if(x == ((int) x))
        return 0;
    return 1;
}


var_t calculateSimple(char * input)
{
    char parts[64*16] = {0};
    int i, j, n, l = len(input);
    var_t result;

    ///parts[ctr*16 + chrctr]
    int ctr = 0, chrctr = 0;

    ///Checks if string starts with
    ///"n..." or "+..." or "-..."
    if(getIntFromChar(input[0]) == -1 && input[0] != '-' && input[0] != '+')
    {
        printf("Error -> calculateSimple, char[0] = '%c'\n", input[0]);
        result.type = 'i';
        result.i = 0;
        return result;
    }

    ///Copy string to parts
    for(i = 0; i < l; i++)
    {
        ///If number
        if(getIntFromHexChar(input[i]) != -1 || input[i] == 'x')
        {
            parts[ctr*16+chrctr] = input[i];
            chrctr++;
        }
        ///If operator
        else
        {
            ///If current position in parts has data, then move forward
            if(chrctr != 0)
                ctr++;

            parts[ctr*16] = input[i];
            ctr++;
            chrctr = 0;
        }
    }

    ///Check if end is no operator
    if(getIntFromChar(parts[ctr*16]) != -1)
        ctr++;
    else
    {
        printf("Error -> calculateSimple, can't end with an operator ('%c')\n", parts[(ctr-1)*16]);
        result.type = 'i';
        result.i = 0;
        return result;
    }

    ///Combine operators
    char change;
    int changed;
    do
    {
        changed = 0;
        for(i = 0; i < ctr; i++)
        {
            ///Reset new operator
            change = 0;
            ///Find side by side operators and get new operator
            if(parts[i*16] == '+' && parts[(i+1)*16] == '+') change = '+';
            if(parts[i*16] == '-' && parts[(i+1)*16] == '-') change = '+';
            if(parts[i*16] == '+' && parts[(i+1)*16] == '-') change = '-';
            if(parts[i*16] == '-' && parts[(i+1)*16] == '+') change = '-';
            if(parts[i*16] == '*' && parts[(i+1)*16] == '+') change = '*';
            if(parts[i*16] == '/' && parts[(i+1)*16] == '+') change = '/';
            if(parts[i*16] == '%' && parts[(i+1)*16] == '+') change = '%';
            if(parts[i*16] == '^' && parts[(i+1)*16] == '+') change = '^';
            if(parts[i*16] == '*' && parts[(i+1)*16] == '*') change = '^';
            ///If found
            if(change != 0)
            {
                ///Change character
                parts[i*16] = change;
                ///Shift left
                for(j = i+1; j < ctr-1; j++)
                    for(n = 0; n < 16; n++)
                        parts[j*16+n] = parts[(j+1)*16+n];
                ///Clear last
                for(j = 0; j < 16; j++)
                    parts[(ctr-1)*16+j] = 0;
                ctr--;
                changed = 1;
            }
        }
    }
    while(changed == 1);

    ///Convert to base 10
    char buf[14] = {0};
    int intbuf;
    for(i = 0; i < ctr; i++)
    {
        ///Check for hex
        if(parts[i*16] == '0' && parts[i*16+1] == 'x')
        {
            ///Clear buffer
            for(j = 0; j < 14; j++)
                buf[j] = 0;
            intbuf = 0;
            ///Copy hex string to buffer
            for(j = 2; j < 16; j++)
                buf[j-2] = parts[i*16+j];
            ///Check for invalid characters in hex string
            for(j = 0; j < 14; j++)
                if(buf[j] != 0)
                    if(getIntFromHexChar(buf[j]) == -1)
                    {
                        printf("Error -> calculateSimple, '%c' is an invalid hex character\n", buf[j]);
                        result.type = 'i';
                        result.i = 0;
                        return result;
                    }
            ///Convert hex string to integer
            intbuf = getIntFromHexStr(buf);
            ///Copy integer as string to buffer
            intToString(intbuf, buf);
            ///Copy buffer to parts
            for(j = 0; j < 14; j++)
                parts[i*16+j] = buf[j];
        }
    }

    ///Combine float numbers
    int len1, len2;
    for(i = 0; i < ctr-2; i++)
    {
        ///if: "n" "." "n"
        if(getIntFromChar(parts[i*16]) != -1 && parts[(i+1)*16] == '.' && getIntFromChar(parts[(i+2)*16]) != -1)
        {
            ///Calculate lengths
            for(len1 = 0; parts[i*16+len1] != 0 && len1 < 16; len1++);
            for(len2 = 0; parts[(i+2)*16+len2] != 0 && len2 < 16; len2++);
            ///Copy "." to first array
            parts[i*16+len1] = '.';
            len1++;
            ///Copy remaining part
            for(j = len1; j < len2+len1 && j < 16; j++)
                parts[i*16+j] = parts[(i+2)*16+j-len1];
            ///Shift parts
            for(j = i+1; j < ctr-2; j++)
                for(n = 0; n < 16; n++)
                    parts[j*16+n] = parts[(j+2)*16+n];
            ctr -= 2;
        }
    }

    ///Check if operators are only 1 char long in parts
    for(i = 0; i < ctr; i++)
        if(charInSet(parts[i*16], "+-*/%^") && parts[i*16+1] != 0)
        {
            printf("Error -> calculateSimple, Operator '%c' followed by '%c'\n", parts[i*16], parts[i*16+1]);
            result.type = 'i';
            result.i = 0;
            return result;
        }


    ///Check that numbers are not side by side without operator in between
    for(i = 0; i < ctr; i++)
        if(getIntFromChar(parts[i*16]) != -1 && getIntFromChar(parts[(i+1)*16]) != -1)
        {
            int x, y;
            char checkBuf[16];
            for(j = 0; j < 16; j++)
            {
                checkBuf[j] = parts[i*16+j];
                if(checkBuf[j] == 0)
                    break;
            }
            x = getIntFromStr(checkBuf);
            for(j = 0; j < 16; j++)
            {
                checkBuf[j] = parts[(i+1)*16+j];
                if(checkBuf[j] == 0)
                    break;
            }
            y = getIntFromStr(checkBuf);
            printf("Error -> calculateSimple, Missing Operator between \"%d\" and \"%d\"\n", x, y);
            result.type = 'i';
            result.i = 0;
            return result;
        }

    ///Get datatypes
    char dtypes[ctr], dtypeBuf[16] = {0}, dtypeTemp;
    for(i = 0; i < ctr; i++)
        dtypes[i] = 0;
    for(i = 0; i < ctr; i++)
    {
        ///If operator
        if(charInSet(parts[i*16], "+-*/%^"))
           dtypes[i] = parts[i*16];
        ///If number
        else
        {
            ///Copy to buffer
            for(j = 0; j < 16; j++)
                dtypeBuf[j] = parts[i*16+j];
            ///Check datatype
            dtypeTemp = getDtypeOfNumber(dtypeBuf);
            if(dtypeTemp == 'E')
            {
                printf("Error -> calculateSimple, invalid Datatype in (\"%s\")!\n", buf);
                result.type = 'i';
                result.i = 0;
                return result;
            }
            dtypes[i] = dtypeTemp;
        }
    }

    ///Check, if there are no operators side by side
    for(i = 0; i < ctr-1; i++)
        if(charInSet(dtypes[i], "+-*/%^") && charInSet(dtypes[i+1], "*/%^"))
        {
            printf("Error -> calculateSimple, operators '%c' and '%c' side by side\n", dtypes[i], dtypes[i+1]);
            result.type = 'i';
            result.i = 0;
            return result;
        }

    ///Combine +- after */%^
    for(i = 0; i < ctr-2; i++)
    {
        if(charInSet(dtypes[i], "*/%^") && charInSet(dtypes[i+1], "+-") && charInSet(dtypes[i+2], "if"))
        {
            ///Set
            if(dtypes[i+1] == '+')
                for(j = 0; j < 16; j++)
                    parts[(i+1)*16+j] = parts[(i+2)*16+j];
            if(dtypes[i+1] == '-')
                for(j = 1; j < 16; j++)
                    parts[(i+1)*16+j] = parts[(i+2)*16+j-1];
            ///Shift
            for(j = i+2; j < ctr-1; j++)
                for(n = 0; n < 16; n++)
                    parts[j*16+n] = parts[(j+1)*16+n];
            for(j = i+2; j < ctr-1; j++)
                dtypes[j] = dtypes[j+1];
            dtypes[i+1] = dtypes[i+2];
            ctr--;
        }
    }

    ///Get result data type
    char resultDtype = 'i';
    for(i = 0; i < ctr; i++)
        if(dtypes[i] == 'f')
            resultDtype = 'f';



    ///Calculate for int case
    if(resultDtype == 'i')
    {
        int intResult = 0;

        ///Convert to int array
        int intArr[ctr];
        for(i = 0; i < ctr; i++)
        {
            if(dtypes[i] == 'i')
            {
                ///Reset Buffer
                char intTempStr[16] = {0};
                ///Copy to Buffer
                for(j = 0; j < 16; j++)
                {
                    intTempStr[j] = parts[i*16+j];
                    if(intTempStr[j] == 0)
                        break;
                }
                ///Copy int-string to integer array
                intArr[i] = getIntFromStr(intTempStr);
            }
        }

        ///Calculate ^
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {

                if(dtypes[i] == 'i' && dtypes[i+1] == '^' && dtypes[i+2] == 'i')
                {
                    ///Set
                    intArr[i] = power(intArr[i], intArr[i+2]);
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        intArr[j] = intArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        ///Calculate */%
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {

                if(dtypes[i] == 'i' && charInSet(dtypes[i+1], "*/%") && dtypes[i+2] == 'i')
                {
                    ///Set
                    if(dtypes[i+1] == '*') intArr[i] = intArr[i] * intArr[i+2];
                    if(dtypes[i+1] == '/') intArr[i] = intArr[i] / intArr[i+2];
                    if(dtypes[i+1] == '%') intArr[i] = intArr[i] % intArr[i+2];
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        intArr[j] = intArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        ///Calculate +-
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {

                if(dtypes[i] == 'i' && charInSet(dtypes[i+1], "+-") && dtypes[i+2] == 'i')
                {
                    ///Set
                    if(dtypes[i+1] == '+') intArr[i] = intArr[i] + intArr[i+2];
                    if(dtypes[i+1] == '-') intArr[i] = intArr[i] - intArr[i+2];
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        intArr[j] = intArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        ///Check for errors
        if(ctr != 1)
        {
            printf("Error -> calculateSimple, Error while calculating result\n");
            result.type = 'i';
            result.i = 0;
            return result;
        }
        else
            intResult = intArr[0];

        result.type = 'i';
        result.i = intResult;
        return result;
    }

    ///Calculate for float case
    const int debug = 0;
    if(resultDtype == 'f')
    {
        float floatResult = 0;

        ///Convert to float array
        float floatArr[ctr];
        for(i = 0; i < ctr; i++)
        {
            if(dtypes[i] == 'f' || dtypes[i] == 'i')
            {
                ///Reset Buffer
                char floatTempStr[16] = {0};
                ///Copy to Buffer
                for(j = 0; j < 16; j++)
                {
                    floatTempStr[j] = parts[i*16+j];
                    if(floatTempStr[j] == 0)
                        break;
                }
                ///Copy float-string to integer array
                floatArr[i] = getFloatFromStr(floatTempStr);
                dtypes[i] = hasDecimals(floatArr[i]) ? 'f' : 'i';
            }
        }

        if(debug)
        {
            printf("After conversion:\n");
            for(i = 0; i < ctr; i++)
            {
                if(dtypes[i] == 'f')
                    printf("[%d] = %f\n", i, floatArr[i]);
                if(dtypes[i] == 'i')
                    printf("[%d] = %d\n", i, (int) floatArr[i]);
                if(dtypes[i] != 'f' && dtypes[i] != 'i')
                    printf("[%d] = %c\n", i, dtypes[i]);
            }
        }

        ///Calculate ^
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {

                if(dtypes[i] == 'i' && dtypes[i+1] == '^' && dtypes[i+2] == 'i')
                {
                    ///Set
                    floatArr[i] = (float) power((int)floatArr[i], (int)floatArr[i+2]);
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        floatArr[j] = floatArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    dtypes[i] = 'i';
                    ctr -= 2;
                    changed = 1;
                    continue;
                }
                ///If f^f, then the exponent's decimal numbers will be cut per typecast to int
                if(charInSet(dtypes[i], "if") && dtypes[i+1] == '^' && charInSet(dtypes[i+2], "if"))
                {
                    ///Set
                    floatArr[i] = floatPower(floatArr[i], (int)floatArr[i+2]);
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        floatArr[j] = floatArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    dtypes[i] = hasDecimals(floatArr[i]) ? 'f' : 'i';
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        if(debug)
        {
            printf("After ^:\n");
            for(i = 0; i < ctr; i++)
            {
                if(dtypes[i] == 'f')
                    printf("[%d] = %f\n", i, floatArr[i]);
                if(dtypes[i] == 'i')
                    printf("[%d] = %d\n", i, (int) floatArr[i]);
                if(dtypes[i] != 'f' && dtypes[i] != 'i')
                    printf("[%d] = %c\n", i, dtypes[i]);
            }
        }

        ///Calculate */%
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {
                if(dtypes[i+1] == '%')
                    if(dtypes[i] == 'f' || dtypes[i+2] == 'f')
                    {
                        printf("Error -> calculateSimple, Can't use %% operator with float numbers\n");
                        result.type = 'i';
                        result.i = 0;
                        return result;
                    }
                    else
                    {
                        ///Set
                        floatArr[i] = (int) floatArr[i] % (int) floatArr[i+2];
                        ///Shift
                        for(j = i+1; j < ctr-2; j++)
                        {
                            floatArr[j] = floatArr[j+2];
                            dtypes[j] = dtypes[j+2];
                        }
                        dtypes[i] = 'i';
                        ctr -= 2;
                        changed = 1;
                        continue;
                    }

                if(charInSet(dtypes[i], "if") && charInSet(dtypes[i+1], "/*") && charInSet(dtypes[i+2], "if"))
                {
                    ///Set
                    if(dtypes[i+1] == '*') floatArr[i] = floatArr[i] * floatArr[i+2];
                    if(dtypes[i+1] == '/') floatArr[i] = floatArr[i] / floatArr[i+2];
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        floatArr[j] = floatArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    dtypes[i] = hasDecimals(floatArr[i]) ? 'f' : 'i';
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        if(debug)
        {
            printf("After */%:\n");
            for(i = 0; i < ctr; i++)
            {
                if(dtypes[i] == 'f')
                    printf("[%d] = %f\n", i, floatArr[i]);
                if(dtypes[i] == 'i')
                    printf("[%d] = %d\n", i, (int) floatArr[i]);
                if(dtypes[i] != 'f' && dtypes[i] != 'i')
                    printf("[%d] = %c\n", i, dtypes[i]);
            }
        }

        ///Calculate +-
        do
        {
            changed = 0;
            for(i = 0; i < ctr-2; i++)
            {

                if(charInSet(dtypes[i], "if") && charInSet(dtypes[i+1], "+-") && charInSet(dtypes[i+2], "if"))
                {
                    ///Set
                    if(dtypes[i+1] == '+') floatArr[i] = floatArr[i] + floatArr[i+2];
                    if(dtypes[i+1] == '-') floatArr[i] = floatArr[i] - floatArr[i+2];
                    ///Shift
                    for(j = i+1; j < ctr-2; j++)
                    {
                        floatArr[j] = floatArr[j+2];
                        dtypes[j] = dtypes[j+2];
                    }
                    dtypes[i] = hasDecimals(floatArr[i]) ? 'f' : 'i';
                    ctr -= 2;
                    changed = 1;
                }
            }
        }
        while(changed == 1);

        if(debug)
        {
            printf("After +-:\n");
            for(i = 0; i < ctr; i++)
            {
                if(dtypes[i] == 'f')
                    printf("[%d] = %f\n", i, floatArr[i]);
                if(dtypes[i] == 'i')
                    printf("[%d] = %d\n", i, (int) floatArr[i]);
                if(dtypes[i] != 'f' && dtypes[i] != 'i')
                    printf("[%d] = %c\n", i, dtypes[i]);
            }
            printf("\n");
        }


        ///Check for errors
        if(ctr != 1)
        {
            printf("Error -> calculateSimple, Error while calculating result\n");
            result.type = 'i';
            result.i = 0;
            return result;
        }
        else
            floatResult = floatArr[0];

        if(hasDecimals(floatResult))
        {
            result.type = 'f';
            result.f = floatResult;
        }
        else
        {
            result.type = 'i';
            result.i = (int) floatResult;
        }

        return result;
    }

    printf("Error -> Unknown Error\n");
    result.type = 'i';
    result.i = 0;
    return result;
}


int main(void)
{
    char input[] = "8.5 ^ 2 + 3.1415 + 12";
    
    removeSpaces(input);
    var_t result = calculateSimple(input);
    var_print(&result);
}
