#include <stdio.h>
#include <string.h>
int main (int argc, char* argv[]) {
	char buffer [10] = {0, };

	if (argc == 1) {
		printf("인자가 부족합니다\n");
		return 0;
	}
	
	FILE* fp1 = fopen(argv[2],"w");
	if (fp1 == NULL){
		printf("ERROR\n");
		return 0;
	}
	FILE* fp2 = fopen(argv[1],"r");
	if (fp2 == NULL){
		printf("ERROR\n");
		return 0;
	}
	
	while (feof(fp2) == 0) {
		fread(buffer,1,10,fp2);
		fwrite(buffer,1,strlen(buffer),fp1);
		memset(buffer,0,10);	
	}
	

	fclose(fp1);
	fclose(fp2);
	
	return 0;

}
