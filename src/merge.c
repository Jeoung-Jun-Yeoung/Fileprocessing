#include <stdio.h>
#include <string.h>





int main (int argc, char* argv[]) {

	char buffer[100] = {0 ,};

	FILE* fp1 = fopen(argv[1],"w");
	FILE* fp2 = fopen(argv[2],"r");
	
	while (feof(fp2) == 0) {
		fread(buffer,1,100,fp2);
		fwrite(buffer,1,strlen(buffer),fp1);
		memset(buffer,0,100);
	}
	fclose(fp2);

	FILE* fp3 = fopen(argv[3],"r");
	
	while (feof(fp3) == 0) {
		fread(buffer,1,100,fp3);
		fwrite(buffer,1,strlen(buffer),fp1);
		memset(buffer,0,100);
	}
	fclose(fp3);
	fclose(fp1);

}
