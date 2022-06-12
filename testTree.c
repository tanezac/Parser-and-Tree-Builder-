// Project 2 - Parser
// testTree.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 6, 2022


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "testTree.h"

// Print tree in PreOder
void preOrderPrint(node_t *root) {

	// If tree is empty
	if (root == NULL){
		printf("Error: Array is Empty\n");
		return;
	}
	
	if(root->token1.instance != NULL && root->token2.instance != NULL && root->token3.instance != NULL){
		printf("%*s%-9s\n", root->level * 2," ",concat(concat(concat(root->name, root->token1.instance), root->token2.instance), root->token3.instance));
	}
	else if(root->token1.instance != NULL && root->token2.instance == NULL && root->token3.instance == NULL){
		printf("%*s%-9s\n", root->level * 2," ", concat(root->name, root->token1.instance));
	}
	else if(root->token1.instance != NULL && root->token2.instance != NULL && root->token3.instance == NULL){
		printf("%*s%-9s\n", root->level * 2," ", concat(concat(root->name, root->token1.instance), root->token2.instance));
	}
	else{
		printf("%*s%-9s\n", root->level * 2," ", root->name);
	}

	if (root->child1 != NULL)
		preOrderPrint(root->child1);
	if (root->child2 != NULL)
		preOrderPrint(root->child2);
	if (root->child3 != NULL)
		preOrderPrint(root->child3);
	if (root->child4 != NULL)
		preOrderPrint(root->child4);
}


//concat strings function
char *concat(const char *s1, const char *s2){
  	char *ns = malloc(snprintf(NULL, 0, "%s %s", s1, s2) + 1);
  	if(ns == NULL){
		printf("CONCAT Failed\n");
		exit(EXIT_FAILURE);
  	}
  	sprintf(ns, "%s %s", s1, s2);
  	return ns;
}
