#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "person.h"
//필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 위의 readPage() 함수를 호출하여 pagebuf에 저장한 후, 여기에 필요에 따라서 새로운 레코드를 저장하거나
// 삭제 레코드 관리를 위한 메타데이터를 저장합니다. 그리고 난 후 writePage() 함수를 호출하여 수정된 pagebuf를
// 레코드 파일에 저장합니다. 반드시 페이지 단위로 읽거나 써야 합니다.
//
// 주의: 데이터 페이지로부터 레코드(삭제 레코드 포함)를 읽거나 쓸 때 페이지 단위로 I/O를 처리해야 하지만,
// 헤더 레코드의 메타데이터를 저장하거나 수정하는 경우 페이지 단위로 처리하지 않고 직접 레코드 파일을 접근해서 처리한다.

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void readPage(FILE *fp, char *pagebuf, int pagenum) {
	fseek(fp,(pagenum - 1) * PAGE_SIZE + 16,SEEK_SET); // header aredea 건너띄어야하지않나?
											// haderrecod 건너띄고
	fread(pagebuf,PAGE_SIZE,1,fp);

}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp,(pagenum - 1) * PAGE_SIZE + 16,SEEK_SET);
	fwrite(pagebuf,PAGE_SIZE,1,fp);
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고, pack() 함수를 사용하여
// 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다. 
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
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다.
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
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
//
void add(FILE *fp, const Person *p) {
	char* recordbuf = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
	pack(recordbuf,p); 

	int next_page;
	int next_record;

	int before_page;
	int before_record;
	
	char header_record[16];
	
	rewind(fp); // fp 초기화.
	
	fread(header_record,sizeof(header_record),1,fp); // file의 header record를 읽는다.

	before_record = -1;
	before_page = -1;

	memcpy(&next_page,header_record + 8 ,4); // header record에서 nextpage를 읽는다.
	memcpy(&next_record,header_record + 12 ,4); // header record에서 nextrecord를 읽는다.

	char pagebuf[PAGE_SIZE];

	int flag = 0;

	while(next_page != -1 && next_record != -1) { // 가장 최근에 삭제된 page의 record를 방문.
		readPage(fp,pagebuf,next_page); // 가장 최근에 삭제된 darapage를 가져온다.
		
		char header_area[HEADER_AREA_SIZE]; // header area 선언
		
		memcpy(header_area,pagebuf,HEADER_AREA_SIZE); // header area를 읽는다.
		
		int offset; // data area 안에서 가려는 레코드의 시작 위치.
		int length; // data area에서 해당 record의 길이

		int curr_page = next_page; // next page를 현재 page로 설정한다. 
		int curr_record = next_record;

		memcpy(&offset,header_area + 8 * curr_record + 4,4); // offset + length = 8 이므로 8 * recordnum을 하면 해당 record의 offset과 length에 접근
		memcpy(&length,header_area + 8 * curr_record + 8,4); // length 읽기.  
		
		memcpy(&next_page,pagebuf + (HEADER_AREA_SIZE + offset + 1) ,4); // 만약, 현재 page에 담을수 없으면 next로 넘어가야 하기때문에 읽어둔다.
		memcpy(&next_record,pagebuf + (HEADER_AREA_SIZE + offset + 5) ,4); // recordnum도 읽는다.
		
		if(strlen(recordbuf) <= length) { // 만약 현재 page의 record의 쓸 수 있다면,
			memcpy(pagebuf + (offset + HEADER_AREA_SIZE),recordbuf,strlen(recordbuf)); // 해당 page의 record자리에 방문해서 record에 작성.
			writePage(fp,pagebuf,curr_page); // page업데이트
			// header record에서 페이지수, 레코드수 업데이트 필요 x
			// 그러나 header record에서 가장 최근 page번호와 record번호 업데이트 필요.
			if(before_page == -1 && before_record == -1) {
				rewind(fp); // 헤더레코드 접근을 위해 fp를 초기화 .
				memcpy(header_record + 8,&next_page,sizeof(int));
				memcpy(header_record + 12,&next_record,sizeof(int));
				fwrite(header_record,sizeof(header_record),1,fp);	
			}
			else { // before_page != -1
				char temp_pagebuf[PAGE_SIZE];
				int temp_offset;
				readPage(fp,temp_pagebuf,before_page);
				memcpy(&temp_offset,temp_pagebuf + 4 + before_record * 8,4);
				memcpy(temp_pagebuf + temp_offset + 1 + HEADER_AREA_SIZE,&next_page,4);
				memcpy(temp_pagebuf + temp_offset + 5 + HEADER_AREA_SIZE,&next_record,4);
			}	

			flag = 1;
			break;
		}
		before_page = curr_page;
		before_record = curr_record;
		
	}

	if(flag != 1) {

		rewind(fp);
		fread(header_record,sizeof(header_record),1,fp); // file의 header record를 읽는다.
		
		int num_of_data_pages = 0; // header record
		int num_of_records = 0; // header record

		memcpy(&num_of_data_pages,header_record,sizeof(int));
		memcpy(&num_of_records,header_record + 4,sizeof(int));
		printf("pg%d re%d\n",num_of_data_pages,num_of_records);
		readPage(fp,pagebuf,num_of_data_pages);
		int temp = 0;
		int sum = 0;
		int temp_off = 0;
		memcpy(&temp,pagebuf,sizeof(int));
		for(int i = 0; i < temp; i++){
			int len;
			memcpy(&len,pagebuf + (temp + 1) * 8 ,sizeof(int));
			sum += len;
		}
		printf("sum %d\n",sum);
		if(sum + strlen(recordbuf) <= PAGE_SIZE - HEADER_AREA_SIZE && temp < 2 && num_of_data_pages != 0) { // append 가능.
			temp++;
			temp_off = sum;
		}
		else {
			temp = 1;
			temp_off = 0;
			num_of_data_pages++;
			rewind(fp); // fp 초기화.
			memcpy(header_record,&num_of_data_pages,sizeof(int));
		}

		num_of_records++;
		memcpy(header_record + 4,&num_of_records,sizeof(int));
		fwrite(header_record,sizeof(header_record),1,fp);
		//header record 업데이트 끝.
		memcpy(pagebuf,&temp,sizeof(int));
		memcpy(pagebuf + (temp * 8) - 4,&temp_off,sizeof(int));

		int tmp = strlen(recordbuf);
		printf("tmp is %d\n",tmp);
		memcpy(pagebuf + (temp * 8), &tmp,sizeof(int));
		memcpy(pagebuf + HEADER_AREA_SIZE + temp_off,recordbuf,strlen(recordbuf));
		writePage(fp,pagebuf,num_of_data_pages);
	}

		
}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *id) {

}

int main(int argc, char *argv[])
{
	FILE *fp;  // 레코드 파일의 파일 포인터

	/*
	data page에 read 나 write는 readpage, writepage 함수의 호출을 통해서만 가능
	*/
	int exist_file = access(argv[2],0);

	int SomeOfDataPage; // header record
	int NumOfRecord; // header record
	int RecentPage; // header record
	int RecentRecord; // header record	
	char Pagebuf[PAGE_SIZE];
	char headerrecord [16];

	if(exist_file == 0) { // file이 기존에 존재할떄.
		fp = fopen(argv[2],"a+");
		printf("file is exist.\n");
	}
	else if(exist_file == -1){ // file이 기존에 존재하지 않을때
		fp = fopen(argv[2],"w+"); // 초기화 과정 필요.
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
	
//./a.out a person.dat "9804191156" "jy jeoung" "24" "incheon" "01092695054" "asdad@asdasd"

	if(!strcmp(argv[1],"a")) { // add 기능.
		Person* p = (Person*)malloc(sizeof(Person));
		strncpy(p->id,argv[3] ,strlen(argv[3])); // 마지막 null 확인.
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
	else if(!strcmp(argv[1],"d")) { //delete 기능

	}
	else {
		printf("지원하지 않는 option입니다.\n");
	}

	
	//filelist의 size를 알아야함.


	return 1;
}
