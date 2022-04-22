/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Gao Zhang
Student ID: 201581878
Email: el21gz@leeds.ac.uk
Date Work Commenced: 17.2.2022
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE
FILE *ioFile;

// File name for self text
/*char* fileName = "Ball.jack";
const char* fileSource = "E:\\Compiler Design and Construction\\Lexer_Program\\SelfGrader\\";*/

// File name for final text
char fileName[1000];
int line = 1;

const char* eof = "End of File";
const char* eofErr = "Error: unexpected eof in comment";
const char* lineErr = "Error: new line in string constant";
const char* uneErr = "Error: unexpected eof in string constant";
const char* illErr = "Error: illegal symbol in source file";
/*const char* failLog = "Fail to open the file!\n";
const char* sucLog = "Success to open file\n";*/

// symbol
static bool isSYMBOL(int type) {
    const char *SYMBOL = "+-*/><=(){}[],.;~|&";

    for (int i = 0; i < strlen(SYMBOL); i++) {
        if (type == SYMBOL[i])
            return true;
    }
    return false;
}

// resword
static bool isRESWORD(char *string) {
    const char *const RESWORD[] = {"boolean","char","constructor","var","void",
                                   "do","else","false","true","while","if",
                                   "int","null","return","static","this",
                                   "method","field","function","class", "let" };

    for (int i = 0; i < sizeof(RESWORD) / sizeof(char *); i++) {
        if (!strcmp(RESWORD[i], string))
            return true;
    }
    return false;
}

// number
static bool isINT(int type) {
    if (type >= '0' && type <= '9') {
        return true;
    }
    else
        return false;
}

// letter
static bool isLetter(int type) {
    if (type >= 'a' && type <= 'z' || type >= 'A' && type <= 'Z') {
        return true;
    }
    else
        return false;
}

char *typePrint(TokenType tokenType) {
    switch (tokenType) {
        case RESWORD:
            return "RESWORD";
        case INT:
            return "INT";
        case ID:
            return "ID";
        case SYMBOL:
            return "SYMBOL";
        case STRING:
            return "STRING";
        case EOFile:
            return "EOFile";
        case ERR:
            return "ERR";
        default:
            return "Unknown";
    }
}

void skipWhiteSpace(int *type, int *line) {
    do {
        *type = fgetc(ioFile);
        if (*type == '\n') {
            *line = *line + 1;
        }
    }
    while ((*type == '\n') || (*type == '\t') || (*type == '\r') || (*type == ' ') || (*type == '\v') || (*type == '\f'));
}

int moveToNextLine(int *type, FILE *file, int *line) {
    if (file == NULL) {
        return -1;
    }
    while (1) {
        *type = fgetc(file);
        if (*type == EOF) {
            return EOF;
        } else if (*type == '\n') {
            *line = *line + 1;
            break;
        }
    }
    return 0;
}

void skipComment(int *type, int *line, int *backFlag, Token *token) {
    *type = fgetc(ioFile);
    if (*type == '/') {
        moveToNextLine(type, ioFile, line);
        skipWhiteSpace(type, line);
    } else if (*type == '*') {
        do {
            *type = fgetc(ioFile);
            if (*type == EOF) {
                token->tp = ERR;
                token->ec = 0;
                strcpy(token->lx, eofErr);
                break;
            } else if (*type == '\r') {
                *line = *line + 1;
            } else if (*type == '*') {
                *type = fgetc(ioFile);
                if (*type == '/') {
                    break;
                } else {
                    fseek(ioFile, -1, SEEK_CUR);
                }
            }
        } while (1);
        skipWhiteSpace(type, line);
    } else {
        *backFlag = 1;
    }
}


// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer(char *file_name) {
    // The file name (path) for self test
    /*file_name = (char *)malloc(strlen(fileSource) + strlen(fileName));
    strcpy(file_name, fileSource);
    strcat(file_name, fileName);*/

    // The file path for final test
    strcpy(fileName, file_name);
    ioFile = fopen(file_name, "r");
    line = 1;
    if (ioFile == NULL) {
        printf("Fail to open the file!\n");
        return 0;
    } else {
        printf("Success to open file\n");
        return 1;
    }
}

// Get the next token from the source file
Token GetNextToken() {
    int type;
    // static int line = 1;
    Token token;
    memset(token.lx, '\0', 128 * sizeof(char));
    strcpy(token.fl, fileName);

    token.ec = -1;
    skipWhiteSpace(&type, &line);
    // The location of type go back
    int backFlag = 0;
    while (type == '/') {
        skipComment(&type, &line, &backFlag, &token);
    }

    if (backFlag == 1) {
        fseek(ioFile, -2, SEEK_CUR);
        type = fgetc(ioFile);
    }
    token.ln = line;

    if (type == EOF && token.ec == -1) {
        token.tp = EOFile;
        strcpy(token.lx, eof);
        return token;
    }

    if (type == '"' && token.ec == -1) {
        int i = 0;
        int errFlag = 0;
        while ((type = fgetc(ioFile)) != '"') {
            if (type == '\n') {
                token.ec = 1;
                errFlag = 1;
                break;
            } else if (type == EOF) {
                token.ec = 2;
                errFlag = 2;
                break;
            }
            token.lx[i] = type;
            i++;
        }
        if (errFlag == 1) {
            strcpy(token.lx, lineErr);
            token.tp = ERR;
        } else if (errFlag == 2) {
            strcpy(token.lx, uneErr);
            token.tp = ERR;
        } else
            token.tp = STRING;
        return token;
    }

    // Number
    if (isdigit(type) && token.ec == -1) {
        token.lx[0] = type;
        int i = 1;
        while (1) {
            type = fgetc(ioFile);
            if (!isINT(type)) {
                if (type != EOF) {
                    fseek(ioFile, -1, SEEK_CUR);
                }
                break;
            }
            token.lx[i] = type;
            i++;
        }
        token.tp = INT;
        return token;
    }

    // Resword  ID
    if ((isLetter(type) || (type == '_')) && (token.ec == -1)) {
        token.lx[0] = type;
        int i = 1;
        while (1) {
            type = fgetc(ioFile);
            if (!(isLetter(type) || isINT(type) || (type == '_'))) {
                if (type != EOF) {
                    fseek(ioFile, -1, SEEK_CUR);
                }
                break;
            }
            token.lx[i] = type;
            i++;
        }
        if (isRESWORD(token.lx)) {
            token.tp = RESWORD;
        } else {
            token.tp = ID;
        }
        return token;
    }

    // Symbol
    if (token.ec == -1) {
        if (isSYMBOL(type)) {
            token.lx[0] = type;
            token.tp = SYMBOL;
            return token;
        } else {
            token.ec = 3;
            strcpy(token.lx, illErr);
            token.tp = ERR;
            return token;
        }
    }
    else {
        return token;
    }
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken() {
    Token token;
    int len;
    token = GetNextToken();
    len = (int)strlen(token.lx);
    if(token.tp == STRING){
        fseek(ioFile, -1 * len - 2, SEEK_CUR);
    } else if (token.tp == EOFile){
        fseek(ioFile, -1, SEEK_CUR);
    } else {
        fseek(ioFile, -1 * len, SEEK_CUR);
    }
    return token;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer() {
    fclose(ioFile);
    return 0;
}

// do not remove the next line
#ifndef TEST

/*int main(int argc, char *argv[]) {
    // implement your main function here
    // NOTE: the autograder will not use your main function
    InitLexer(fileName);
    for(int i = 0; i < 10; i++){
        Token token = PeekNextToken();
        printf("< %s, %d, %s, %s >\n", token.fl, token.ln, token.lx, typePrint(token.tp));
        if (token.tp == EOFile || token.tp == ERR) {
            break;
        }
    }
    while (1) {
        Token token = GetNextToken();
        printf("< %s, %d, %s, %s >\n", token.fl, token.ln, token.lx, typePrint(token.tp));
        if (token.tp == EOFile || token.tp == ERR) {
            break;
        }
    }
    StopLexer();
    return 0;
}*/
// do not remove the next line
#endif
