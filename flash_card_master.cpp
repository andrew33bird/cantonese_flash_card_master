/*******************************************************************************
** Project: Chinese Flash Cards
** Version: 0.0.0.1
********************************************************************************
** 4/29/17 Opens file test.txt and reads each line.  First part until | is
    Cantonese, after is English.  File must end with \n.  Prints Cantonese and
    English strings.
*******************************************************************************/

#include <stdio.h> // printf and file
#include <stdlib.h> // exit
#include <string.h> // strcpy

int main(int argc, char *argv[])
{
    FILE *f;
    
    f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file");
        exit(-1000);
    }
    
    char lineCantonese[2][100];
    char lineEnglish[2][100];
    char string[100];
    int i = 0;
    int line = 0;
    char c;
    
    while (!feof(f))
    {
        c = fgetc(f);
        if ((c == '\n') || (c == '\r'))
        {
            string[i] = '\0';
            strcpy(lineEnglish[line++], string);
            i = 0;
        }
        else if (c == '|')
        {
            string[i] = '\0';
            strcpy(lineCantonese[line], string);
            i = 0;
        }
        else
        {
            string[i++] = c;
        }
    }
    
    printf("Cantonese: %s\n", lineCantonese[0]);
    printf("English: %s\n", lineEnglish[0]);
    
    printf("Cantonese: %s\n", lineCantonese[1]);
    printf("English: %s\n", lineEnglish[1]);
    
    fclose(f);
    
    return 0;
}