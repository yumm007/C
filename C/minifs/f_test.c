/*******************************************************
***	�û�����Ժ���
*******************************************************/
#include <stdio.h>
#include <string.h>
#include "mini_fs.h"
#include <stdlib.h>
#include <msp430f149.h>

#define TEST_COUNT  	8
#define BUF_SIZE		800

#define TIMER_CLOCK	32768.0/4

static int tim_over = 0;
void timer_start(void) {
	tim_over = 0;
	TAR	= 0;
	CCR0 = 0xFFFF;
	TACTL = TASSEL_1 + MC_2 + ID_2; // ACKK/4
	CCTL0 = CCIE; // CCR0 interrupt enabled
}

UINT32 timer_end(void) {
	UINT32 ret = TAR;
	TACTL = 0;
	CCTL0 = 0;
	CCTL0 = CCIE; // CCR0 interrupt enabled
	return tim_over * TIMER_CLOCK + ret;
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{ 
	TAR	= 0;
	CCR0 = 0xFFFF;
	TACTL = TASSEL_1 + MC_2 + ID_2; // ACKK/2
	tim_over++;
}

static BYTE test_str[BUF_SIZE];
static BYTE BUF[BUF_SIZE];
struct  f_test_arr_t {
 	int offset;
	int len;
};

#ifdef FS_DISK_ROM_FLASH
#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK
extern const BYTE DISK[DISK_SPACE];
void f_dump(void) {
	int i, j, n;
	n = sizeof(fs.file) / sizeof(fs.file[0]);
	for (i = 0; i < n; i++) {
		printf("FILE %d: addr = %d, len = %d, size = %d\n", i + 1, \
			fs.file[i].start_addr, fs.file[i].file_len, fs.file[i].file_size);
		for (j = 0; j < fs.file[i].file_size; j++)
			putchar(DISK[fs.file[i].start_addr + j]);
		putchar('\n');
	}
}
#endif

#ifdef FS_DISK_SPI_FLASH
void f_dump(void) {
	int i, j, n;
	n = sizeof(fs.file) / sizeof(fs.file[0]);
	for (i = 0; i < n; i++) {
		printf("FILE %d: addr = %d, len = %d, size = %d\n", i + 1, \
			fs.file[i].start_addr, fs.file[i].file_len, fs.file[i].file_size);
		f_read((file_id_t)(FILE1 + i), 0, BUF, fs.file[i].file_len);
		for (j = 0 ; j < fs.file[i].file_len; j++)
			putchar(BUF[j]);
		printf("\n");
	}
}
#endif

static void rand_800(BYTE *buf, int len) {
	int i, r;	
	for (i = 0; i < len; i++) {
		r = rand();
		buf[i] = r % 52 + 'A';
		srand(r);
	}
}

static void rand_arr(struct  f_test_arr_t *arr, int len) {
	int id, r;
	for (id = FILE1; id < FILE_ID_END; id++) {
		r = rand();
		arr[id].offset = r % fs.file[id].file_size;
		srand(r);
		r = rand();
		arr[id].len = r % fs.file[id].file_size;
		srand(r);
		if (arr[id].offset > arr[id].len)
			arr[id].len = fs.file[id].file_size - arr[id].offset;
		if (arr[id].offset + arr[id].len > fs.file[id].file_size)
			arr[id].len = fs.file[id].file_size - arr[id].offset;
	}		
}

void f_test(void) {
	int i = TEST_COUNT, count = 0, failed;
	file_id_t id;
  	struct  f_test_arr_t arr[FILE_ID_END];
	WORD w_c, tim;
	float ave;
	BYTE *p;
	
	f_write(FILE4, 0, "AAAAAAA", 7);
	f_write(FILE7, 0, "AAAAAAA", 7);
	
	printf("\n\n=================BEGIN TEST %u clock/s ==============\n", TIMER_CLOCK);
	while (i--) {
		rand_800(test_str, BUF_SIZE);
    	rand_arr(arr, FILE_ID_END);
		failed = 0;
		printf("\n");
		
		w_c = 0;
		tim = 0;		
		
		for (id = FILE1; id < FILE_ID_END; id++) {
		  	if (id == FILE4 || id == FILE7)
			  continue;
			timer_start();
			f_write(id, arr[id].offset, test_str, arr[id].len);
			tim += timer_end();
			w_c += arr[id].len;
			//printf("f_write(FILE%d, \toffset %d, \tlen %d)\t\t...\n", id + 1, arr[id].offset, arr[id].len);
		}
		ave = (tim / TIMER_CLOCK) / w_c * 1000.0 * 1000; 
		printf("write %u byte use %u clock, average %.2fus/B\n", w_c, tim, ave);
		
		w_c = 0;
		tim = 0;
		
		for (id = FILE1; id < FILE_ID_END; id++) {
#if 1
		   if (id == FILE4 || id == FILE7) {
			  w_c += 7;
			  timer_start();
			  p = f_read(id, 0, BUF, 7);
			  tim += timer_end();
			  if (memcmp(p, "AAAAAAA", 7) != 0) {
				 printf("memcmy failed. FILE%d, offset %d, len %d\n", id +1, 0, 7);
			  	 failed = 1;
			  } else {
			  	 ;//printf("f_read(FILE%d, \toffset %d, \tlen %d)\t\tok\n", id + 1, 0, 7);
			  }
			  continue;
		   }  
		  timer_start();
		  p = f_read(id, arr[id].offset, BUF, arr[id].len);
		  tim += timer_end();			
			if (memcmp(p, test_str, arr[id].len) != 0) {
				printf("memcmy failed. FILE%d, offset %d, len %d\n", id +1, arr[id].offset, arr[id].len);
				failed = 1;
				w_c += arr[id].len;
      	} else {
			   w_c += arr[id].len;
      		//printf("f_read(FILE%d, \toffset %d, \tlen %d)\t\tok\n", id + 1, arr[id].offset, arr[id].len);
      	}
#else
			timer_start();
			f_read(id, arr[id].offset, BUF, arr[id].len);
			tim += timer_end();
			w_c += arr[id].len;
#endif
		}
		ave = (tim / TIMER_CLOCK) / w_c * 1000.0 * 1000;
		printf("read %u byte use %u clock, average %.2fus/B\n", w_c, tim, ave);
		if (!failed)
			count++;
	
		f_sync();
	}
#if 1
	tim = 0;
	timer_start();
	f_erase(FILE9);
	tim += timer_end();
	printf("\nerase %u byte use %u clock\n", fs.file[FILE9].file_size, tim);
	tim = 0; 
	for (i = fs.file[FILE9].file_size / 32 ; i >= 0; i--) {
		//memset(BUF, 0, sizeof(BUF));
	   //printf("file 9 len = %d\n",  fs.file[FILE9].file_len);
		timer_start();
	   f_write(FILE9, fs.file[FILE9].file_len, "12345678901234567890123456789012", 32);
		tim += timer_end();
		//f_read(FILE9, fs.file[FILE9].file_len - 32, BUF, 32);
		//printf("read value is %s\n", BUF);
	}
	ave = (tim / TIMER_CLOCK) / fs.file[FILE9].file_len * 1000 * 1000;
	printf("append %d byte use %u clock, average %.2fus/B \n", fs.file[FILE9].file_len, tim, ave);
#endif
	printf("\n=====Test %d, pass %d, failed %d.=====\n\n", TEST_COUNT, count, TEST_COUNT - count);
	f_sync();
	//f_dump();
}
