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
	double a;
	FILE* fp = fopen(argv[1],"rb");

	fread(&num,sizeof(num),1,fp);

	gettimeofday(&start,NULL);
	for (int i = 0; i < num; i++) {
		fread(&buffer,sizeof(buffer),1,fp);
	}
	gettimeofday(&end,NULL);
	timersub(&end,&start,&rst);
	printf("#record: %d elapsed_time: %ld us\n",num,rst.tv_usec);
	
	return 0;
}
