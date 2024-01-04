https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"

// Values we want to persist
static FILE* fp;
static char* tokenString;
static int token;

int scanner_open(char* filename) {
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error: File %s couldn't be opened!\n", filename);
    }
    tokenString = NULL;
    return nextToken();
}

int scanner_close() {
    fclose(fp);
    free(tokenString);
}

int currentToken() {
    return token;
}

int nextToken() {
    // Clear the previous token, placeholder malloc to realloc later
    free(tokenString);
	tokenString = malloc(0);

    // Find the start of the next token
    int firstChar = ' ';
    while (firstChar != EOF && isspace(firstChar)) {
        firstChar = fgetc(fp);
    }

    // Figure out what kind of token we have
    if (firstChar == EOF) {
        token = EOS;
    } else if (!isalnum(firstChar)) {
        // At the start of a symbol
        char symbol = (char) firstChar;
        switch(symbol) {
            case '+' : token = ADD; break;
            case '-' : token = SUBTRACT; break;
            case '*' : token = MULTIPLY; break;
            case '/' : token = DIVIDE; break;
            case '=' : token = EQUAL; break;
            case '<' : token = LESS; break;
            case ':' : 
                token = COLON;
                firstChar = fgetc(fp);
                if (firstChar == '=') {
                    token = ASSIGN;
                } else {
                    ungetc(firstChar, fp);
                }
                break;
            case ';' : token = SEMICOLON; break;
            case '.' : token = PERIOD; break;
            case ',' : token = COMMA; break;
            case '(' : token = LPAREN; break;
            case ')' : token = RPAREN; break;
            case '[' : token = LBRACE; break;
            case ']' : token = RBRACE; break;
        }
    } else if (isdigit(firstChar)) {
        // At the start of a constant
        token = CONST;
        int size = 0;
        while (isdigit(firstChar)) {
            size++;
            tokenString = realloc(tokenString, sizeof(char)*size);
            tokenString[size-1] = (char) firstChar;
            firstChar = fgetc(fp);
        }
        ungetc(firstChar, fp);
        // Don't forget null terminator!
        size++;
        tokenString = realloc(tokenString, sizeof(char)*size);
        tokenString[size-1] = '\0';
        int value = atoi(tokenString);
    } else if (isalpha(firstChar)) {
        // At the start of a keyword or identifier
        token = ID;
        int size = 0;
        while (isalnum(firstChar)) {
            size++;
            tokenString = realloc(tokenString, sizeof(char)*size);
            tokenString[size-1] = firstChar;
            firstChar = fgetc(fp);
        }
        ungetc(firstChar, fp);
        // Don't forget null terminator!
        size++;
        tokenString = realloc(tokenString, sizeof(char)*size);
        tokenString[size-1] = '\0';
        // Check if a keyword
        if (strcmp(tokenString, "and") == 0) {
            token = AND;
        } else if (strcmp(tokenString, "begin") == 0) {
            token = BEGIN;
        } else if (strcmp(tokenString, "do") == 0) {
            token = DO;
        } else if (strcmp(tokenString, "else") == 0) {
            token = ELSE;
        } else if (strcmp(tokenString, "end") == 0) {
            token = END;
        } else if (strcmp(tokenString, "if") == 0) {
            token = IF;
        } else if (strcmp(tokenString, "in") == 0) {
            token = IN;
		} else if (strcmp(tokenString, "integer") == 0) {
            token = INTEGER;
        } else if (strcmp(tokenString, "is") == 0) {
            token = IS;
        } else if (strcmp(tokenString, "new") == 0) {
            token = NEW;
        } else if (strcmp(tokenString, "not") == 0) {
            token = NOT;
        } else if (strcmp(tokenString, "or") == 0) {
            token = OR;
        } else if (strcmp(tokenString, "out") == 0) {
            token = OUT;
        } else if (strcmp(tokenString, "procedure") == 0) {
            token = PROCEDURE;
        } else if (strcmp(tokenString, "record") == 0) {
            token = RECORD;
        } else if (strcmp(tokenString, "then") == 0) {
            token = THEN;
        } else if (strcmp(tokenString, "while") == 0) {
            token = WHILE;
        }
    }

    return token;
}

void getId(char* identifier) {
    strcpy(identifier, tokenString);
}

int getConst() {
    return atoi(tokenString);
}