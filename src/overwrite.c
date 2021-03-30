#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	
	if (argc == 1) {
		printf("인자가 부족합니다.\n");
		return 0;
	}
	FILE* fp = fopen(argv[1],"r+");
	if (fp == NULL) {
		printf("ERROR\n");
		return 0;
	}
	int off = atoi(argv[2]);

	fseek(fp,off,SEEK_SET);
	
	fwrite(argv[3],strlen(argv[3]),1,fp);
	
	fclose(fp);

}
