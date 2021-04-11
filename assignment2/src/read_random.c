#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#define SUFFLE_NUM	50000	

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);

int main(int argc, char **argv) {
	int *read_order_list;
	int num_of_records;
	char buffer[250];
	int result;
	struct timeval start, end, rst;
	
	FILE* fp = fopen(argv[1],"rb");
	
	fread(&num_of_records,sizeof(num_of_records),1,fp);

	read_order_list = (int*)malloc(sizeof(int)*num_of_records);
	fclose(fp);

	FILE* fp1 = fopen(argv[1],"r");
	fseek(fp1,4,SEEK_SET);

	GenRecordSequence(read_order_list, num_of_records);
	
	gettimeofday(&start,NULL);
	for (int i = 0; i < num_of_records; i++) {
		fseek(fp,read_order_list[i] * 250,SEEK_CUR);
		fread(buffer,sizeof(buffer),1,fp);
	}
	gettimeofday(&end,NULL);
	timersub(&end,&start,&rst);
	
	result = (int)rst.tv_usec + (int)rst.tv_sec;
	printf("#record: %d elapsed_time: %d us\n",num_of_records,result);


	return 0;
}

void GenRecordSequence(int *list, int n) {
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
