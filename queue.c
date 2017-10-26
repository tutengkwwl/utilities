#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "string.h"

#define COM_BUF_SIZE		    1
#define COM_TX_BUF_COUNT        6
#define COM_RX_BUF_COUNT        3

typedef unsigned char u8;
u8 com_tx_buf[COM_BUF_SIZE * COM_TX_BUF_COUNT];
u8 com_tx_push_pos   = 0;            /* Push position, The queue is filled when bit7 is set */
u8 com_tx_pop_pos    = 0;            /* Read position */

u8 com_rx_buf[COM_BUF_SIZE * COM_RX_BUF_COUNT];
u8 com_rx_push_pos = 0;
u8 com_rx_pop_pos  = 0;






u8* pop_queue_dat(u8 *p_dat)
{
    u8 *ret = NULL;
    u8 *push_pos, *pop_pos;
    u8 buf_count;

    if(p_dat == com_rx_buf)
    {
        push_pos  = &com_rx_push_pos;
        pop_pos   = &com_rx_pop_pos;
        buf_count = COM_RX_BUF_COUNT;
    }
    else if(p_dat == com_tx_buf)
    {
        push_pos  = &com_tx_push_pos;
        pop_pos   = &com_tx_pop_pos;
        buf_count = COM_TX_BUF_COUNT;
    }
    else{
        return ret;
    }
    
    if(*push_pos == *pop_pos)
    {
        return ret;
    }

    /* Queue is filled */
    ret = p_dat + (*pop_pos) * COM_BUF_SIZE;
    (*pop_pos) += 1;
    
    if(*pop_pos >= buf_count)
    {
        *pop_pos = 0;
    }

    /* Not full now */
    if(*push_pos & 0x80)
    {
        *push_pos &= 0x7f;
    }

    return ret;
}



int push_queue_dat(u8 *p_dst, u8 *p_dat, u8 size)
{
    u8 *push_pos, *pop_pos;
    u8 buf_count;

    if(p_dst == com_rx_buf)
    {
        push_pos  = &com_rx_push_pos;
        pop_pos   = &com_rx_pop_pos;
        buf_count = COM_RX_BUF_COUNT;
    }
    else if(p_dst == com_tx_buf)
    {
        push_pos  = &com_tx_push_pos;
        pop_pos   = &com_tx_pop_pos;
        buf_count = COM_TX_BUF_COUNT;
    }
    else
    {
        return 0;
    }

    if(size > buf_count)
        return 0;

    if((*push_pos) & 0x80)
    {
        /* Current is full */
        u8 tmp = *push_pos & 0x7f;
        
        memcpy(p_dst + tmp * COM_BUF_SIZE, p_dat, size);
        
        tmp++;
        
        if(tmp >= buf_count)
        {
            tmp = 0;
        }
        *pop_pos = tmp;
        *push_pos = tmp | 0x80;
    }
    else
    {
        /* Not full */
        u8 tmp = *push_pos;

        memcpy(p_dst + tmp * COM_BUF_SIZE, p_dat, size);

        tmp++;
        if(tmp >= buf_count)
        {
            tmp = 0;
        }
        
        if(tmp == *pop_pos)
        {
            tmp |= 0x80;
        }
        *push_pos = tmp;
    }
    
    
    return 1;
}




int main()
{
	char ch;
	u8 dat[1];
	u8 *p_dat;
	int i;
	while(1)
	{
		ch = getch();
		
		if(ch == '.')
		{
			dat[0]++;
			printf("push [%02x] to: ", dat[0]);
			push_queue_dat(com_rx_buf, dat, 1);
			for(i = 0; i < COM_RX_BUF_COUNT; i++)
			{
				printf("%02x ", com_rx_buf[i]);
			}
		}
		else if(ch == ',')
		{
			p_dat = pop_queue_dat(com_rx_buf);
			if(!p_dat){
				printf("Pop out: NULL\r\n");
			}
			else{
				printf("Pop out: %02x\r\n", *p_dat);
			}
		}
		
		
		printf("\r\n");	
	}
	
	
	
	
	return 0;
}


