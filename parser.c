// Project 2 -Parser
// parser.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "scanner.h"
#include "parser.h"

static FILE *inFilePtr;
static token tempToken;
int lineNum = 1;

char *tokenName[28] = {
	"END OF FILE", "ERROR", "IDENTIFIER TOKEN", "DIGIT TOKEN","COMMENT - IGNORE",
	
	"AGAIN KEYWORD", "IF KEYWORD", "ASSIGN KEYWORD", "MOVE KEYWORD", "SHOW KEYWORD", "FLIP KEYWORD", 
	"NAME KEYWORD", "HOME KEYWORD", "DOTS KEYWORD", "SPOT KEYWORD","PLACE KEYWORD", "HERE KEYWORD", "THERE KEYWORD", 

	"OPERATOR AND '&'", "OPERATOR ADD '+'", "OPERATOR DIVISION '/'", "OPERATOR MODULUS '%'", "OPERATOR DOTS '.'", "OPERATOR BRACKET LEFT '{'", 
	"OPERATOR BRACKET RIGHT '}'", "OPERATOR LEFTSHIFT '<<'", "OPERATOR SMALLER MINUS '<-'"
	
};

//Parser function
node_t *parser(FILE *input){

	inFilePtr = input;
	node_t *nodeRoot;

	tempToken = scanner(inFilePtr, &lineNum);
	nodeRoot = S();
 
	if(tempToken.type != EOF_TK){
		printf("Error: Parse Failed. Expected EOF TOKEN -- %s Returned at Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
	printf("Parser Success!\n");    // TEST	PARSER	
	return nodeRoot;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<S> -> Name Identifier Spot Identifier <R> <E>  
//
node_t *S(){

	int level = 0;
	node_t *node = createNode("<S>", level);
	//NAME
	if(tempToken.type == NAME_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//ID
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SPOT
			if(tempToken.type == SPOT_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//ID
				if(tempToken.type == ID_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					node->child1 = R(level);
					node->child2 = E(level);
					return node;
				}
				else{
					printf("Error <S>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <S>: Invalid Token, Expected SPOT TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <S>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Error <S>: Invalid Token, Expected NAME TOKEN for FIRST SET of <S>-- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<R>->Place <A> <B> Home 
//

node_t *R(int level){
	level++;
	node_t *node = createNode("<R>", level);
	//PLACE
	if(tempToken.type == PLACE_TK){
		tempToken = scanner(inFilePtr, &lineNum);
		// <A> and <B>
		node->child1 = A(level);
		node->child2 = B(level);
		
		//HOME
		if(tempToken.type == HOME_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <R>: Invalid Token, Expected HOME TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <R>: Invalid Token, Expected PLACE TOKEN for FIRST SET of <R> -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <E> -> Show Identifier 
//

node_t *E(int level){
		
	level++;
	node_t *node = createNode("<E>", level);
	//SHOW
	if(tempToken.type == SHOW_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//INDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <E>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <E>: Invalid Token, Expected SHOW TOKEN for FIRST SET of <E> -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//empty |  .  <C> . <B> | <D> <B> 
//

node_t *B(int level){
	level++;
	node_t *node = createNode("<B>", level);
	//FIRST EMPTY OR DOTS OR EQUAL FIRST <D>
	//DOTS
	if(tempToken.type == DOTS_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		node->child1 = C(level);
		//DOTS
		if(tempToken.type == DOTS_TK){
			node->token2 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			node->child2 = B(level);
			return node;
		}
		else{
			printf("Error <B>: Invalid Token, Expected DOTS TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//FIRST <D> -> <H>
	else if(tempToken.type == DIVISION_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
	
		node->child1 = Z(level);
		node->child2 = B(level);
		return node;
	}
	//FIRST <D> -> <J>
	else if(tempToken.type == ASSIGN_TK){
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);

			node->child1 = D(level);
			node->child2 = B(level);
			return node;		
		}
		else{
			printf("Error <B> -> <D> -> <L>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//FIRST <D> -> <K>
	else if(tempToken.type == SPOT_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//NUMBER
		if(tempToken.type == DIGIT_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//NUMBER
				if(tempToken.type == DIGIT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
	
					node->child1 = B(level);
					//tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <B> -> <D> -> <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <B> -> <D> -> <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <B> -> <D> -> <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else if(tempToken.type == MOVE_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//IDENTIFIER
				if(tempToken.type == ID_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);

					node->child1 = B(level);
					return node;
				}
				else{
					printf("Error <B> -> <D> -> <K>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <B> -> <D> -> <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
	}
	//FIRST <D> -> <L>
	else if(tempToken.type == FLIP_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			
			node->child1 = B(level);
			return node;
		}
		else{
			printf("Error <B> -> <D> -> <L>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	//FIRST <D> -> <E>
	else if(tempToken.type == SHOW_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			
			node->child1 = B(level);
			return node;
		}
		else{
			printf("Error <B> -> <D> -> <E>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	//FIRST <D> -> <F> { = BRACKET_RIGHT_TK
	else if(tempToken.type == BRACKET_RIGHT_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		//IF OR DO
		if(tempToken.type == IF_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//IDENTIFIER
			if(tempToken.type == ID_TK){
				node->token2 = tempToken;
				tempToken = scanner(inFilePtr, &lineNum);
				
				node->child1 = T(level);
				node->child2 = W(level);
				node->child3 = D(level);
				
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token3 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);

					node->child4 = B(level);
					return node;
				}
				else{
					printf("Error <B> -> <D> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("Error <B> -> <D> -> <F>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else if(tempToken.type == DO_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//AGAIN
			if(tempToken.type == AGAIN_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				node->child1 = D(level);
				node->child2 = T(level);
				node->child3 = W(level);
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);

					node->child4 = B(level);
					return node;
				}
				else{
					printf("Error <B> -> <D> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <B> -> <D> -> <F>: Invalid Token, Expected AGAIN TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <B> -> <D> -> <F>: Invalid Token, Expected IF TOKEN OR DO TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else {
		return node;
	}
	return node;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<C> -> <F> | <G>
//

node_t *C(int level){
	level++;
	node_t *node = createNode("<C>", level);
	//FIRST <F> { = BRACKET_RIGHT_TK
	if(tempToken.type == BRACKET_RIGHT_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		//IF
		if(tempToken.type == IF_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//IDENTIFIER
			if(tempToken.type == ID_TK){
				node->token2 = tempToken;
				tempToken = scanner(inFilePtr, &lineNum);
				
				node->child1 = T(level);
				node->child2 = W(level);
				node->child3 = D(level);
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token3 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <C> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("Error <C> -> <F>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		//DO
		else if(tempToken.type == DO_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//AGAIN
			if(tempToken.type == AGAIN_TK){
				tempToken = scanner(inFilePtr, &lineNum);

				node->child1 = D(level);
				node->child2 = T(level);
				node->child3 = W(level);
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <C> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <C> -> <F>: Invalid Token, Expected AGAIN TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <C> -> <F>: Invalid Token, Expected IF TOKEN OR DO TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//FIRST <G> Here
	else if(tempToken.type == HERE_TK){
		tempToken = scanner(inFilePtr, &lineNum);
		//NUMBER
		if(tempToken.type == DIGIT_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//THERE
			if(tempToken.type == THERE_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error <C> -> <G>: Invalid Token, Expected THERE TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <C> -> <G>: Invalid Token, NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}		
	}
	else{
		printf("Error <C>: Invalid Token, Expected BRACKET LEFT TOKEN ({) OR  HERE TOKEN for FIRST SET of <C> -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <D> -> <H> | <J> | <K> | <L>  | <E> | <F> 
//

node_t *D(int level){
		
	level++;
	node_t *node = createNode("<D>", level);
	//FIRST <H> DIVISION /
	if(tempToken.type == DIVISION_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);	
		node->child1 = Z(level);
		return node;
	}
	//FIRST <J> ASSIGN
	else if(tempToken.type == ASSIGN_TK){
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);

			node->child1 = D(level);
			return node;		
		}
		else{
			printf("Error <D> -> <J>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//FIRST <K> SPOT AND MOVE
	else if(tempToken.type == SPOT_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//NUMBER
		if(tempToken.type == DIGIT_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//NUMBER
				if(tempToken.type == DIGIT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <D> -> <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <D> -> <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <D> -> <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else if(tempToken.type == MOVE_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//IDENTIFIER
				if(tempToken.type == ID_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <D> -> <K>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <D> -> <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <D> -> <K>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//FIRST <L> FLIP
	else if(tempToken.type == FLIP_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <D> -> <L>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	//FIRST <E> SHOW
	else if(tempToken.type == SHOW_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <D> -> <E>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	//FIRST <F> { = BRACKET_RIGHT_TK
	else if(tempToken.type == BRACKET_RIGHT_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		//IF OR DO
		if(tempToken.type == IF_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//IDENTIFIER
			if(tempToken.type == ID_TK){
				node->token2 = tempToken;
				tempToken = scanner(inFilePtr, &lineNum);

				node->child1 = T(level);
				node->child2 = W(level);
				node->child3 = D(level);
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token3 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <D> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("Error <D> -> <F>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else if(tempToken.type == DO_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//AGAIN
			if(tempToken.type == AGAIN_TK){
				tempToken = scanner(inFilePtr, &lineNum);

				node->child1 = D(level);
				node->child2 = T(level);
				node->child3 = W(level);
				// } = BRACKET_LEFT_TK
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <D> -> <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <D> -> <F>: Invalid Token, Expected AGAIN TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <D> -> <F>: Invalid Token, Expected IF TOKEN OR DO TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Error: Invalid Token, Expected DIVISION (/) | ASSIGN | SPOT | MOVE | FLIP | SHOW | RIGHT BRACKET (}) for FIRST SET of <D> -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <A> -> Name Identifier 
//

node_t *A(int level){
		
	level++;
	node_t *node = createNode("<A>", level);
	//NAME
	if(tempToken.type == NAME_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <A>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <A>: Invalid Token, Expected NAME TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <F> ->{ If  Identifier <T> <W> <D> } | { Do Again <D> <T> <W> } 
//

node_t *F(int level){
	level++;
	node_t *node = createNode("<F>", level);
	//FIRST SET F {
	if(tempToken.type == BRACKET_RIGHT_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		// IF OR DO TOKEN
		if(tempToken.type == IF_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//IDENTIFIER
			if(tempToken.type == ID_TK){
				node->token2 = tempToken;
				tempToken = scanner(inFilePtr, &lineNum);

				node->child1 = T(level);
				node->child2 = W(level);
				node->child3 = D(level);
				// }
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token3 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("Error <F>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else if(tempToken.type == DO_TK){
			tempToken = scanner(inFilePtr, &lineNum);
			//AGAIN
			if(tempToken.type == AGAIN_TK){
				tempToken = scanner(inFilePtr, &lineNum);

				node->child1 = D(level);
				node->child2 = T(level);
				node->child3 = W(level);
				// }
				if(tempToken.type == BRACKET_LEFT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <F>: Invalid Token, Expected RIGHT BRACKET TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <F>: Invalid Token, Expected AGAIN TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <F>: Invalid Token, Expected IF TOKEN OR DO TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <F>: Invalid Token, Expected LEFT BRACKET TOKEN ({) -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}			
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
// <G> -> Here Number There
//

node_t *G(int level){
		
	level++;
	node_t *node = createNode("<G>", level);
	//HERE
	if(tempToken.type == HERE_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//NUMBER
		if(tempToken.type == DIGIT_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//THERE
			if(tempToken.type == THERE_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error >G>: Invalid Token, Expected THERE TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error <G>: Invalid Token, NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <G>: Invalid Token, Expected HERE TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<T> -> << | <-
//

node_t *T(int level){
		
	level++;
	node_t *node = createNode("<T>", level);
	//<<
	if(tempToken.type == LEFT_SHIFT_TK){
		node->token1 = tempToken;	
		tempToken = scanner(inFilePtr, &lineNum);
		return node;
	}
	//<-
	else if(tempToken.type == SMALLER_MINUS_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		return node;
	}
	else{
		printf("Error <T>: Invalid Token, Expected LEFT SHIFT TOKEN (<<) OR SMALLER MINUS TOKEN (<-) -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
//<V> -> +|%|&
//

node_t *V(int level){
		
	level++;
	node_t *node = createNode("<V>", level);
	//+
	if(tempToken.type == ADD_TK){
		node->token1 = tempToken;	
		tempToken = scanner(inFilePtr, &lineNum);
		return node;
	}
	//%
	else if(tempToken.type == MODULUS_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		return node;
	}
	//&
	else if(tempToken.type == AND_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		return node;
	}
	else{
		printf("Error <V>: Invalid Token, Expected ADD TOKEN OR MODULUS TOKEN OR AND TOKEN  -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<H> -> / <Z>
//	

node_t *H(int level){
		
	level++;
	node_t *node = createNode("<H>", level);
	//DIVISION
	if(tempToken.type == DIVISION_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
	
		node->child1 = Z(level);
		return node;
	}
	else{
		printf("Error <H>: Invalid Token, Expected DIVISION TOKEN (/) -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}	

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <J> -> Assign Identifier <D>
//

node_t *J(int level){

	level++;
	node_t *node = createNode("<J>", level);
	//ASSIGN
	if(tempToken.type == ASSIGN_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);

			node->child1 = D(level);
			return node;		
		}
		else{
			printf("Error <J>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Error <J>: Invalid Token, Expected ASSIGN TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<K> -> Spot Number Show Number |  Move Identifer Show Identifier 													i
//

node_t *K(int level){

	level++;
	node_t *node = createNode("<K>", level);
	//SPOT
	if(tempToken.type == SPOT_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//NUMBER
		if(tempToken.type == DIGIT_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//NUMBER
				if(tempToken.type == DIGIT_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <K>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	//MOVE
	else if(tempToken.type == MOVE_TK){									
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//SHOW
			if(tempToken.type == SHOW_TK){
				tempToken = scanner(inFilePtr, &lineNum);
				//IDENTIFIER
				if(tempToken.type == ID_TK){
					node->token2 = tempToken;
					tempToken = scanner(inFilePtr, &lineNum);
					return node;
				}
				else{
					printf("Error <K>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
					exit(EXIT_FAILURE);
				}	
			}
			else{
				printf("Error <K>: Invalid Token, Expected SHOW TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}		
		}
		else{
			printf("Error <K>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	// SPOT OR MOVE	
	else{
		printf("Error <K>: Invalid Token, Expected SPOT TOKEN OR MOVE TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <L> -> Flip Identifier 
//

node_t *L(int level){
		
	level++;
	node_t *node = createNode("<L>", level);
	//FLIP
	if(tempToken.type == FLIP_TK){	
		tempToken = scanner(inFilePtr, &lineNum);
		//IDENTIFIER
		if(tempToken.type == ID_TK){
			node->token1 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		else{
			printf("Error <L>: Invalid Token, Expected IDENTIFIER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
			exit(EXIT_FAILURE);
		}	
	}
	else{
		printf("Error <L>: Invalid Token, Expected FLIP TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// <W> -> Number <V> Number | Number .
//

node_t *W(int level){
	level++;
	node_t *node = createNode("<W>", level);
	//NUMBER
	if(tempToken.type == DIGIT_TK){	
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		//DOT OR <V>
		//IF DOT
		if(tempToken.type == DOTS_TK){
			node->token2 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			return node;
		}
		//ELSE IF <V>
		else if(tempToken.type == ADD_TK){
			node->token2 = tempToken;	
			tempToken = scanner(inFilePtr, &lineNum);
			//NUMBER
			if(tempToken.type == DIGIT_TK){
				node->token3 = tempToken;
				tempToken =scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error <W> -> <V>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
	
		else if(tempToken.type == MODULUS_TK){
			node->token2 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//NUMBER
			if(tempToken.type == DIGIT_TK){
				node->token2 = tempToken;
				tempToken =scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error <W> -> <V>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
	
		else if(tempToken.type == AND_TK){
			node->token2 = tempToken;
			tempToken = scanner(inFilePtr, &lineNum);
			//NUMBER
			if(tempToken.type == DIGIT_TK){
				node->token2 = tempToken;
				tempToken =scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error <W> -> <V>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}
	else{
		printf("Error <V>: Invalid Token, Expected ADD TOKEN OR MODULUS TOKEN OR AND TOKEN  -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
		/*
		else{
			node->child1 = V(level);
			//NUMBER
			if(tempToken.type == DIGIT_TK){
				node->token2 = tempToken;
				tempToken =scanner(inFilePtr, &lineNum);
				return node;
			}
			else{
				printf("Error <W>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
				exit(EXIT_FAILURE);
			}
		}*/	
	}
	else{
		printf("Error <W>: Invalid Token, Expected NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//<Z> -> Identifier | Number
//

node_t *Z(int level){
		
	level++;
	node_t *node = createNode("<Z>", level);
	//IDENTIFIER
	if(tempToken.type == ID_TK){
		node->token1 = tempToken;	
		tempToken = scanner(inFilePtr, &lineNum);
		//return node;
	}
	//NUMBER
	else if(tempToken.type == DIGIT_TK){
		node->token1 = tempToken;
		tempToken = scanner(inFilePtr, &lineNum);
		//return node;
	}
	else{
		printf("Error <Z>: Invalid Token, Expected IDENTIFIER TOKEN OR NUMBER TOKEN -- %s Received At Line %d\n", tokenName[tempToken.type], lineNum);
		exit(EXIT_FAILURE);
	}
	return node;
}

//----------------------------------------------------------------------------------
//CreateNode function 
node_t *createNode(char *name, int level){

	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	//not enough memory checking
	if(newNode == NULL){
		printf("ERROR: CreateNode Function\n");
		exit(EXIT_FAILURE);
	}

	strcpy(newNode->name, name);
	newNode->level = level;

	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	return newNode;
}


/*
//function call scanner, it will return 1 token at 1 time
void testScanner(FILE *input){
    	token tempToken;
	int lineNum = 1;

    	do{
        	tempToken = scanner(inputFile, &lineNum);
		if(strcmp(tokenNames[tempToken.type], "Error") == 0){
			printf("Scanner Error: %s Not Allowed At Line: %i.\n", tempToken.instance, tempToken.lineNum);
			printf("Exiting\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("TokenID: %s | Instance: %s | Line#: %i \n", tokenNames[tempToken.type], tempToken.instance, tempToken.lineNum);
		}
    	} while(!feof(inputFile));
}*/

