// 주의사항
// 1. blockmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blockmap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(blockmap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
// 필요한 경우 헤더 파일을 추가하시오.
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다	.
//
int address_mapping_table[DATABLKS_PER_DEVICE][2];
int freeblock_index;

void ftl_open()
{
	char pagebuf[PAGE_SIZE];
	char temp[4];
	int flag = FALSE;
	int k = 0;
	int temp_index = 0;
	//file 이 존재 하면
	for(int i = 0; i < BLOCKS_PER_DEVICE; i++) {
		int ppn = PAGES_PER_BLOCK * i;
		dd_read(ppn,pagebuf);
		
		memset(temp,0xFF,4);
		if (memcmp(pagebuf + SECTOR_SIZE, temp, 4)){
			flag = TRUE;
			strncpy(temp,pagebuf +SECTOR_SIZE,4);
			int lbn = atoi(temp); //없어도 될수도 있음./
			address_mapping_table[lbn][0] = lbn;
			address_mapping_table[lbn][1] = i;
		}
		else {
			freeblock_index = i;
		}
	}
	
	// file이 존재 하지 않으면,,
	// address mapping table 초기화 또는 복구
	// free block's pbn 초기화
    // address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일
	/*기존에 address_mapping_table이 없다는 가정.*/
	// file이 없을 경우에 만들어준다.
	if (!flag) {
		for (int i = 0; i < DATABLKS_PER_DEVICE ; i++) {
			address_mapping_table[i][1] = -1;
			address_mapping_table[i][0] = i;
		}
		freeblock_index = 15;
	}
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_read(int lsn, char *sectorbuf)
{
	int lbn = lsn/PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;
	int pbn = address_mapping_table[lbn][1];
	int ppn = (pbn * PAGES_PER_BLOCK) + offset;
	char pagebuf[PAGE_SIZE];

	dd_read(ppn,pagebuf);
	memcpy(sectorbuf,pagebuf,SECTOR_SIZE);

	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_write(int lsn, char *sectorbuf)
{
	int lbn, offset;
	char secotor[SECTOR_SIZE];
	char pagebuf[PAGE_SIZE];
	char spare[SPARE_SIZE];
	char temp[4];
	memset(temp,0xFF,4);
	lbn = lsn/PAGES_PER_BLOCK; // 몇번째 블록?
	offset = lsn % PAGES_PER_BLOCK; // 블록에서 몇번째 page?
	char tlbn[4];
	sprintf(tlbn,"%d",lbn);
	char tlsn[4];
	sprintf(tlsn,"%d",lsn);
	int pbn = address_mapping_table[lbn][1]; // data를 어떤 ppn에 써야 할지 결정.

	if (pbn == -1) { // 해당 pbn에 data가 최초로 쓰인다는 의미. 즉 lbn과 매칭된 pbn이 없다.
		for (int i = 0; i < BLOCKS_PER_DEVICE; i++) { // 비어있는 pbn을 실제 블럭과 매칭시켜줘야 한다.
			if(i == freeblock_index) {
				continue;
			}
			int first_ppn =  i * PAGES_PER_BLOCK;	// 각 블록의 첫번째 page_num을 구한다.
			dd_read(first_ppn, pagebuf);	
			if (!memcmp(pagebuf+SECTOR_SIZE, temp, 4)){ // read를 통해 살펴보면서 freeblock을 찾는다.
				address_mapping_table[lbn][1] = i; // 해당 블럭을 배정한다.
			
				break;	// i is 피지컬 블럭 넘버.
			}
		} //lbn과 pbn이 매칭은 된 상황.
		// offset은 블럭기준.

		pbn = address_mapping_table[lbn][1];
	
		int ppn =  (pbn * PAGES_PER_BLOCK) + offset; //내가 요청받은 data를 작성할 ppn
		 // spare는 그냥 문자열로 저장하지 않아도 되나?
		// sectorbuf + spare(spare의 앞 4b는 lbn 뒤 4b 는 lsn) = pagebuf;
		// 앞에 비우고 spare에만 lbn넣어주기.
		//printf("pbn*PAGES_PER_BLOCK %d\n",pbn*PAGES_PER_BLOCK);
		dd_read(pbn*PAGES_PER_BLOCK, pagebuf);
		memcpy(pagebuf+SECTOR_SIZE, tlbn, sizeof(tlbn));
		dd_write(pbn *PAGES_PER_BLOCK,pagebuf);
		//spare = lbn; // + lsn
		dd_read(ppn,pagebuf);
		memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
		memcpy(pagebuf+SECTOR_SIZE,tlbn,sizeof(tlbn));
		memcpy(pagebuf+SECTOR_SIZE + 4,tlsn,sizeof(tlsn));
		dd_write(ppn,pagebuf);
	}
	else { // lbn이 pbn과 매칭이되어있다.
		int ppn = (pbn * PAGES_PER_BLOCK) + offset;
		dd_read(ppn,pagebuf);

		strncpy(spare,pagebuf + SECTOR_SIZE,16);
	
		// 0보다 같거나 큰 정수가 나오면 갱신. 아니면 그냥 쓰기.
		if (memcmp(spare+4,temp,4)) {
			int post_pbn;
			post_pbn = freeblock_index; // 해당 블럭을 배정한다.

			for (int i = 0; i < PAGES_PER_BLOCK; i++){
				if(i == offset){
					continue;
				}
				else {
					dd_read ((pbn * PAGES_PER_BLOCK) + i,pagebuf);
					dd_write ((post_pbn * PAGES_PER_BLOCK) + i,pagebuf);
				}
			} // 옴기는것 끝.
			int post_ppn = (post_pbn * PAGES_PER_BLOCK) + offset;
			memcpy(spare,tlbn,4);
			strncpy(spare + 4,tlsn,4);
			memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
			strncpy(pagebuf +SECTOR_SIZE,spare,SPARE_SIZE);
			dd_write(post_ppn,pagebuf);
			

			dd_erase(pbn);
			freeblock_index = pbn;
			address_mapping_table[lbn][1] = post_pbn;
		}

		else { 
			memcpy(spare,tlbn,4);
			strncpy(spare + 4,tlsn,4);
			memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
			strncpy(pagebuf +SECTOR_SIZE,spare,SPARE_SIZE);
			dd_write(ppn,pagebuf);
		}
	}
	return;
}

void ftl_print() {
	printf("lbn pbn\n");
	for(int i = 0; i < DATABLKS_PER_DEVICE; i++){
		printf("%d %d\n",address_mapping_table[i][0],address_mapping_table[i][1]);
	}
	printf("free block's pbn = %d\n",freeblock_index);
	return;
}