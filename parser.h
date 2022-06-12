// Project 2 - Parser
// parser.h
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022


#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"
#include "node.h"

//extern FILE *inFilePtr;

void testScanner(FILE *);

node_t *createNode(char *, int);	//new node function
node_t *parser(FILE *);

node_t *S();
node_t *R(int level);
node_t *E(int level);
node_t *A(int level);
node_t *B(int level);
node_t *C(int level);
node_t *D(int level);
node_t *F(int level);
node_t *G(int level);
node_t *T(int level);
node_t *V(int level);
node_t *H(int level);
node_t *J(int level);
node_t *K(int level);
node_t *L(int level);
node_t *W(int level);
node_t *Z(int level);


#endif
