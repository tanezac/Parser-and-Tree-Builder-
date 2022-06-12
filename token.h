// Project 1 - Parser
// token.h
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022
//

#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
	EOF_TK, ERROR_TK, ID_TK, DIGIT_TK, COMMENT_TK,				// identifier, interger	

	AGAIN_TK, IF_TK, ASSIGN_TK, MOVE_TK, SHOW_TK, FLIP_TK, 
	NAME_TK, HOME_TK, DO_TK, SPOT_TK,PLACE_TK, HERE_TK, THERE_TK,			// keyword

	AND_TK, ADD_TK, DIVISION_TK, MODULUS_TK, DOTS_TK, BRACKET_RIGHT_TK, 
	BRACKET_LEFT_TK, LEFT_SHIFT_TK, SMALLER_MINUS_TK				// operators and delimiters
} tokenType;

typedef struct {
	tokenType type;
	char *instance; 
	int lineNum;
} token;


#endif
