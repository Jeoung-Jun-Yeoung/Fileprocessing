#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
void readPage(FILE *fp, char *pagebuf, int pagenum)
{

}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{

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
	char* token = strtok(recordbuf,"#");
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
void add(FILE *fp, const Person *p)
{

}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *id)
{

}

int main(int argc, char *argv[])
{
	FILE *fp;  // 레코드 파일의 파일 포인터
	//argv[1] option 
	//argv[2] record filename
	//argv[3] field list
	fp = fopen(argv[2],"a+");
	if(!strcmp(argv[1],"a")) { // add 기능.
		Person* p = (Person*)malloc(sizeof(Person));
		strncpy(p->id,argv[3] ,strlen(argv[3])); // 마지막 null 확인.
		strncpy(p->name,argv[4],strlen(argv[4]));
		strncpy(p->age,argv[5] ,strlen(argv[5]));
		strncpy(p->addr,argv[6] ,strlen(argv[6]));
		strncpy(p->phone,argv[7] ,strlen(argv[7]));
		strncpy(p->email,argv[8] ,strlen(argv[8]));
		char* recordbuf = (char*)malloc(sizeof(char)*MAX_RECORD_SIZE);
		
		pack(recordbuf,p);
		printf("recordbuf %s \n",recordbuf);
		Person* temp = (Person*)malloc(sizeof(Person));
		unpack(recordbuf,temp);
		printf("%s %s %s \n",temp->id,temp->age,temp->phone);
	}
	else if(!strcmp(argv[1],"d")){

	}
	else {
		printf("지원하지 않는 option입니다.\n");
	}

	
	//filelist의 size를 알아야함.


	return 1;
}
