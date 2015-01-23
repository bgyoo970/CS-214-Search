#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "hashTable.c"


#define MAX_HEX_CHARS 2
#define MAX_OCT_CHARS 3



int main(int argc, char** argv) {
	FILE *fp;
	checkOutputFileName(fp, argv[1]);
	HashTable *hash = CreateTable();
	checkDir(hash, argv[2]);
	printOut(hash, argv[1]);
		
	return 0;
}

