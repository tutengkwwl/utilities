#include "stdio.h"
#include "string.h"


/* 条件： 一任意长数组，从index 0到 index n均已写入非0 
   功能： 该代码可快速找到最开始未写入0的地方
   测试： 目前测试一切正常，测试过1000个元素的数组 
    */ 

#define RECORD_SIZE		120
#define UN_SAVE_START_ADDR  1
#define DEBUG_INFO //	printf

typedef unsigned long u32;
typedef unsigned char u8;

u8  data_x[RECORD_SIZE + 1];

typedef struct
{
    u32 call_times;
    u32 addr_start;
    u32 ele_cur_pos;
    u32 ele_size;
    u32 ele_num;
    u32 next_addr;
    u32 addr_end;
    u8 *addr_base;
}seek_next_t;


typedef enum
{
    FMDB_ERR_SUCCESS = 0,
    FMDB_ERR_LACK_OF_FLASH_MEM,
    FMDB_ERR_INVALID_STATE,
    FMDB_ERR_INVALID_PARA,
    FMDB_ERR_HARDWARE,
}fmdb_err_t;

fmdb_err_t fmdb_seek_next_addr(seek_next_t *p_seek_next)
{
	p_seek_next->call_times++;
	
	DEBUG_INFO("---------------------->CALL fmdb_seek_next_addr\r\n");
	if(p_seek_next->call_times > 20)
	{
		DEBUG_INFO("Time over\r\n"); 
		return FMDB_ERR_INVALID_PARA;
	}
	
	DEBUG_INFO("POS: %2d\r\n", p_seek_next->addr_start + p_seek_next->ele_cur_pos * p_seek_next->ele_size);
	
	if(p_seek_next->addr_base[p_seek_next->addr_start + p_seek_next->ele_cur_pos * p_seek_next->ele_size])
	{
		/* Seek section next */
		p_seek_next->addr_start += p_seek_next->ele_cur_pos * p_seek_next->ele_size;
		
		p_seek_next->ele_num -= p_seek_next->ele_cur_pos;
		
		DEBUG_INFO("Save Left %d element, zone %d to %d\r\n", p_seek_next->ele_num, p_seek_next->addr_start, p_seek_next->addr_start + p_seek_next->ele_num - 1);
		
		if(p_seek_next->ele_num == 0)
		{	
			if(p_seek_next->addr_end >= p_seek_next->addr_start + 2 * p_seek_next->ele_size)
			{
				p_seek_next->next_addr = p_seek_next->addr_start + 2 * p_seek_next->ele_size;
				return 	FMDB_ERR_SUCCESS;
			}
			else
			{
				DEBUG_INFO("Data full 0\r\n");
				return FMDB_ERR_INVALID_PARA;
			} 
		}
		else if(p_seek_next->ele_num == 1)
		{
			if(p_seek_next->addr_base[p_seek_next->addr_start + p_seek_next->ele_size])
			{
				DEBUG_INFO("Data full 1\r\n");
				return FMDB_ERR_INVALID_PARA;
			}
			else
			{
				if(p_seek_next->addr_end > p_seek_next->addr_start + p_seek_next->ele_size)
				{
					p_seek_next->next_addr = p_seek_next->addr_start + p_seek_next->ele_size;
					return 	FMDB_ERR_SUCCESS;
				}
				return FMDB_ERR_INVALID_PARA;
			} 
		}
		
		p_seek_next->ele_cur_pos = p_seek_next->ele_num >> 1;

		return fmdb_seek_next_addr(p_seek_next);
	}

	/* Seek section before */
	p_seek_next->ele_num = p_seek_next->ele_cur_pos;
	DEBUG_INFO("Unsave Left %d element, zone %d to %d\r\n", p_seek_next->ele_num, p_seek_next->addr_start, p_seek_next->addr_start + p_seek_next->ele_num - 1);
	if(p_seek_next->ele_num == 0)
	{
		p_seek_next->next_addr = p_seek_next->addr_start;
		return FMDB_ERR_SUCCESS;
	}
	else if(p_seek_next->ele_num == 1)
	{
		if(p_seek_next->addr_base[p_seek_next->addr_start])
		{
			if(p_seek_next->addr_end > p_seek_next->addr_start)
			{
				p_seek_next->next_addr = p_seek_next->addr_start + p_seek_next->ele_size;
				return FMDB_ERR_SUCCESS;
			}
			else
			{
				DEBUG_INFO("Data full 2\r\n");
				return FMDB_ERR_INVALID_PARA;
			} 
			
		}
		else
		{
			p_seek_next->next_addr = p_seek_next->addr_start;
			return 	FMDB_ERR_SUCCESS;
		} 
	}
	p_seek_next->ele_cur_pos = p_seek_next->ele_num >> 1;
	return fmdb_seek_next_addr(p_seek_next);
}












#define TEST_CASE_VAL		40
int main()
{
	int i;
	
	seek_next_t  seek_next;
	
	for(i = 0; i < RECORD_SIZE + 1; i++)
	{
		DEBUG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test : %d\r\n", i);
		seek_next.call_times = 0;
		seek_next.addr_start = 0;
		seek_next.ele_cur_pos = RECORD_SIZE / 2;
		seek_next.ele_num = RECORD_SIZE;
		seek_next.next_addr = 0xffffffff;
		seek_next.ele_size = 1;
		seek_next.addr_end = seek_next.addr_start + seek_next.ele_num * seek_next.ele_size;
		seek_next.addr_base = data_x;
		memset(data_x, 0xff, RECORD_SIZE);
		memset(data_x + i, 0x00, RECORD_SIZE - i);
		if(fmdb_seek_next_addr(&seek_next))
		DEBUG_INFO("Seek failed\r\n");
		DEBUG_INFO("\r\n>>>>>>>>>>>>>>>>>>>>>>Unsave start addr is : %d\r\n", seek_next.next_addr);
		if(i != seek_next.next_addr)
		{
			printf("Unequle %d, %d\r\n", i, seek_next.next_addr);
		}
		else{
			printf("Equle, i %d , next_addr  %d\r\n", i, seek_next.next_addr);
		}
	}
	
}
