/*******************************************************************************
** Project: Chinese Flash Cards
** Current Version: 0.0.0.2
********************************************************************************
** Version 0.0.0.3 *************************************************************
** 5/8/17 Rev 0.0.0.3 compiles, but test.txt always throws error -1200
** 5/8/17 Creates 50 lines of 100 characters.  Throws error if exceeds 
        -1400 Error if line count > 50 or character count > 100
********************************************************************************
** Version 0.0.0.2 *************************************************************
** 5/6/17 Rev 0.0.0.2 does not compile.  Variable length array is not in C
    http://stackoverflow.com/questions/33423502/expression-did-not-evaluate-to-a-constant-c
** 5/6/17 Opens test file, counts number of lines and max characters for a 
    Cantonese or English string.  When scanning file, returns different error:
        -1000 Error if can't open file
        -1100 Error if empty line in file
        -1200 Error if no '|' on line
        -1300 Error if 2 '|' characters on one line
********************************************************************************
** Version 0.0.0.1 *************************************************************
** 4/29/17 Opens file test.txt and reads each line.  First part until | is
    Cantonese, after is English.  File must end with \n.  Prints Cantonese and
    English strings.
*******************************************************************************/

#include <stdio.h> // printf and file
#include <stdlib.h> // exit
#include <string.h> // strcpy

int main(int argc, char *argv[])
{
    FILE *f; // pointer to file
    
    // Open file.  exit with error code if error.
    f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file");
        exit(-1000);
    }
    
    int maxChar = 0; // max character for each line of Cantonese or English
    int numberChar = 0; // number of characters for current line
    int lineNumber = 0; // number of lines in files
    int newLineFlag = 1; // flag when at a new line.  Starts with new line
    int pipeRead = 0; // ensures that a | is read for each line
    char c; // temp char during upload of file
    
    while(!feof(f))
    {
        c = fgetc(f);
        if ((c == '\n') || (c == '\r'))
        {
            if (numberChar > maxChar)
                maxChar = numberChar;
            numberChar = 0;
            lineNumber++;
            if (newLineFlag == 1)
            {
                printf("Error: Empty line in file");
                exit(-1100);
            }
            else if (pipeRead = 0)
            {
                printf("Error: Must have '|' on each line to divide Cantonese and English");
                exit(-1200);
            }
            else
            {
                newLineFlag = 1;
            }
        }
        else if (c == '|')
        {
            if (numberChar > maxChar)
                maxChar = numberChar;
            numberChar = 0;
            if (pipeRead == 1)
            {
                printf("Error: 2 '|' characters on the same line");
                exit(-1300);
            }
            else
            {
                pipeRead = 0;
            }
        }
        else
        {
            numberChar++;
            if (newLineFlag == 1)
            {
                lineNumber++;
                newLineFlag = 0;
            }
        }
    }
    if (newLineFlag == 0)
    {
        if ((c == '|') && (pipeRead == 1))
        {
            printf("Error: 2 '|' characters on the same line");
            exit(-1300);
        }
        else if ((c != '|') && (pipeRead == 0))
        {
            printf("Error: Must have '|' on each line to divide Cantonese and English");
            exit(-1200);
        }
        else if (c == '|')
        {
            if (numberChar > maxChar)
                maxChar = numberChar;
            lineNumber++;
        }
        else
        {
            if (numberChar > maxChar)
                maxChar = numberChar;
            lineNumber++;
        }
    }
    fclose(f);
    
    if (lineNumber > 50)
    {
        printf("Total lines must be less than 50\n");
        exit(-1400);
    }
    else if (maxChar > 100)
    {
        printf("Highest character count must be less than 100\n");
        exit(-1400);
    }
    // Open file.  exit with error code if error.
    f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file");
        exit(-1000);
    }
    
    maxChar++; // needs one for the null character
    //char lineCantonese[lineNumber][maxChar]; // x lines of y char Cantonese
    //char lineEnglish[lineNumber][maxChar]; // x lines of y char English
    char lineCantonese[50][100];
    char lineEnglish[50][100];
    //char string[maxChar]; // temp string during upload of file
    char string[100];
    int i = 0; // counter
    int line = 0;  // line number
    
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