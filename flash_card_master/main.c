/*******************************************************************************
** Project: Chinese Flash Cards
** Current Version: 0.1.2.11
*******************************************************************************/

#include <stdio.h> // printf and file
#include <stdlib.h> // exit
#include <string.h> // strcpy
#include <time.h> // rand

#define FLASH_ERROR_OPEN_FILE -1000
#define FLASH_EMPTY_LINE_IN_FILE -1100
#define FLASH_NO_SEPARATOR_ON_LINE -1200
#define FLASH_MULTIPLE_SEPARATORS_ON_LINE -1300

int inputArgumentsCheck(int numberOfArguments, char *version);
int fileScan(char *fileName, int *maxLine, int *lineNumber);
int check(int rc);
int fillLanguageStrings(char *fileName, char **lang0, char **lang1, int lineNumber);
int shuffle(int *array, int length);
int promptUser(char *promptString, int *userInput, int min, int max);

int main(int argc, char *argv[])
{
    char versionString[20];
    strcpy(versionString, "0.1.2.11");
    inputArgumentsCheck(argc, versionString);

    int currentMaxCard = 0; // max characters per card of current file
    int maxCard = 0; // max characters per card of all scanned files
    int currentNumOfCards = 0; // number of cards of current file
    int numOfCards = 0; // number of flash cards for all scanned files

    for (int i=1;i<argc;i++)
    {
        check(fileScan(argv[i], &currentMaxCard, &currentNumOfCards));
        if (currentMaxCard > maxCard)
            maxCard = currentMaxCard;
        numOfCards += currentNumOfCards;
    }
    maxCard++; // needs one for the null character

    // memory allocation
    char **cantonese;
    char **english;

    cantonese = (char**) malloc(sizeof(char*) * numOfCards);

    for (int i=0; i<numOfCards; i++)
        cantonese[i] = (char*) malloc(maxCard);

    english = (char**) malloc(sizeof(char*) * numOfCards);

    for (int i=0; i<numOfCards; i++)
        english[i] = (char*) malloc(maxCard);

    int *randomCard = (int*) malloc(sizeof(int) * numOfCards); // array for shuffled cards
    // end memory allocation

    for (int i=0; i<numOfCards; i++) // fill randomCard array with unshuffled deck
        randomCard[i] = i; // fixed in 0.0.2.9 (used to be = i + 1)

    int lineNumber = 0; // current line number

    for (int i=1; i<(argc); i++)
            lineNumber = check(fillLanguageStrings(argv[i], cantonese, english, lineNumber));

    // Gather User input at beginning
    int input; // integer input from user
    char promptString[1024]; // Prompt string for user input
    strcpy(promptString,"Which do you want to work on?\n\t1. Computer gives Cantonese\n\t2. Computer gives English\nType 1 or 2 and hit enter\n");
    promptUser(promptString, &input, 1, 2);

    // how many times through to go through cards
    int cycles; // number of times to go through the cards
    sprintf(promptString,"There are %d number of cards.  How many cycles through all the cards to practice?\n\tA cycle will go though and test on all %d cards in a random order\n", numOfCards, numOfCards);
    promptUser(promptString, &cycles, 1, 100);

    while(getchar()!='\n'); // clean stdin

    if (input == 1)
    {
        for (int i=0; i<cycles; i++)
        {
            check(shuffle(randomCard, numOfCards));
            for (int i=0;i<numOfCards;i++)
            {
                printf("%s", cantonese[randomCard[i]]);
                while(getchar()!='\n'); // clean stdin
                printf("%s\n\n", english[randomCard[i]]);
            }
        }
    }
    else
    {
        for (int i=0; i<cycles; i++)
        {
            check(shuffle(randomCard, numOfCards));
            for (int i=0;i<numOfCards;i++)
            {
                printf("%s", english[randomCard[i]]);
                while(getchar()!='\n'); // clean stdin
                printf("%s\n\n", cantonese[randomCard[i]]);
            }
        }
    }

    for (int i=0; i<numOfCards; i++)
    {
        free(cantonese[i]);
        free(english[i]);
    }

    free(cantonese);
    free(english);
    free(randomCard);

    printf("Done\n");

    return 0;
}

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
        return FLASH_ERROR_OPEN_FILE;

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
                return FLASH_EMPTY_LINE_IN_FILE;
            else if (pipeRead == 0)
                return FLASH_NO_SEPARATOR_ON_LINE;
            else
                newLineFlag = 1;
        }
        else if (c == '|')
        {
            if (numberOfChars > *maxLine)
                *maxLine = numberOfChars;
            numberOfChars = 0;
            if (pipeRead == 1)
                return FLASH_MULTIPLE_SEPARATORS_ON_LINE;
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
            return FLASH_MULTIPLE_SEPARATORS_ON_LINE;
        else if ((c != '|') && (pipeRead == 0))
            return FLASH_NO_SEPARATOR_ON_LINE;
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
        char message[1024];
        switch(rc)
        {
            case FLASH_ERROR_OPEN_FILE :
                strcpy(message,"Error opening file");
                break;
            case FLASH_EMPTY_LINE_IN_FILE :
                strcpy(message,"Empty Line in file");
                break;
            case FLASH_NO_SEPARATOR_ON_LINE :
                strcpy(message,"No '|' separator on line");
                break;
            case FLASH_MULTIPLE_SEPARATORS_ON_LINE :
                strcpy(message,"Multiple '|' separators on line");
                break;
            default :
                strcpy(message,"Unknown error");
        }
        printf("Error: %s\n", message);
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
        return FLASH_ERROR_OPEN_FILE;

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

/* Function takes pointer to array of card order and number of cards and then
shuffles the card order
Returns 0 */
int shuffle(int *array, int length)
{
    int randomCard; // random card
    int tempHolder; // holder for current card to replace shuffled card

    srand(time(0));  // seed it

    for(int i=0;i<length;i++) // fixed in 0.0.2.9
    {
        randomCard = rand() % length;
        tempHolder = array[i];
        array[i] = array[randomCard];
        array[randomCard] = tempHolder;
    }

    return 0;
}

/* Function takes a string, pointer to an int, and valid range.  Prints string
as a prompt to user, then takes user input and checks it against range.
Doesn't return until user inputs proper value
Returs 0 */
int promptUser(char *promptString, int *userInput, int min, int max)
{
    printf("%s", promptString);
    scanf("%d", userInput);
    while ((*userInput < min) || (*userInput > max))
    {
        printf("Invalid entry.  ");
        if (*userInput < min)
            printf("Input must be greater than %d\n", min-1);
        else
            printf("Input must be less than %d\n", max+1);

        printf("Re-enter value: ");
        scanf("%d", userInput);
    }

    return 0;
}
