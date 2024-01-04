https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef SCANNER_H
#define SCANNER_H

#include "core.h"

// Opens the file, fine the first token
int scanner_open(char* filename);

// Frees memory allocated for the scanner
int scanner_close();

// Returns the current token
int currentToken();

// Advances to the next token
int nextToken();

// If the current token is ID, returns the string value
void getId(char* identifier);

// If the current token is CONST, returns the int value
int getConst();

#endif