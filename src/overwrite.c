#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	
	FILE* fp = fopen(argv[1],"r+");
	int off = atoi(argv[2]);

	fseek(fp,off,SEEK_SET);
	
	fwrite(argv[3],strlen(argv[3]),1,fp);
	
	fclose(fp);

}
