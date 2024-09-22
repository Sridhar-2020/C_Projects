#ifndef LEXER_H
#define LEXER_H

/* Predefined libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Macros*/
#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100
#define SUCCESS 1
#define FAILURE 0

/* Function to check whether the passed file exists or not*/
int initializeLexer(const char* filename);

/* Function to check the string is keyword or not*/
int isKeyword(const char* str);

/* Function to check the string is keyword or not*/
int isOperator(char ch);

/* Function to check the string is special character or not*/
int isSpecialCharacter(char ch);

/* Function to check the string is constant or not*/
int isConstant(const char* str);

/* Function to check the string is Identifier or not*/
int isIdentifier(const char* str);

/* Function to check the string is double_operator or not*/
int is_double_opt(char str[]);

/* Function to extract all the tokens and categorise them*/
void initiate_lexer();

#endif
