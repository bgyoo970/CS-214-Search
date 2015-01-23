#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, stderr */
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "indextokenizer.c"
#include "hashTable.h"

Node *CreateNode(char *token) {
	Node *newNode = malloc(sizeof(Node));
	newNode->value = token;
	newNode->next = NULL;
	return newNode;
}

LL *CreateLL() {
	LL *newLL = malloc(sizeof(LL));
	newLL->root = NULL;
	return newLL;
}

HashTable *CreateTable() {
	HashTable *newTable = malloc(sizeof(HashTable));
	newTable->index = malloc(sizeof(LL *) * 36);
	newTable->capacity = 36;
	newTable->size = 36;
	int i;
	for(i = 0; i<37; i++){
		newTable->index[i] = NULL;
	}
	return newTable;
}

Record *CreateRecord(){
	Record *newRec = malloc(sizeof(Record));
	newRec->file = NULL;
	newRec->freq = 1;
	newRec->next = NULL;
	return newRec;
}

void InsertKey(HashTable *hash, char *token, char* argv) {
	int key = GetKey(hash, token);
	Node *temp = NULL;
	Node *ptr, *prev = NULL;
	Record *rec = malloc(sizeof(Record));
	int cmp;
	int cmp2;

	/* Error Check */
	if (hash == NULL) {printf("ERROR. Given HashTable has a NULL value.");return;}
	if (key == -1) {printf("ERROR. Key is invalid");return;}
		
	
	/* If no record exists within the given key, create the LL. The 2nd case is if the Key already has a LL inside. Must SL the horizontal. */
	temp = CreateNode(token);
	rec = CreateRecord();
	temp->fileptr = rec;
	rec->file = argv;


	if (hash->index[key] == NULL) {
		hash->index[key] = CreateLL();	
		hash->index[key]->root = temp;	
	}
	else {
		/*go through linked list at index[key].*/
		ptr = hash->index[key]->root;
		prev = ptr;
		while(ptr != NULL){
			cmp = strcmp(token,ptr->value);

			/* If the tokens are equal to each other */
			if(cmp == 0){
				/* if the file names are equal, just increase record freq */
				/* Otherwise, create a new record, with new filename and freq. Sort. */
				cmp2 = strcmp(ptr->fileptr->file, argv);
				if(cmp2 == 0){
					ptr->fileptr->freq++;
				}
				else if(cmp2 != 0){
					RecordInsert(ptr, argv);
				}
				break;
			}
			/* If the temp token is alpha-lower than the curr (ie ptr) */
			if(cmp > 0){
				if (ptr->next == NULL) {
					ptr->next = temp;
					break;
				}
				else {
					prev = ptr;
					ptr = ptr->next;
					continue;
				}
			}
			if(cmp < 0){
				if (ptr == hash->index[key]->root) {
					temp->next = ptr;
					hash->index[key]->root = temp;
					break;
				}
				else {
					temp->next = ptr;
					prev->next = temp;
					break;
				}
			}
		}
	}
}

int GetKey(HashTable *hash, char *token) {
	int key;
	if(isalpha(token[0])){
		key = token[0] % 97;
	}

	if(isdigit(token[0])){
		key = token[0] - 22;
	}
	
	if (token == NULL || token[0] == '\0')
		return -1;
	else{
		return key;
	}
}

void RecordSort(Node *node){

	Record *curr = node->fileptr;

	int ct = 0;
	curr = node->fileptr;
	while(curr != NULL){
		ct++;
		curr = curr->next;
	}	

	/*Copies the LL into an array*/
	Record *arr = malloc(sizeof(Record) * ct);
	int i;
	curr = node->fileptr;
	for(i = 0; i < ct; i++){
		arr[i].freq = curr->freq;
		arr[i].file = curr->file;
		curr = curr->next;		
	}
		
	/*Bubble sort*/
	Record temp;
	int j;
	for(i = 0; i < ct; i++){
		for(j = 0; j < ct-1; j++){
			if(arr[j].freq < arr[j+1].freq){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
	
	/*Copies back into LL*/
	curr = node->fileptr;
	for(i = 0; i < ct; i++){
		curr->freq = arr[i].freq;
		curr->file = arr[i].file;
		curr = curr->next;
	}
}

void RecordInsert(Node *node, char* argv){
	Record *rec = malloc(sizeof(Record));
	Record *ptr = node->fileptr;
	int compare;
	int boolean = 0;
	rec->file = argv;
	
	while(ptr != NULL){
		compare = strcmp(ptr->file, rec->file);
		rec = CreateRecord();
		rec->file = argv;
		if(compare == 0){
			ptr->freq++;
			boolean = 1;
		}
		ptr = ptr->next;
	}

	/*Inserts records into hash->node*/
	ptr = node->fileptr;
	if(boolean == 0){
		rec->next = ptr;
		node->fileptr = rec;
	}
}

void checkOutputFileName (FILE *fp, char *argv) {
	if(access(argv, F_OK) != -1){
		char a;
		printf("File \"%s\" already exists, overwrite? Input y or n.\n", argv);
		scanf("%c", &a);
		if(a == 'y'){
			printf("YOU CHOSE OPTION: 'y'. OVERWRITING: \"%s\".\n", argv);
			fp = fopen(argv, "w");
		}
		else if(a == 'n'){
			printf("YOU CHOSE OPTION: 'n'. THE PROGRAM IS NOW EXITING\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("ERROR: INCORRECT INPUT. THE PROGRAM IS NOW EXITING\n");
			exit(EXIT_FAILURE);
		}
	}
	else{
		fp = fopen(argv, "w");
	}
}

void indexFile(HashTable *hash, char *tkFileStr, char *pathName) {
	/* Declare vars and Error check tokenizer */
	int i;
	TokenizerT* tokenizer = TKCreate(tkFileStr);
	free(tkFileStr);
	char* token = NULL;
	if(tokenizer == NULL) printf("Error: unable to create tokenizer\n");

	/* tokenize the string and destroy the tokenizer*/
	while((token = TKGetNextToken(tokenizer)) != NULL) {
		InsertKey(hash, token, pathName);
	}
	TKDestroy(tokenizer);

	/*Sort records*/
	Node *ptr = NULL;
	for(i = 0; i < 37; i++){
		
		if(hash->index[i] == NULL){
			continue;
		}
		ptr = hash->index[i]->root;
			
		while(ptr != NULL){
			RecordSort(ptr);
			ptr = ptr->next;
		}
	}
	return;
}

void printOut (HashTable *hash, char *argv) {
	int i;
	Node *ptr = NULL;
	Record *ptr2 = NULL;
	FILE *fp;
	fp = fopen(argv, "w");
	for(i = 0; i < 37; i++){
		if(hash->index[i] == NULL){
			continue;
		}
		
		ptr = hash->index[i]->root;	
		while(ptr != NULL){
			ptr2 = ptr->fileptr;
			fprintf(fp, "<list> %s\n", ptr->value);
			fprintf(fp, "%s %d ", ptr->fileptr->file, ptr->fileptr->freq);
			int ct = 0;
			while(ptr2->next != NULL){
				ptr2 = ptr2->next;
				ct++;
				if(ct == 5){
					fputs("\n", fp);
					ct = 0;
				}
				fprintf(fp, "%s %d ", ptr2->file, ptr2->freq);
			}
			ptr = ptr->next;
			fputs("\n</list>\n", fp);
		}	
	}
	return;
}

void constructPathNames(HashTable *hash, char *pathName)
{
	DIR *dp;
	dp = opendir(pathName);
	struct dirent *entry;
	struct stat buffer;
	char *newPath;
	
	if(dp == NULL) {
		printf("ERROR: Directory does not exist.\n");
		return;
	}
	while((entry = readdir(dp)) != NULL) {
		if( strcmp( entry->d_name, "." ) == 0 || strcmp( entry->d_name, ".." ) == 0) {
			continue;
		}
		char *temp_pathname = (char *)malloc(strlen(pathName) + strlen(entry->d_name) + 2);
		sprintf(temp_pathname, "%s/%s", pathName, entry->d_name);
		lstat(temp_pathname, &buffer);
		if(S_ISDIR(buffer.st_mode))
		{	
			constructPathNames(hash, temp_pathname);
		}
		else if(S_ISREG(buffer.st_mode)) {
			char* tkFileStr = createFileStr(temp_pathname);
			indexFile(hash, tkFileStr, temp_pathname);
		}
	}
	closedir(dp);
}

void checkDir(HashTable *hash, char* pathName)
{	
	struct stat buff;
	lstat(pathName, &buff);
	if(S_ISDIR(buff.st_mode))
	{
		constructPathNames(hash, pathName);
	}
	else if(S_ISREG(buff.st_mode))
	{
		char* tkFileStr = createFileStr(pathName);
		indexFile(hash, tkFileStr, pathName);
	}
	else {
		printf("ERROR: YOUR INPUT DOES NOT HAVE A VALID FILE OR DIRECTORY.\n");
		exit(EXIT_FAILURE);
	}
}


