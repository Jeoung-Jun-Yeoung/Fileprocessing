#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv) {
	int num;
	int record;
	int filesize;
	char buffer[250];
	struct timeval start,end,rst;
	int result;
	FILE* fp = fopen(argv[1],"rb");

	fread(&num,sizeof(num),1,fp);
	fclose(fp);

	FILE* fp2 = fopen(argv[1],"r");
	fseek(fp2,4,SEEK_SET);

	gettimeofday(&start,NULL);	
	for (int i = 0; i < num; i++) {
		fread(buffer,sizeof(buffer),1,fp2);
	}
	gettimeofday(&end,NULL);
	timersub(&end,&start,&rst);
	result = (int)rst.tv_usec + (int)rst.tv_sec;
	printf("#record: %d elapsed_time: %d us\n",num,result);	
	return 0;
}
