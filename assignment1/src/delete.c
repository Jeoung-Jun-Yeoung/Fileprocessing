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

	int off = atoi(argv[2]);
	int delenum = atoi(argv[3]);

	fseek(fp,SEEK_SET,SEEK_END);
	int size = ftell(fp);

	rewind(fp);

	char* temp = (char*)malloc(sizeof(char) * size);

	fread(temp,size,1,fp);
	
	for (int i = off; i < delenum+off; i++) {
		temp[i] = '+';
	}
	
	int len = size - delenum;
	char* str = (char*)malloc(sizeof(char) * len);

	int i = 0;
	int j = 0;
	while(size != 0){
		if(temp[j] != '+'){
			str[i] = temp[j];
			i++;
		}
		j++;
		size--;
	}
	fclose(fp);
	
	FILE* fp1 = fopen(argv[1],"w");
	if (fp1 == NULL) {
		printf("ERROR\n");
		return 0;
	}
	fwrite(str,strlen(str),1,fp);
	fclose(fp1);	

}
