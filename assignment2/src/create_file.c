#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*


 */
typedef struct abcde {
	char a[50];
	char b[50];
	char c[50];
	char d[50];
	char e[50];
}alph;

int main (int argc, char **argv) {
	int record_num = 0;
	char* file_name = (char*)malloc(sizeof(char)*250);
	char binary [4];
	char buffer [250] ={0,};
	alph* sample = (alph*)malloc(sizeof(alph));
	int rst;
	strcpy(binary,argv[1]);
	record_num = atoi(argv[1]);
/*
 head 만들어서 넣어줘야함.
 */
	FILE* fp = fopen(argv[2],"wb");
	fwrite(&record_num, sizeof(record_num), 1, fp);
	if (fp == NULL) {
		printf("ERROR\n");
	}
	fclose(fp);
/*
	FILE* fp1 = fopen(argv[2],"rb");
	fread(&rst,1,1,fp1);
	printf("%d\n",rst);
	fread(&rst,1,1,fp1);
	printf("%d\n",rst);
	fread(&rst,1,1,fp1);
	printf("%d\n",rst);
	fread(&rst,1,1,fp1);
	printf("%d\n",rst);
	fread(&rst,sizeof(rst),1,fp1);
*/

	
	for(int i = 0; i < 49; i++) {
		sample->a[i] = 'a';
		sample->b[i] = 'b';
		sample->c[i] = 'c';
		sample->d[i] = 'd';
		sample->e[i] = 'e';
	}
	strcat(buffer,sample->a);
	strcat(buffer,sample->b);
	strcat(buffer,sample->c);
	strcat(buffer,sample->d);
	strcat(buffer,sample->e);

	FILE* fp1 = fopen(argv[2],"a+");
	for (int i = 0; i < record_num; i++) {
		fwrite(buffer,sizeof(buffer),1,fp1);
	}
	fclose(fp1);

	return 0;
}
