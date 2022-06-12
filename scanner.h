// Project 2 - Parser
// scanner.h
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "token.h"
#include "character.h"

#define TOKENS 16
#define KEYWORDS 13


typedef struct charLookup{
	int input;
	int column;
}char_m;

typedef struct stateLookup{
	int state;
	tokenType tkEnum;
}state_m;


token scanner(FILE * , int *);	//Scanner function
token lookup(int , char *);	//Token/Keyword mapping function 
int getTableColNum(char );

#endif
