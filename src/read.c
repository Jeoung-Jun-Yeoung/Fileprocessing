#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define max_size 500

int main (int argc, char* argv[]){
	//argv 2 은 오프셋
	//argv 3는 읽을 바이트수	

	FILE* fp = fopen(argv[1],"r");

	int off = atoi(argv[2]);
	int read = atoi(argv[3]);

	char* buffer = (char*)malloc(sizeof(char) * read);
	
	fseek(fp,off,SEEK_SET);
	fread(buffer,1,read,fp);
	printf("%s\n",buffer);

}
