#ifndef SEARCH_H
#define SEARCH_H
#include <stdlib.h>
#include <stdio.h>

typedef struct Filename Filename;
typedef struct LL LL;
typedef struct Node Node;

struct Filename{
	char *list;
};

struct LL{
	Node *root;
};

struct Node{
	char *value;
	char **files;
	Node *next;
};


void SO(char **words, LL *list);
void SA(char **words, LL *list);
void createFileList(Node *node, Filename *filenames);
char **splitans(char *ans);
Node *CreateNode(FILE *fp, int offset, char* word);
void CreateFiles(Node* curr, char* filename, int i);
void InsertNode(LL *list, Node *node);
Node *resetNode(Node* node);

void compressprint(char **arr);

#endif
