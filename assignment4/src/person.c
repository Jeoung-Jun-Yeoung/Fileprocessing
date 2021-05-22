#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "person.h"
//�ʿ��� ��� ��� ���ϰ� �Լ��� �߰��� �� ����

// ���� ������� �����ϴ� ����� ���� �ٸ� �� ������ �ణ�� ������ �Ӵϴ�.
// ���ڵ� ������ ������ ������ ���� �����Ǳ� ������ ����� ���α׷����� ���ڵ� ���Ϸκ��� �����͸� �а� �� ����
// ������ ������ ����մϴ�. ���� �Ʒ��� �� �Լ��� �ʿ��մϴ�.
// 1. readPage(): �־��� ������ ��ȣ�� ������ �����͸� ���α׷� ������ �о�ͼ� pagebuf�� �����Ѵ�
// 2. writePage(): ���α׷� ���� pagebuf�� �����͸� �־��� ������ ��ȣ�� �����Ѵ�
// ���ڵ� ���Ͽ��� ������ ���ڵ带 �аų� ���ο� ���ڵ带 ���ų� ���� ���ڵ带 ������ ����
// ���� readPage() �Լ��� ȣ���Ͽ� pagebuf�� ������ ��, ���⿡ �ʿ信 ���� ���ο� ���ڵ带 �����ϰų�
// ���� ���ڵ� ������ ���� ��Ÿ�����͸� �����մϴ�. �׸��� �� �� writePage() �Լ��� ȣ���Ͽ� ������ pagebuf��
// ���ڵ� ���Ͽ� �����մϴ�. �ݵ�� ������ ������ �аų� ��� �մϴ�.
//
// ����: ������ �������κ��� ���ڵ�(���� ���ڵ� ����)�� �аų� �� �� ������ ������ I/O�� ó���ؾ� ������,
// ��� ���ڵ��� ��Ÿ�����͸� �����ϰų� �����ϴ� ��� ������ ������ ó������ �ʰ� ���� ���ڵ� ������ �����ؼ� ó���Ѵ�.

//
// ������ ��ȣ�� �ش��ϴ� �������� �־��� ������ ���ۿ� �о �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
void readPage(FILE *fp, char *pagebuf, int pagenum) {


}

//
// ������ ������ �����͸� �־��� ������ ��ȣ�� �ش��ϴ� ���ڵ� ������ ��ġ�� �����Ѵ�. 
// ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum) {

}

//
// ���ο� ���ڵ带 ������ �� �͹̳ηκ��� �Է¹��� ������ Person ����ü�� ���� �����ϰ�, pack() �Լ��� ����Ͽ�
// ���ڵ� ���Ͽ� ������ ���ڵ� ���¸� recordbuf�� �����. 
// 
void pack(char *recordbuf, const Person *p){
	strcat(recordbuf,p->id);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->name);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->age);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->addr);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->phone);
	strcat(recordbuf,"#");
	strcat(recordbuf,p->email);
	strcat(recordbuf,"#");
}

// 
// �Ʒ��� unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� ���ڵ带 ����ü�� ��ȯ�� �� ����Ѵ�.
//
void unpack(const char *recordbuf, Person *p) {
	char* temp = (char*)malloc(sizeof(char)*sizeof(recordbuf));
	memcpy(temp,recordbuf,sizeof(recordbuf)); 
	char* token = strtok(temp,"#");
	strcpy(p->id,token);
	int i = 0;
	while(token != NULL){
		token = strtok(NULL,"#");
		switch (i) {
			case 0 : 
				strcpy(p->name,token);
				break;
			case 1 : 
				strcpy(p->age,token);
				break;
			case 2 :
				strcpy(p->addr,token);
				break;
			case 3 :
				strcpy(p->phone,token);
				break;
			case 4 : 
				strcpy(p->email,token);
				break;
			default :
				break;
		}
		i++;
	}
}

//
// ���ο� ���ڵ带 �����ϴ� ����� �����ϸ�, �͹̳ηκ��� �Է¹��� �ʵ尪���� ����ü�� ������ �� �Ʒ� �Լ��� ȣ���Ѵ�.
//
void add(FILE *fp, const Person *p) {
	char* recordbuf = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
	pack(recordbuf,p);

	int next_page;
	int next_record;
	int before_page;
	int before_record;
	char header_record[16];
	
	rewind(fp);
	
	fread(header_record,sizeof(header_record),1,fp);
	before_record = -1;
	before_page = -1;
	memcpy(&next_page,header_record + 8 ,4);
	memcpy(&next_record,header_record + 12 ,4);

	char pagebuf[PAGE_SIZE];
	int flag = 0;

	while(next_page != -1 && next_record != -1) {
		readPage(fp,pagebuf,next_page);
		
		char header_area[HEADER_AREA_SIZE];
		
		memcpy(header_area,pagebuf,HEADER_AREA_SIZE);
		
		int offset; // data area �ȿ��������� ���ڵ��� ���� ��ġ.
		int length;

		memcpy(&offset,header_area + 8 * next_record + 4,4);
		memcpy(&length,header_area + 8 * (next_record+1),4);
		int curr_page = next_page;
		int curr_record = next_record;
		memcpy(&next_page,pagebuf + (HEADER_AREA_SIZE + offset +1),4);
		memcpy(&next_record ,pagebuf + (HEADER_AREA_SIZE + offset + 5),4);
		if(strlen(recordbuf) <= length){
			// ���� ���� ���� �����ϰ� �ִ� ������ ������ ������Ʈ.
			// if() �Ѵ� -1�϶� headerrecord�� ���� ������Ʈ
			//before_page = next_page;
			//before_record = next_record;

			//�ƴҶ��� readPage(before)�ؼ� writePage(before)
			memcpy(pagebuf + (offset + HEADER_AREA_SIZE),recordbuf,strlen(recordbuf));
			writePage(fp,pagebuf,curr_page);
			flag = 1;
			break;
		}
		before_page = curr_page;
		before_record = curr_record;
	}

	// if(!flag) {



}

//
// �ֹι�ȣ�� ��ġ�ϴ� ���ڵ带 ã�Ƽ� �����ϴ� ����� �����Ѵ�.
//
void delete(FILE *fp, const char *id)
{

}

int main(int argc, char *argv[])
{
	FILE *fp;  // ���ڵ� ������ ���� ������
	//argv[1] option 
	//argv[2] record filename
	//argv[3] field list

	/*
	data page�� read �� write�� readpage, writepage �Լ��� ȣ���� ���ؼ��� ����
	*/
	int exist_file = access(argv[2],0);
	int SomeOfDataPage; // header record
	int NumOfRecord; // header record
	int RecentPage; // header record
	int RecentRecord; // header record	
	char Pagebuf[PAGE_SIZE];
	char headerrecord [16];

	if(exist_file == 0) { // file�� ������ �����ҤԤ��¤�.
		fp = fopen(argv[2],"ab+");
		printf("file is exist.\n");
		printf("cur %ld\n",ftell(fp));
	}
	else if(exist_file == -1){ // file�� ������ �������� ������
		fp = fopen(argv[2],"wb+"); // �ʱ�ȭ ���� �ʿ�.
		SomeOfDataPage = 0;
		NumOfRecord = 0;
		RecentPage = -1;
		RecentRecord = -1;

		memcpy(headerrecord,&SomeOfDataPage,sizeof(int));
		memcpy(headerrecord + 4,&NumOfRecord,sizeof(int));
		memcpy(headerrecord + 8,&RecentPage,sizeof(int));
		memcpy(headerrecord + 12,&RecentRecord,sizeof(int));
		
		fwrite(headerrecord,sizeof(headerrecord),1,fp);
		
		printf("file is not exist.\n");
		
	} 
	
//./a.out a  person.dat "9804191156" "jy jeoung" "24" "incheon" "01092695054" "asdad@asdasd"

	if(!strcmp(argv[1],"a")) { // add ���.
		Person* p = (Person*)malloc(sizeof(Person));
		strncpy(p->id,argv[3] ,strlen(argv[3])); // ������ null Ȯ��.
		strncpy(p->name,argv[4],strlen(argv[4]));
		strncpy(p->age,argv[5] ,strlen(argv[5]));
		strncpy(p->addr,argv[6] ,strlen(argv[6]));
		strncpy(p->phone,argv[7] ,strlen(argv[7]));
		strncpy(p->email,argv[8] ,strlen(argv[8]));
		 
		add(fp,p);
		
		/*
		printf("cur %ld\n",ftell(fp));
		fread(&SomeOfDataPage,sizeof(SomeOfDataPage),1,fp); // headerrecord
		fread(&NumOfRecord,sizeof(NumOfRecord),1,fp);
		fread(&RecentPage,sizeof(RecentPage),1,fp);
		fread(&RecentRecord,sizeof(RecentRecord),1,fp);
		printf("cur %ld\n",ftell(fp));
		printf("SomeOfDataPage %d NumOfRecord %d RecentPage %d RecentRecord %d \n",SomeOfDataPage,NumOfRecord,RecentPage,RecentRecord);
		printf("cur %ld\n",ftell(fp));
		int datasize = strlen(recordbuf);
		*/





		//Person* temp = (Person*)malloc(sizeof(Person));
		//unpack(recordbuf,temp);
		//printf("%s %s %s \n",temp->id,temp->age,temp->phone);
	}
	else if(!strcmp(argv[1],"d")) { //delete ���

	}
	else {
		printf("�������� �ʴ� option�Դϴ�.\n");
	}

	
	//filelist�� size�� �˾ƾ���.


	return 1;
}
