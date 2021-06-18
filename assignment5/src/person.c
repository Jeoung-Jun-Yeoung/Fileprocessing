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
	fseek(fp,(pagenum) * PAGE_SIZE + 16,SEEK_SET); 
	fread(pagebuf,PAGE_SIZE,1,fp);

}

//
// ������ ������ �����͸� �־��� ������ ��ȣ�� �ش��ϴ� ���ڵ� ������ ��ġ�� �����Ѵ�. 
// ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp,(pagenum) * PAGE_SIZE + 16,SEEK_SET);
	fwrite(pagebuf,PAGE_SIZE,1,fp);
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
	char* temp = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
	memcpy(temp,recordbuf,strlen(recordbuf)); 
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
	
	rewind(fp); // fp �ʱ�ȭ.
	
	fread(header_record,sizeof(header_record),1,fp); // file�� header record�� �д´�.

	before_record = -1;
	before_page = -1;
	
	int count_page;
	
	memcpy(&count_page,header_record,sizeof(int));
	int sum_of_records;
	memcpy(&sum_of_records,header_record + 4,sizeof(int));

	memcpy(&next_page,header_record + 8 ,sizeof(int)); // header record���� nextpage�� �д´�.
	memcpy(&next_record,header_record + 12 ,sizeof(int)); // header record���� nextrecord�� �д´�.
	char* pagebuf = (char*)malloc(sizeof(char)*PAGE_SIZE);
	
	if(count_page == 0) {
		int first_num_of_data_pages = 1;
		rewind(fp); // fp �ʱ�ȭ.
		memcpy(header_record,&first_num_of_data_pages,sizeof(int));
		int first_num_of_records = 1;
		memcpy(header_record + 4,&first_num_of_records,sizeof(int));
		fwrite(header_record,sizeof(header_record),1,fp);
		//header record ������Ʈ ��.
		
		int first_temp = 1; // page�� �� ���ڵ� ����. 
		memcpy(pagebuf,&first_temp,sizeof(int));
		
		int first_temp_off = 0; // ù��° page�� ù��° record�� offset�� 0
		memcpy(pagebuf + 4,&first_temp_off,sizeof(int));

		int first_tmp = strlen(recordbuf);
		memcpy(pagebuf +  8, &first_tmp,sizeof(int));
		memcpy(pagebuf + HEADER_AREA_SIZE,recordbuf,strlen(recordbuf));
		writePage(fp,pagebuf,first_num_of_data_pages-1);
		return;
	}
	int flag = 0;

	while(next_page != -1 && next_record != -1) { // ���� �ֱٿ� ������ page�� record�� �湮.
		readPage(fp,pagebuf,next_page); // ���� �ֱٿ� ������ darapage�� �����´�.
		char header_area[HEADER_AREA_SIZE]; // header area ����
		
		memcpy(header_area,pagebuf,HEADER_AREA_SIZE); // header area�� �д´�.
		
		int offset; // data area �ȿ��� ������ ���ڵ��� ���� ��ġ.
		int length; // data area���� �ش� record�� ����

		int curr_page = next_page; // next page�� ���� page�� �����Ѵ�. 
		int curr_record = next_record;

		memcpy(&offset,header_area + 8 * curr_record + 4,4); // offset + length = 8 �̹Ƿ� 8 * recordnum�� �ϸ� �ش� record�� offset�� length�� ����
		memcpy(&length,header_area + 8 * curr_record + 8,4); // length �б�.  

		memcpy(&next_page,pagebuf + (HEADER_AREA_SIZE + offset + 1) ,4); // ����, ���� page�� ������ ������ next�� �Ѿ�� �ϱ⶧���� �о�д�.
		memcpy(&next_record,pagebuf + (HEADER_AREA_SIZE + offset + 5) ,4); // recordnum�� �д´�.

		if(strlen(recordbuf) <= length) { // ���� ���� page�� record�� �� �� �ִٸ�,
			memcpy(pagebuf + (offset + HEADER_AREA_SIZE),recordbuf,strlen(recordbuf)); // �ش� page�� record�ڸ��� �湮�ؼ� record�� �ۼ�.
			writePage(fp,pagebuf,curr_page); // page������Ʈ
			// header record���� ��������, ���ڵ�� ������Ʈ �ʿ� x
			// �׷��� header record���� ���� �ֱ� page��ȣ�� record��ȣ ������Ʈ �ʿ�.
			if(before_page == -1 && before_record == -1) {
				rewind(fp); // ������ڵ� ������ ���� fp�� �ʱ�ȭ .
				memcpy(header_record + 8,&next_page,sizeof(int));
				memcpy(header_record + 12,&next_record,sizeof(int));
				fwrite(header_record,sizeof(header_record),1,fp);	
			}
			else { // before_page != -1 
				char temp_pagebuf[PAGE_SIZE];
				int temp_offset;
				readPage(fp,temp_pagebuf,before_page);
				memcpy(&temp_offset,temp_pagebuf + 4 + (before_record * 8),4);
				memcpy(temp_pagebuf + temp_offset + 1 + HEADER_AREA_SIZE,&next_page,4);
				memcpy(temp_pagebuf + temp_offset + 5 + HEADER_AREA_SIZE,&next_record,4);
			}	

			flag = 1;
			break;
		}
		before_page = curr_page;
		before_record = curr_record;
	}

// od -xc person.c

	if(flag != 1) { // �ֱٿ� �����Ȱ��� ���⿡ ���� ������ page�� append�ؾ��Ѵ�.
		//rewind(fp);
		//fread(header_record,sizeof(header_record),1,fp); // file�� header record�� �д´�.
		int num_of_data_pages = 0; // header record
		int num_of_records = 0; // header record

		memcpy(&num_of_data_pages,header_record,sizeof(int));
		memcpy(&num_of_records,header_record + 4,sizeof(int));

		readPage(fp,pagebuf,num_of_data_pages-1);	
	
		int temp = 0;
		int sum = 0;
		int temp_off = 0;
		
		memcpy(&temp,pagebuf,sizeof(int));
		
		
		for(int i = 0; i < temp; i++) {
			int len;
			memcpy(&len,pagebuf + (i + 1) * 8 ,sizeof(int));
			sum = sum + len;
		}

		
		int update;

		if (sum + strlen(recordbuf) <= DATA_AREA_SIZE && 8 <= HEADER_AREA_SIZE - (temp * 8) - 4) { // append ����.
			temp++;
			temp_off = sum;
			update = 1;
		}

		else {
			temp = 1;
			temp_off = 0;
			num_of_data_pages++;
			update = 0;
		}

		
		num_of_records++;
		
		memcpy(header_record,&num_of_data_pages,sizeof(int));
		memcpy(header_record + 4,&num_of_records,sizeof(int));
		fseek(fp,SEEK_SET,SEEK_SET);
		fwrite(header_record,sizeof(header_record),1,fp);
		//header record ������Ʈ ��.
		if(update == 1) {
			memcpy(pagebuf,&temp,sizeof(int));
			memcpy(pagebuf + (temp * 8) - 4,&temp_off,sizeof(int));
			int tmp = strlen(recordbuf);
			memcpy(pagebuf + (temp * 8), &tmp,sizeof(int));
			memcpy(pagebuf + HEADER_AREA_SIZE + temp_off,recordbuf,strlen(recordbuf));
			writePage(fp,pagebuf,num_of_data_pages-1);
		}

		else if (update == 0) {
			char* new_pagebuf = (char*)malloc(sizeof(char) * PAGE_SIZE);
			memcpy(new_pagebuf,&temp,sizeof(int));
			memcpy(new_pagebuf + (temp * 8) - 4,&temp_off,sizeof(int));
			int new_tmp = strlen(recordbuf);
			memcpy(new_pagebuf + (temp * 8), &new_tmp,sizeof(int));
			memcpy(new_pagebuf + HEADER_AREA_SIZE + temp_off,recordbuf,strlen(recordbuf));
			writePage(fp,new_pagebuf,num_of_data_pages-1);
		}

	}
		
}

//
// �ֹι�ȣ�� ��ġ�ϴ� ���ڵ带 ã�Ƽ� �����ϴ� ����� �����Ѵ�.
//
void delete(FILE *fp, const char *id) {
	char header_record[16];
	int num_of_records;
	int num_of_data_pages;
	int recent_delete_page;
	int recent_delete_record;
	rewind(fp);
	fread(header_record,sizeof(header_record),1,fp);
	memcpy(&num_of_data_pages,header_record,sizeof(int));
	memcpy(&recent_delete_page,header_record + 8,sizeof(int));
	memcpy(&recent_delete_record,header_record + 12,sizeof(int));
	char mark = '*';
	if(num_of_data_pages == 0) { // data page �� ���� ��� ���� ���ڵ带 ã�������⿡ ����ó��
		return;
	}
	else { // data page�� �Ѱ��� �ִٸ� page�� �о ���ڵ带 ã�ƾ���.
		char* pagebuf = (char*)malloc(sizeof(char) * PAGE_SIZE);
		for(int i = 0; i < num_of_data_pages; i++) {
			char* header_area = (char*)malloc(sizeof(char)*HEADER_AREA_SIZE);
			readPage(fp,pagebuf,i); // file�� �� data page�� �о�´�.
			memcpy(header_area,pagebuf,HEADER_AREA_SIZE);// �о�� page�� header_area�� �д´�.
			int in_page_records;
			memcpy(&in_page_records,header_area,sizeof(int)); // �� page�� record�� � �ִ��� Ȯ���ؼ�
			
			
			for(int j = 0; j < in_page_records; j++) { // �� record�� �����ؼ� ������ �����´�.
				int offset;
				int len;
				memcpy(&offset,header_area + 8 * j + 4,4); // offset + length = 8 �̹Ƿ� 8 * recordnum�� �ϸ� �ش� record�� offset�� length�� ����
				memcpy(&len,header_area + 8 * j + 8,4); // length �б�.
				
				char* temp_record = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
				
				memcpy(temp_record,pagebuf + (offset + HEADER_AREA_SIZE),len); // �ش緹�ڵ尡������
				
				
				Person* p = (Person*)malloc(sizeof(Person));
				unpack(temp_record,p);
				if (strncmp(temp_record,id,strlen(p->id)) == 0 ) {// id���� ��. ���ٸ� �ش� ���ڵ� �����ؾ���
					memcpy(pagebuf + (offset + HEADER_AREA_SIZE),&mark,sizeof(char)); // markǥ�����ְ� 
					//header record �� ���� �ֱ� ���� ���ڵ� ������ ���.
					memcpy(pagebuf + (offset + HEADER_AREA_SIZE + 1),&recent_delete_page,sizeof(int)); 
					memcpy(pagebuf + (offset + HEADER_AREA_SIZE + 5),&recent_delete_record,sizeof(int));
					writePage(fp,pagebuf,i);
					// ������ ������ڵ��� ���� ��������ȣ�� ���ڵ� ��ȣ �ۼ��ؾ���. i���� j��.
					memcpy(header_record + 8,&i,sizeof(int));
					memcpy(header_record + 12,&j,sizeof(int));
					fseek(fp,SEEK_SET,SEEK_SET);
					fwrite(header_record,sizeof(header_record),1,fp);
					break;		
			}
		}
	}
}
	
}


//
// �־��� ���ڵ� ����(recordfp)�� �̿��Ͽ� ���� �ε��� ����(idxfp)�� �����Ѵ�.
//
void createIndex(FILE *idxfp, FILE *recordfp) {
	char header_record[16];
	int pagenum;
	rewind(recordfp); // fp �ʱ�ȭ.
	fread(header_record,sizeof(header_record),1,recordfp); // file�� header record�� �д´�.
	memcpy(&pagenum,header_record,sizeof(int));
	int recordnum;
	memcpy(&recordnum,header_record + 4,sizeof(int));
	// page���� ����.
	// fp, pagebuf , pagenunm
	char* pagebuf = (char*)malloc(sizeof(char)*PAGE_SIZE);
	char* recordbuf = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
	char id[14];
	double* id_array = (double*)malloc(sizeof(double)*recordnum); // ���Ŀ�
	char** record_array = (char**)malloc(sizeof(char*)*recordnum); // �ۼ���
	int cnt = 0;
	for(int i = 0; i < recordnum; i++){
		record_array[i] = (char*)malloc(sizeof(char)*21);
	}

	for(int i = 0; i < pagenum; i++){
		readpage(recordfp,pagebuf,i);
		int numofrecord;
		int offset;
		int length;
		memcpy(&numofrecord,pagebuf,sizeof(int));
		for(int j = 0; j < numofrecord; j++){
			memcpy(&offset,pagebuf + 4 + j * 8,sizeof(int));
			memcpy(&length,pagebuf + 8 + j * 8,sizeof(int));
			memcpy(recordbuf,pagebuf + offset + HEADER_AREA_SIZE,length);
			char ck;
			memcpy(&ck,recordbuf,1);
			if(ck == '*'){
				continue;
			}			
			Person* p = (Person*)malloc(sizeof(Person));
			unpack(recordbuf,p);
			id_array[cnt] = atof(p->id);
			memcpy(record_array[cnt],p->id,13);
			memcpy(record_array[cnt] + 13,i,sizeof(int));
			memcpy(record_array[cnt] + 17,j,sizeof(int));
			cnt++;
			// index file�� ���� record ù 4����Ʈ�� ������ ��ȣ, �״��� ���ڵ� ��ȣ �״��� ���̵�
			// ���� �ε��� ���� �о �������� ������ ���ݲ� �ۼ�.
			//p->id
		}
	}
	//id �迭 �����ϰ�, �״��� record array����Ī �ؼ� �ٷ� �ۼ�. idx�����Ϳ� �ۼ��Ѵ�.
}

//
// �־��� ���� �ε��� ����(idxfp)�� �̿��Ͽ� �ֹι�ȣ Ű���� ��ġ�ϴ� ���ڵ��� �ּ�, �� ������ ��ȣ�� ���ڵ� ��ȣ�� ã�´�.
// �̶�, �ݵ�� ���� �˻� �˰����� ����Ͽ��� �Ѵ�.
//
void binarysearch(FILE *idxfp, const char *id, int *pageNum, int *recordNum) {
	rewind(idxfp);
	//fseek(idxfp,SEE_END,SEEK_SET);

}

int main(int argc, char *argv[])
{
	FILE *fp;  // ���ڵ� ������ ���� ������

	/*
	data page�� read �� write�� readpage, writepage �Լ��� ȣ���� ���ؼ��� ����
	*/
	int exist_file = access(argv[2],F_OK);

	int SomeOfDataPage; // header record
	int NumOfRecord; // header record
	int RecentPage; // header record
	int RecentRecord; // header record	
	char Pagebuf[PAGE_SIZE];
	char headerrecord [16];

	if(exist_file == 0) { // file�� ������ �����ҤԤ��¤�.
		fp = fopen(argv[2],"r+");
	}
	else if(exist_file == -1){ // file�� ������ �������� ������
		fp = fopen(argv[2],"w+"); // �ʱ�ȭ ���� �ʿ�.
		SomeOfDataPage = 0;
		NumOfRecord = 0;
		RecentPage = -1;
		RecentRecord = -1;

		memcpy(headerrecord,&SomeOfDataPage,sizeof(int));
		memcpy(headerrecord + 4,&NumOfRecord,sizeof(int));
		memcpy(headerrecord + 8,&RecentPage,sizeof(int));
		memcpy(headerrecord + 12,&RecentRecord,sizeof(int));
		
		fwrite(headerrecord,sizeof(headerrecord),1,fp);
		
	} 
	
	if(!strcmp(argv[1],"a")) { // add ���.
		Person* p = (Person*)malloc(sizeof(Person));
		strncpy(p->id,argv[3] ,strlen(argv[3])); // ������ null Ȯ��.
		strncpy(p->name,argv[4],strlen(argv[4]));
		strncpy(p->age,argv[5] ,strlen(argv[5]));
		strncpy(p->addr,argv[6] ,strlen(argv[6]));
		strncpy(p->phone,argv[7] ,strlen(argv[7]));
		strncpy(p->email,argv[8] ,strlen(argv[8]));
		 
		add(fp,p);
		
		
	}
	else if(!strcmp(argv[1],"d")) { //delete ���
		char delete_id[14];
		strncpy(delete_id,argv[3],strlen(argv[3]));
		delete(fp,delete_id);
	}
	else if(!strcmp(argv[1],"i")){

		
	}
	else if(!strcmp(argv[1],"b")){
		
	}
	else {
		printf("�������� �ʴ� option�Դϴ�.\n");
	}

	return 1;
}
