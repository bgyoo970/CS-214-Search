#ifndef INDEXTOKENIZER_H
#define INDEXTOKENIZER_H
#include <stdlib.h>
#include <stdio.h>
#define MAX_HEX_CHARS 2
#define MAX_OCT_CHARS 3

typedef struct TokenizerT_ TokenizerT;
struct TokenizerT_ {
	char* copied_string;
	char* delimiters;		
	char* current_position;
};


char is_escape_character(char character);
int char_to_hex(char character);
int char_to_oct(char character);
int is_oct_digit(char oct_digit);
char* unescape_string(char* string);
TokenizerT *TKCreate(char *ts);
void TKDestroy(TokenizerT *tk);
char is_delimiter(char character, char* delimiters);
char *TKGetNextToken(TokenizerT *tk);
char *createFileStr(char *argv);

#endif
