#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct HashTable HashTable;
typedef struct LL LL;
typedef struct Node Node;
typedef struct Record Record;

/*Letters*/
struct HashTable {
	LL **index;
	int capacity;
	int size;
};
/*Words*/
struct LL {
	Node *root;
};

struct Node {
	char *value;
	Record *fileptr;
	Node *next;
};
/*Filename, frequencies*/
struct Record {
	int freq;
	char *file;
	Record *next;
};

Node *CreateNode(char *token);
LL *CreateLL();
HashTable *CreateTable();
void InsertKey(HashTable *hash, char *token, char *argv);
int GetKey(HashTable *hash, char *token);
void RecordInsert(Node *node, char *argv);
void RecordSort(Node *node);

void checkOutputFileName (FILE *fp, char *argv);
void checkDir(HashTable *hash, char* pathName);
void constructPathNames(HashTable *hash, char* pathName);
void printOut (HashTable *hash, char *argv);
void indexFile(HashTable *hash, char *tkFileStr, char *pathName);

#endif
