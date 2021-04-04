#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char* argv[]){
	if (argc != 4) {
		printf("인자가 부족합니다\n");
		return 0;
	}

	FILE* fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("ERROR\n");
		return 0;
	}

	fseek(fp,SEEK_SET,SEEK_END);
	int size = ftell(fp);
	int offset = atoi(argv[2]);
	int read = atoi(argv[3]);

	if (size < offset) {
		printf("offset ERROR\n");
		return 0;
	}
	rewind(fp);

	char* buffer = (char*)malloc(sizeof(char) * read);
	
	fseek(fp,offset,SEEK_SET);
	fread(buffer,1,read,fp);
	printf("%s",buffer);

}
