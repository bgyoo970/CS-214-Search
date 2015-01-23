#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
static int numcount = 0;

int main(int argc, char **argv){
	
	/*Error Checking*/
	if(argc >= 3){
		printf("Error: Input only one argument.\n");
	}
	
	FILE *fp = fopen(argv[1], "r");
	
	if(!fp){
		printf("Error: File not found.\n");
		return 1;
	}
	
	LL *list = malloc(sizeof(LL));
	list->root = resetNode(list->root);
	Node *temp = resetNode(temp);
	char *word = malloc(sizeof(char));
	char c = fscanf(fp, "%s", word);
	int offset;
	
	/*Creation of Linked List*/
	while(c != EOF && word != NULL){
		if(strcmp(word, "<list>") == 0){
			c = fscanf(fp, "%s", word);
			offset = ftell(fp);
			temp = CreateNode(fp, offset, word); 	
			InsertNode(list, temp);
		}
		c = fscanf(fp, "%s", word);
	}
	
	
	free(word);
	fclose(fp);
	
	printf("\nList of commands: \n\tsa <term> //Searches for files containing all given terms\n\tso <term> //Searches for files containing any subset of given terms\n\tq //Quit\n\n");
	char ans[1024];
	char **final = NULL;

	do{
		printf("> ");
		scanf("%[^\n]%*c", ans);

		/*Error check
		CHECK FOR EMPTY COMMAND LATER		
		*/
		if(strcmp(ans, "\0") == 0){
			printf("Empty entry. Please choose from list of commands.\n");
			break;
		}

		final = splitans(ans);
		if(strcmp(final[0], "sa") == 0){
			SA(final, list);
		}
		else if(strcmp(final[0], "so") == 0){
			SO(final, list);
		}
		else if(strcmp(final[0], "q") != 0){
			printf("Invalid entry. Please choose from list of commands.\n");

		}


	}while(strcmp(ans, "q") != 0);

	return 0;
}

void SO(char **words, LL *list){

	Filename *filenames = malloc(sizeof(Filename));
	filenames->list = malloc(sizeof(char*));
	filenames->list = NULL;

	Node *ptr;
	int i;	
	for(i = 1; words[i] != NULL; i++){
		for(ptr = list->root; ptr != NULL; ptr = ptr->next){
			/*If word found*/			
			if(strcmp(ptr->value, words[i]) == 0){
				createFileList(ptr, filenames);
			}
			/*Since LL is in ascending order, don't have to check rest of LL*/
			else if(strcmp(ptr->value, words[i]) < 0){
				break;
			}
			else{
			}
		}
	}

	if(filenames->list == NULL){ printf("\n");}
	else{ printf("%s\n", filenames->list); }
}

void SA(char **words, LL *list){

	Filename *filenames = malloc(sizeof(Filename));
	filenames->list = malloc(sizeof(char*));
	filenames->list = NULL;

	LL *ans = malloc(sizeof(LL*));
	ans->root = resetNode(ans->root);
	

	Node *ptr = resetNode(ptr);
	Node *temp = resetNode(temp);
	temp = ans->root;
	int i;
	for(i = 1; words[i] != NULL; i++){
		for(ptr = list->root; ptr != NULL; ptr = ptr->next){
			/*If word found*/			
			if(strcmp(ptr->value, words[i]) == 0){
				InsertNode(ans, ptr);

			}
			/*Since LL is in ascending order, don't have to check rest of LL*/
			else if(strcmp(ptr->value, words[i]) < 0){
				break;
			}
			/*If word NOT found*/
			else{
			
			}
		}
	}

	Node *curr;
	char **temparr;
	int k, l = 0;
	for(curr = ans->root; curr != NULL; curr = curr->next){
		for(k = 0; curr->files[k] != NULL; k++) {
			temparr[l] = curr->files[k];
			l++;
		}
	}

	
	compressprint(temparr);
	

}

void compressprint(char **arr) {
	int ct = 0;
	char *temp;
	int i, j, curr = 0;
	for(i = 0; arr[i] != NULL; i++) {
		temp = arr[i];
		for(j = i; arr[j] != NULL; j++) {
			if (strcmp(temp, arr[j]) == 0) {
				ct++;	
			}
		}
		printf("ct: %d numcount: %d \n" , ct, numcount);
		if (ct == numcount) {
			printf("%s " , temp);
		}
		else{}
		ct = 0;
	}
	printf("\n");
}


/*just for SO*/
void createFileList(Node *node, Filename *filenames){
	char *res, *dup;

	/*Go through node->files*/
	int i;
	for(i = 0; node->files[i] != NULL; i++){
	
		dup = strdup(node->files[i]);
		
		/*Put in answer array*/
		if(filenames->list == NULL){
			filenames->list = dup;

		}

		else{

			/*Check if filename is already in filenames->list*/
			res = strstr(filenames->list, dup);

			/*filename is not in list*/	
			if(res == NULL){
				filenames->list = strcat(filenames->list, " ");
				filenames->list = strcat(filenames->list, dup);
			}
			else if(res != NULL){}
		}
		
	}
	//printf("%s\n", filenames->list);
	

	

}


char **splitans(char *ans){
	char *word = strtok (ans, " ");
	char **result  = NULL;
	int size = 0;
	numcount = 0;
	while(word){
		result = realloc(result, sizeof (char*)*++size);

		/*If given empty string*/
		if(result == NULL){
			exit(-1);
		} 
		result[size-1] = word;
		word = strtok (NULL, " ");
		numcount++;
	}

	/*Add for null bit*/
	result = realloc(result, sizeof (char*)*(size+1));
	result[size]= '\0';

	//free (result);
	numcount--;
	return result;


}



Node *CreateNode(FILE* fp, int offset, char* word){
	Node *newnode = resetNode(newnode);
	newnode->value = word;

	/*just to make sure file ptr is at correct location*/
	rewind(fp);
	fseek(fp, offset, SEEK_CUR);

	char *w = malloc(sizeof(char)); //word
	char *f; //file;
	char c;
	int count = 1; //count has to be odd for dir/file
	int i = 0;


	c = fscanf(fp, "%s", w);

	for(i = 0; strcmp(w, "</list>") != 0; count++){
		if(count%2 == 0){}
		else if(count%2 == 1){

			f = strdup(w);
			newnode->files[i] = f;
			i++;
		}
		c = fscanf(fp, "%s", w);
	}
	
	free(w);
	return newnode;
}


void InsertNode(LL* list, Node* node){
	/*So nodes won't be duplicated*/
	Node *temp = resetNode(temp);
	char* val = strdup(node->value);
	temp->value = val;
	temp->files = node->files;
	if(list->root->value == NULL){
		list->root = temp;
	}
	
	else{
		temp->next = list->root;
		list->root = temp;
	}

}


Node* resetNode(Node* node){
	node = malloc(sizeof(Node));
	node->next = NULL;
	node->value = NULL;
	node->files = malloc(sizeof(char*));
	return node;

}



