#include "readFile.h"


char* readFileName() {
	char chr;
	char* tempfile = (char*) malloc(sizeof(char)*MAX_FILENAME_SIZE+4);
	int i = 0;

	if (!tempfile) return tempfile;

	while (scanf("%c", &chr), chr!=EOF && chr!='\n'){
		tempfile[i] = chr;
		i++;
	}
	tempfile[i] = '\0';

	return tempfile;
}
