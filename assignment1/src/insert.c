#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc,char* argv[]) {
	if (argc != 4) {
		printf("인자가 부족합니다.\n");
		return 0;
	}

	FILE* fp = fopen(argv[1],"r+");
	if (fp == NULL) {
		printf("ERROR\n");
		return 0;
	}

	int off1 = atoi(argv[2]);
	int off2 = off1 + 1;

	fseek(fp,SEEK_SET,SEEK_END);
	
	int size = ftell(fp);

	if (size < off1) {
		printf("offset ERROR\n");
		return 0;
	}
	
	off2 = size - off1;
	
	rewind(fp);

	char* temp = (char*)malloc(sizeof(char) * off2);
	
	fseek(fp,off1,SEEK_SET);
	
	fread(temp,off2,1,fp);
	
	rewind(fp);
	fseek(fp,off1,SEEK_SET);	
	fwrite(argv[3],strlen(argv[3]),1,fp);
	fwrite(temp,strlen(temp),1,fp);
	fclose(fp);

}
