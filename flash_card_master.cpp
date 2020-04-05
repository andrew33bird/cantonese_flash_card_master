/*******************************************************************************
** Project: Chinese Flash Cards
** Current Version: 0.0.2.6
*******************************************************************************/
/* Version 0.0.1.5 *************************************************************
** 5/27/17 Program name change to flashCardMaster
    Program takes in arguments of files.  Only searches the files now.  Still
        inputs from test.txt.
    Moved the file scan to a separtate function
    Added error code check function
*******************************************************************************/
/* Version 0.0.0.4 *************************************************************
** 5/20/17 Fixed two bugs: lines 89 and 174.
    Started testing file read and error codes.
********************************************************************************
** Version 0.0.0.3 *************************************************************
** 5/8/17 Rev 0.0.0.3 compiles, but test.txt always throws error -1200
** 5/8/17 Creates 50 lines of 100 characters.  Throws error if exceeds 
        -1400 Error if line count > 50 or character count > 100
********************************************************************************
** Version 0.0.0.2 *************************************************************
** 5/6/17 Rev 0.0.0.2 does not compile.  Variable length array is not in C
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
#include <time.h>

/* Function takes number of arguments entered with exe.  If just exe, print out
version number, and usage example. */
int inputArgumentsCheck(int numberOfArguments, char *version)
{
    if (numberOfArguments == 1)
    {
        printf("Flash Card Master\n");
        printf("Version: %s\n", version);
        printf("\tEnter file names in quotations\n");
        printf("\tFiles must be in same directory as exe\n");
        printf("\tExample: flashCardMaster.exe \"file_1.txt\" \"file_2.txt\"\n");
        exit(0);
    }
    return 0;
}

/* Function takes pointer to file name and searches file for line number and
max characters per language phrase
Pointers to max characters per file and number of lines are passed
Function returns error code - 0 if no error */
int fileScan(char *fileName, int *maxLine, int *lineNumber)
{
    FILE *f; // pointer to file
    
    f = fopen(fileName, "r");
    if (f == NULL)
        return -1000;
    
    int numberOfChars = 0; // number of characters for the current line
    int newLineFlag = 1; // flag when at a new line.  Starts with new line
    int pipeRead = 0; // ensures that a | is read for each line
    char c; // temp char during upload of file
    *maxLine = 0; // max characters for each line of Cantonese or English
    *lineNumber = 0; // number of lines in file
    
    while(!feof(f))
    {
        c = fgetc(f);
        if ((c == '\n') || (c == '\r'))
        {
            if (numberOfChars > *maxLine)
                *maxLine = numberOfChars;
            numberOfChars = 0;
            *lineNumber = *lineNumber + 1;
            if (newLineFlag == 1)
                return -1100;
            else if (pipeRead = 0)
                return -1200;
            else
                newLineFlag = 1;
        }
        else if (c == '|')
        {
            if (numberOfChars > *maxLine)
                *maxLine = numberOfChars;
            numberOfChars = 0;
            if (pipeRead == 1)
                return -1300;
            else
                pipeRead = 1;
        }
        else
        {
            numberOfChars++;
            if (newLineFlag == 1)
                newLineFlag = 0;
        }
    }
    if (newLineFlag == 0)
    {
        if ((c == '|') && (pipeRead == 1))
            return -1300;
        else if ((c != '|') && (pipeRead == 0))
            return -1200;
        else if (c == '|')
        {
            if (numberOfChars > *maxLine)
                *maxLine = numberOfChars;
            *lineNumber = *lineNumber + 1;
        }
        else
        {
            if (numberOfChars > *maxLine)
                *maxLine = numberOfChars;
            *lineNumber = *lineNumber + 1;
        }
    }
    fclose(f);
    
    return 0;
}

/* Function takes return code and exits if error */
int check(int rc)
{
    if (rc < 0)
    {
        printf("Error: %d\n", rc);
        exit(rc);
    }
    return rc;
}

/* Function takes 2 pointers to array of strings and fills the contents with
English and Cantonese from the file 
Returns next line number for subsequent files */
int fillLanguageStrings(char *fileName, char **lang0, char **lang1, int lineNumber)
{
    FILE *f;
    
    f = fopen(fileName, "r");
    if (f == NULL)
        return -1000;
    
    char c;
    int i = 0;  // index of string
    int languageSelect = 0;
    
    while (!feof(f))
    {
        c = fgetc(f);
        if ((c == '\n') || (c == '\r'))
        {
            lang1[lineNumber++][i] = '\0';
            i = 0;
            languageSelect = 0;
        }
        else if (c == '|')
        {
            lang0[lineNumber][i] = '\0';
            i = 0;
            languageSelect = 1;
        }
        else if (languageSelect == 0)
            lang0[lineNumber][i++] = c;
        else
            lang1[lineNumber][i++] = c;
    }
    if (i != 0)
        lang1[lineNumber++][i] = '\0';
    
    fclose(f);
    
    return lineNumber;
}

int main(int argc, char *argv[])
{
    char versionString[20];
    strcpy(versionString, "0.0.2.6");
    inputArgumentsCheck(argc, versionString);
    
    int currentMaxLine = 0; // max characters per line of current file
    int maxLine = 0; // max characters per line of all scanned files
    int currentNumOfLines = 0; // number of lines of current file
    int numOfLines = 0; // number of lines for all scanned files
    
    for (int i=1;i<argc;i++)
    {
        check(fileScan(argv[i], &currentMaxLine, &currentNumOfLines));
        if (currentMaxLine > maxLine)
            maxLine = currentMaxLine;        
        numOfLines += currentNumOfLines;
    }
    maxLine++; // needs one for the null character

    
    char **cantonese;
    char **english;
    
    cantonese = (char**) malloc(sizeof(char*) * numOfLines);
    
    for (int i=0; i<numOfLines; i++)
        cantonese[i] = (char*) malloc(maxLine);
    
    english = (char**) malloc(sizeof(char*) * numOfLines);
    
    for (int i=0; i<numOfLines; i++)
        english[i] = (char*) malloc(maxLine);
    
    int lineNumber = 0; // current line number
    
    for (int i=1; i<(argc); i++)
            lineNumber = check(fillLanguageStrings(argv[i], cantonese, english, lineNumber));
    
    for (int i=0; i<(numOfLines-2); i++)
    {
        printf("Cantonese: %s\n", cantonese[i]);
        printf("English: %s\n", english[i]);
    }
    
    // this is the real thing
    int input;
    printf("Which do you want to work on?  Type 1 or 2 and hit enter\n");
    printf("\n1. Computer gives Cantonese\n");
    printf("2. Computer gives English\n");
    scanf("%d", &input);
    while ((input < 1) || (input > 2))
    {
        printf("\nInvalid entry.  Try again:\n");
        scanf("%d", &input);
    }
    
    srand(time(0));  // seed it
    
    int randomNumber;
    
    if (input == 1)
    {
        for (int i=0; i<20; i++)
        {
            randomNumber = rand() % numOfLines;
            printf("%s\n", cantonese[randomNumber]);
            while(getchar()!='\n'); // clean stdin
            getchar(); // wait for ENTER
            printf("%s\n", english[randomNumber]);
        }
    }
    else
    {
        for (int i=0; i<20; i++)
        {
            randomNumber = rand() % numOfLines;
            printf("%s\n", english[randomNumber]);
            while(getchar()!='\n'); // clean stdin
            getchar(); // wait for ENTER
            printf("%s\n", cantonese[randomNumber]);
        }
    }
    
    for (int i=0; i<numOfLines; i++)
    {
        free(cantonese[i]);
        free(english[i]);
    }
    
    free(cantonese);
    free(english);
    
    printf("\nDone\n");
    
    return 0;
}