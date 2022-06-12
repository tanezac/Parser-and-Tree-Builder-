// Project 2 - Parser
// scanner.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include"token.h"
#include"scanner.h"

//list of differnt states
typedef enum {
	ERRORFN = -1,
	s1 = 0, s2 = 1, s3 = 2, s4 = 3, s5 = 4, s6 = 5, s7 = 6, s8 = 7, s9 = 8,
	s10 = 9, s11 = 10, s12 = 11, s13 = 12, s14 = 13, s15 = 14, s16 = 15, s17 = 16, s18 = 17,
	KEYFN = 1011, COMFN = 1012, ERROR = -2
} state_fn;

//FSA TABLE
int FSATable[18][15] = {
// a-z   	A-Z	0-9	*	&	+	/	%	.	{	}	<	-	EOF	WS    
//----------------------------------------------------------------------------------------------------------------
{s2,   	s4,	s5,	s6,	s8,	s9,	s10,	s11,	s12,	s13,	s14,	s15,	s18,	-1,	s1},	//s1
  {s3,	 	s3,	s3,	s2,	s2,	s2,	s2,	s2,	s2,	s2,	s2,	s2, 	s2,	s2,	s2},	//s2
  {s3,	 	s3,	s3,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000,	1000},	//s3
  {s4,	 	1011,	1011,	1011, 	1011, 	1011, 	1011,	1011,	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011},	//s4
  {1001,	1001,	s5,	1001,	1001,	1001,	1001,	1001,	1001, 	1001,	1001,	1001,	1001,	1001,	1001},	//s5
  {s6,	 	s6,	s6,	s7,	s6,	s6,	s6,	s6,	s6,	s6,	s6,	s6,	s6,	s6,	s6},	//s6
  {COMFN,	COMFN,	COMFN,	COMFN,	COMFN,	COMFN,	COMFN, 	COMFN,	COMFN,	COMFN,	COMFN,	COMFN,	COMFN,	COMFN, 	COMFN},	//s7
  {1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002, 	1002,	1002,	1002,	1002,	1002,	1002},	//s8
  {1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003, 	1003,	1003},	//s9
  {1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004},	//s10
  {1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005},	//s11
  {1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006},	//s12
  {1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007},	//s13
  {1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008},	//s14
  {s15,		s15,	s15,	s15,	s15,	s15,	s15,	s15,	s15,	s15,	s15,	s16,	s17,	s15,	s15},	//s15
  {1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009},	//s16
  {1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010},	//s17
  { ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	ERROR}	//s18
};


// Final state to token struct mapping array

state_m finalStateLookup[] = {
	{-1, EOF_TK}, 
	{-2, ERROR_TK},
	{1000, ID_TK}, 
	{1001, DIGIT_TK}, 
	{1002, AND_TK},
	{1003, ADD_TK},
	{1004, DIVISION_TK},
	{1005, MODULUS_TK},
	{1006, DOTS_TK},
	{1007, BRACKET_RIGHT_TK},
	{1008, BRACKET_LEFT_TK},
	{1009, LEFT_SHIFT_TK},
	{1010, SMALLER_MINUS_TK},
	{1012, COMMENT_TK}
};


// Table used to lookup keywords 
token keywordLookupTable[] = {
	{AGAIN_TK, "Again", 0}, 
	{IF_TK, "If", 0}, 
	{ASSIGN_TK, "Assign", 0},
	{MOVE_TK, "Move", 0}, 
	{SHOW_TK, "Show", 0},
	{FLIP_TK, "Flip", 0}, 
	{NAME_TK, "Name", 0}, 
	{HOME_TK, "Home", 0},
	{DO_TK, "Do", 0},
	{SPOT_TK, "Spot", 0},
	{PLACE_TK, "Place", 0},
	{HERE_TK, "Here", 0},
	{THERE_TK, "There", 0}  
};


//scanner function
token scanner(FILE *infile, int *lineNum) {

	token returnTk;

	// FSA state implementation 
	int currentState = 0;
	int nextState = 0;

	// Return token values to defaults 
	returnTk.type = 999;
	returnTk.instance = NULL;
	returnTk.lineNum = 999;

	char currentChar;
	int FSAColumn;
	char *currentWord = malloc(100);
	char *currentCharStr = malloc(20);
	

	
	while (currentState < 100 && currentState > -1) {
		currentChar = fgetc(infile);

		//skip all comment
		if (currentChar == '*') {
			while(currentChar != '*') {
				currentChar = fgetc(infile);
			} 
			
		}
			
		FSAColumn = getTableColNum(currentChar);
		sprintf(currentCharStr, "%c", currentChar);
		nextState = FSATable[currentState][FSAColumn];

		//ERROR - INLAVID CHARACTER
		if (FSAColumn == -2) {
			returnTk.type = ERROR_TK;
			returnTk.lineNum = *lineNum;
			strcpy(currentWord, currentCharStr);
			printf("\nScanner Error: Invalid Character: %c.\n", currentChar);
			returnTk.instance = currentWord;
			return returnTk;
		}
		
		if (nextState > 999 || nextState == -1 || nextState == -2 ) {

			// EOF Token
			if (nextState == -1) {
				returnTk.type = EOF_TK;
				returnTk.lineNum = *lineNum;
				returnTk.instance = "EOF";
				return returnTk;
			}
			// token << and <-
			else if (nextState == 1009 || nextState == 1010 ) {
				sprintf(currentCharStr, "%c", currentChar);
				strcat(currentWord, currentCharStr);
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNum = *lineNum;
				return returnTk;
			}
			// Nomarl final states
			else if (nextState > 999) {			
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNum = *lineNum;
				ungetc(currentChar, infile);
				return returnTk;
			}
			// ERROR -2
			else {
				ungetc(currentChar, infile);
				int temp = fgetc(infile);
				sprintf(currentCharStr, "%c", temp);
				strcat(currentWord, currentCharStr);
				printf("\nScanner Error: Line: %i. Character: %c.No token begins with - \n", *lineNum, currentChar);
				returnTk.type = ERROR_TK;
				returnTk.lineNum = *lineNum;
				returnTk.instance = currentWord;
				return returnTk;
			}
		}
	
		else{
			if(!isspace(currentChar)){
			strcat(currentWord, currentCharStr);
			}
			
			if (strlen(currentWord) > 100){
				printf("Scanner Error: Identifier/Number Length Exception %s > 100\n", currentWord);
				returnTk.type = ERROR_TK;
				returnTk.lineNum = *lineNum;
				returnTk.instance = "Length";
				return returnTk;
			}
		
			if (currentChar == '\n') {
				(*lineNum)++;
			}
		
			currentState = nextState;
		}
	}
	
	returnTk.type = ERROR_TK;
	returnTk.lineNum = *lineNum;
	returnTk.instance = "Scanner Error: Scanner Failure";
	return returnTk;
}



// Function to find the CurrentWord from tokens 
token lookup(int state, char *searchString) {
	token returnTk;
	int i;
	for (i = 0; i < TOKENS; i++) {
		if (finalStateLookup[i].state == state) {
			returnTk.type = finalStateLookup[i].tkEnum;
			returnTk.instance = searchString;
			returnTk.lineNum = 999; 
		}
	}
	
	if (state == 1011) {
		for (i = 0; i < KEYWORDS; i++) {
			if (strcmp(keywordLookupTable[i].instance, searchString) == 0) {
				returnTk.type = keywordLookupTable[i].type;
				returnTk.instance = keywordLookupTable[i].instance;
				returnTk.lineNum = 999; 
			}
		}
	}
	return returnTk;
}
//check the character then return nextChar Index
int getTableColNum(char x) {

	if (islower(x)) {
		return LOWER;
	}
	if (isupper(x)) {
		return UPPER;
	}
	if (isdigit(x)) {
		return DIGIT;
	}
	if (isspace(x)) {
		return WHITESPACE;
	}
	if (x == '&') {
		return AND;
	}
	if (x == '+') {
		return ADD;
	}
	if (x == '/') {
		return DIVISION;
	}
	if (x == '%'){
		return MODULUS;
	}
	if (x == '.') {
		return DOT;
	}
	if (x == '{') {
		return BRACKET_LEFT;
	}
	if (x == '}') {
		return BRACKET_RIGHT;
	}
	if (x == '*') {
		return STAR;
	}
	if (x == '<') {
		return SMALLER;
	}
	if (x == '-') {
		return MINUS;
	}
	if (x == EOF){
		return ENDOFFILE;
	}
	return (-2);
}

