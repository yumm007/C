/*******************************************************
***	”√ªß≤„≤‚ ‘∫Ø ˝
*******************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mini_fs.h"
#include <stdbool.h>

#define TEST_COUNT  	80
#define BUF_SIZE		800

#define TIMER_CLOCK	32768/4

#ifdef FS_DISK_RAM_FLASH
void timer_start(void) {return;}
WORD timer_end(void) {return 1;}
#else
#include <msp430f149.h>
static int tim_over = 0;
void timer_start(void) {
	tim_over = 0;
	TAR	= 0;
	CCR0 = 0xFFFF;
	TACTL = TASSEL_1 + MC_2 + ID_2; // ACKK/4
	CCTL0 = CCIE; // CCR0 interrupt enabled
}

WORD timer_end(void) {
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
#endif

static BYTE test_str[BUF_SIZE];
static BYTE BUF[BUF_SIZE];
struct  f_test_arr_t {
 	WORD offset;
	WORD len;
};

#if defined(FS_DISK_ROM_FLASH) || defined(FS_DISK_RAM_FLASH)
#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK

extern WORD	f_addr(file_id_t id);

extern const BYTE DISK[DISK_SPACE];
void f_dump(void) {
	file_id_t i;
	int j;
	for (i = FILE1; i < FILE_ID_END; i++) {
		printf("FILE %u: addr = %lu, len = %lu, size = %lu\n", i + 1, \
			f_addr(i), f_len(i), f_size(i));
		for (j = 0; j < f_size(i); j++)
			putchar(DISK[f_addr(i) + j]);
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
			f_addr(i), f_len(i), f_size(i));
		f_read((file_id_t)(FILE1 + i), 0, BUF, f_len(i));
		for (j = 0 ; j < f_len(i); j++)
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
	file_id_t id;
	int r;
	for (id = FILE1; id < FILE_ID_END; id++) {
		r = rand();
		arr[id].offset = r % f_size(id);
		srand(r);
		r = rand();
		arr[id].len = r % f_size(id) + 1;
		srand(r);
		if (arr[id].offset > arr[id].len)
			arr[id].len = f_size(id) - arr[id].offset;
		if (arr[id].offset + arr[id].len > f_size(id))
			arr[id].len = f_size(id) - arr[id].offset;
	}		
}

bool f_test(void) {
	int i = TEST_COUNT, count = 0, failed;
	file_id_t id;
  	struct  f_test_arr_t arr[FILE_ID_END];
	WORD w_c, tim;
	float ave;
	BYTE *p;
	
	f_write(FILE4, 0, (const BYTE *)"AAAAAAA", 7);
	f_write(FILE7, 0, (const BYTE *)"AAAAAAA", 7);
	
	//printf("\n\n=================BEGIN TEST %u clock/s ==============\n", TIMER_CLOCK);
	while (i--) {
		rand_800(test_str, BUF_SIZE);
    	rand_arr(arr, FILE_ID_END);
		failed = 0;
		//printf("\n");
		
		w_c = 0;
		tim = 0;		
		
		//fprintf(stderr, "-----------A-----------\n");
		for (id = FILE1; id < FILE_ID_END; id++)
		{
		  	if (id == FILE4 || id == FILE7)
				continue;
			timer_start();
			f_write(id, arr[id].offset, test_str, arr[id].len);
			if (memcmp(f_read(id, arr[id].offset, BUF, arr[id].len), test_str, arr[id].len) != 0) {
				fprintf(stderr, "memcmp failed after write. FILE%d offset %lu, len %lu\n", id+1, arr[id].offset, arr[id].len);
			}
			tim += timer_end();
			w_c += arr[id].len;
			//fprintf(stderr, "f_write(FILE%d, addr %lu, \toffset %lu, \tlen %lu)\t\t...\n", id + 1, f_addr(id), arr[id].offset, arr[id].len);
		}
		//fprintf(stderr, "-----------Z-----------\n");
		ave = (tim / TIMER_CLOCK) / w_c * 1000.0 * 1000; 
		//printf("write %lu byte use %lu clock, average %.2fus/B\n", w_c, tim, ave);
		
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
				 fprintf(stderr, "memcmy failed. FILE%u, offset %d, len %d\n", id +1, 0, 7);
			  	 failed = 1;
				 return false;
			  } else {
			  	 ;//printf("f_read(FILE%d, \toffset %d, \tlen %d)\t\tok\n", id + 1, 0, 7);
			  }
			  continue;
		   }  
		  timer_start();
		  p = f_read(id, arr[id].offset, BUF, arr[id].len);
		  tim += timer_end();			
			if (memcmp(p, test_str, arr[id].len) != 0) {
				fprintf(stderr, "memcmy failed. FILE%u, offset %lu, len %lu\n", id +1, arr[id].offset, arr[id].len);
				failed = 1;
				w_c += arr[id].len;
				return false;
      	} else {
			   w_c += arr[id].len;
      		//fprintf(stderr, "f_read(FILE%d, \toffset %d, \tlen %d)\t\tok\n", id + 1, arr[id].offset, arr[id].len);
      	}
#else
			timer_start();
			f_read(id, arr[id].offset, BUF, arr[id].len);
			tim += timer_end();
			w_c += arr[id].len;
#endif
		}
		ave = (tim / TIMER_CLOCK) / w_c * 1000.0 * 1000;
		//printf("read %lu byte use %lu clock, average %.2fus/B\n", w_c, tim, ave);

		//f_copy ≤‚ ‘
		f_erase(FILE9);
		f_write(FILE9, 0, (const BYTE *)"12345678901234567890", 20);
		f_write(FILE9, 10, (const BYTE *)"ABCDE", 5);
		f_read(FILE9, 0, BUF, 20);
		if (memcmp(BUF, "1234567890ABCDE67890", 20) != 0) {
			fprintf(stderr, "f_copy test failed.\n");
			failed = 1;
		}

		if (!failed)
			count++;
		if (failed)
			break;
	
		//f_sync();
	}
#if 0
	tim = 0;
	timer_start();
	f_erase(FILE9);
	tim += timer_end();
	printf("\nerase %lu byte use %lu clock\n", f_size(FILE9), tim);
	tim = 0; 
	for (i = f_size(FILE9) / 32 ; i >= 0; i--) {
		//memset(BUF, 0, sizeof(BUF));
	   //printf("file 9 len = %d\n",  fs.file[FILE9].file_len);
		timer_start();
	   f_write(FILE9, f_len(FILE9), (const BYTE *)"12345678901234567890123456789012", 32);
		tim += timer_end();
		//f_read(FILE9, fs.file[FILE9].file_len - 32, BUF, 32);
		//printf("read value is %s\n", BUF);
	}
	ave = (tim / TIMER_CLOCK) / f_len(FILE9) * 1000 * 1000;
	printf("append %lu byte use %lu clock, average %.2fus/B \n", f_len(FILE9), tim, ave);
#endif
	printf("=====Test %d, pass %d, failed %d.=====\n", TEST_COUNT, count, TEST_COUNT - count);
	f_sync();
	//f_dump();

	if (count != TEST_COUNT)
		return false;
	else
		return true;
}

#ifdef FS_USE_MEM_SWAP
BYTE __FS_SWAP_SPACE[SEGMENT_SIZE];
#endif

#ifdef FS_DISK_RAM_FLASH
#include <unistd.h>

int main(void) {
	f_init();
	fprintf(stderr, "f_init comp.\n");
	f_erase(FILE1);
	f_erase(FILE1);
	f_erase(FILE1);
	f_erase(FILE1);
	return 0;
	while (f_test()) {
		sleep(1);
	}
	return 0;
}
#endif
