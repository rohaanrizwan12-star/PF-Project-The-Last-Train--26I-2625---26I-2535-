#include "util.h"
#include <iostream>
// Primitive helper utilities, clamping, and screen control
using namespace std;


int clampInt(int val, int minVal, int maxVal)
{
    if (val < minVal)
        return minVal;

    else if (val > maxVal)
        return maxVal;
    
    else
        return val;
}

void trimWhitespace(char* str)
{
    int lenstr=0;
    while(str[lenstr]!='\0')
    {
        lenstr++;
    }
    int start=0;
    int last= lenstr-1;

    while(start<lenstr && (str[start]==' ' || str[start]=='\t' || str[start]=='\n'))
        start++;
    
    while(start<last && (str[last]==' ' || str[last]=='\t' || str[last]=='\n'))
        last--;

    int white=0;
    for(; start<=last; ++white)
    {
        str[white]= str[start];
        ++start;
    }
    str[white]='\0';
}

bool stringEquals(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return false;
        a++;
        b++;
    }
    return *a == *b;
}

void printHeader(const char* title)
{
    cout << "============";
    cout << title;
    cout << "============" << endl;
}

void clearScreen()
{
    // Clear the console screen using ANSI escape codes
    cout << "\033[2J\033[H"<< flush;
}

