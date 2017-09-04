/*******************************************************************************
 * File Name     : bin-gen-c.cpp
 * Author        : Soul (tutengkwwl@126.com or tutengkwwl@outlook.com)
 * Date          : 2017-08-17
 * Description   : .C file function description
 * Version       : 1.0
 * 
 * Record        :
 * Modification  : Created file // 2017-08-17

*******************************************************************************/
#include "stdio.h"
#include "conio.h"
#include "string.h"
#include "malloc.h"
#include "stdint.h"

#define VERSION		"0.r1"

#define EVERY_LINE_HEX_NUM      16
#define RW_BUF_LEN              1024

/* CRC16 From https://github.com/lireric/crc16 
 * Author: eric, based on sources from http://www.acooke.org/cute/16bitCRCAl0.html
 */

typedef uint16_t bit_order_16(uint16_t value);
typedef uint8_t bit_order_8(uint8_t value);


uint16_t straight_16(uint16_t value) {
    return value;
}

uint16_t reverse_16(uint16_t value) {
    uint16_t reversed = 0;
    int i;
    for (i = 0; i < 16; ++i) {
        reversed <<= 1;
        reversed |= value & 0x1;
        value >>= 1;
    }
    return reversed;
}

uint8_t straight_8(uint8_t value) {
    return value;
}

uint8_t reverse_8(uint8_t value) {
    uint8_t reversed = 0;
    int i;
    for (i = 0; i < 8; ++i) {
        reversed <<= 1;
        reversed |= value & 0x1;
        value >>= 1;
    }
    return reversed;
}

uint16_t crc16common(uint8_t const *message, int nBytes,
    bit_order_8 data_order, bit_order_16 remainder_order,
    uint16_t remainder, uint16_t polynomial) 
{
	int byte;
	uint8_t bit;
    for (byte = 0; byte < nBytes; ++byte) {
        remainder ^= (data_order(message[byte]) << 8);
        for (bit = 8; bit > 0; --bit) {
            if (remainder & 0x8000) {
                remainder = (remainder << 1) ^ polynomial;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder_order(remainder);
}

/*
uint16_t crc16ccitt(uint8_t const *message, int nBytes)
{
    return crc16common(message, nBytes, straight_8, straight_16, 0xffff, 0x1021);
}


uint16_t crc16ccitt_kermit(uint8_t const *message, int nBytes)
{
    uint16_t swap = crc16common(message, nBytes, reverse_8, reverse_16, 0x0000, 0x1021);
    return swap << 8 | swap >> 8;
}

uint16_t crc16x25(uint8_t const *message, int nBytes) 
{
    uint16_t swap = crc16common(message, nBytes, reverse_8, reverse_16, 0x0FFFF, 0x1021);
    return ~(swap);
}

uint16_t crc16ccitt_1d0f(uint8_t const *message, int nBytes)
{
    return crc16common(message, nBytes, straight_8, straight_16, 0x1d0f, 0x1021);
}

uint16_t crc16ibm(uint8_t const *message, int nBytes)
{
    return crc16common(message, nBytes, reverse_8, reverse_16, 0x0000, 0x8005);
}

*/


uint16_t crc16ccitt_xmodem(uint8_t const *message, int nBytes)
{
    return crc16common(message, nBytes, straight_8, straight_16, 0x0000, 0x1021);
}





int main(int argc,char**argv)
{
	FILE *fp_bin, *fp_c;
	int file_size, pos, len, w_len, i;
    unsigned char *p_mem;
    char rw_buf[RW_BUF_LEN];
    
    printf("COPYRIGHT (C) 2017 WE ALL  VER(%s) bin.C File AUTO GEN \r\n", VERSION);
    
	if(argc == 1)
	{
		printf("ERR: Shall drag bin-file to this bin-gen-c.exe\r\n");
		getch();
		return -1;
	}


    if(!memcmp(argv[1] + strlen(argv[1]) - 4, ".bin", 4) ||\
       !memcmp(argv[1] + strlen(argv[1]) - 4, ".Bin", 4) ||\
       !memcmp(argv[1] + strlen(argv[1]) - 4, ".BIN", 4))
    {
        
    }
    else
    {
        printf("ERR: Invalid file type: %s\r\n", (argv[1] + strlen(argv[1]) - 4));
        getch();
        return - 1;
    }
	
	
	/* Open bin file */
    fp_bin = fopen(argv[1], "rb");
    
	if(!fp_bin)
	{
	    printf("open file: %s fail\r\n", argv[1]);
        getch();
		return -1;
	}

    /* Copy bin file to RAM */
    fseek(fp_bin, 0, SEEK_END);
    file_size = ftell(fp_bin);

    if(!file_size)
    {
        printf("ERR: File is empty\r\n", file_size);

        fclose(fp_bin);
        getch();
        return -1;
    }
    printf("File size: %d\r\n", file_size);
    p_mem = (unsigned char *)malloc(file_size);
    if(!p_mem)
    {
        printf("ERR: No enough RAM\r\n");
        fclose(fp_bin);
        getch();
        return -1;
    }
    fseek(fp_bin, 0, SEEK_SET);
    
    if(fread(p_mem, 1, file_size, fp_bin) != file_size)
    {
        printf("ERR: Read bin file fail\r\n");
        fclose(fp_bin);
        free(p_mem);
        getch();
        return -1;
    }
    else
    {
        fclose(fp_bin);
    }

    memset(rw_buf, 0x00, RW_BUF_LEN);
    
    sprintf(rw_buf, ".\\firmware_CRC(0x%04x).c", crc16ccitt_xmodem(p_mem, file_size));
    
    /* Open .C file and save it */
    fp_c = fopen(rw_buf, "w+");

	if(!fp_c)
	{
	    printf("open file: %s fail\r\n", rw_buf);
        getch();
        free(p_mem);
		return -1;
	}
    fseek(fp_c, 0, SEEK_SET);
    
    memset(rw_buf, 0x00, RW_BUF_LEN);

    sprintf(rw_buf, "/* COPYRIGHT (C) 2017 WE ALL  VER(%s) bin.C File AUTO GEN */\n", VERSION);
    fwrite(rw_buf, 1, strlen(rw_buf), fp_c);

    memset(rw_buf, 0x00, RW_BUF_LEN);
    sprintf(rw_buf, "const unsigned char fw[%d] = \n{\n", file_size);
    fwrite(rw_buf, 1, strlen(rw_buf), fp_c);
    pos = 0;
    
    while(1)
    {
        len = file_size - pos;

        if(len > EVERY_LINE_HEX_NUM)
        {
            len = EVERY_LINE_HEX_NUM;
        }

        memset(rw_buf, 0x00, RW_BUF_LEN);
        w_len = 0;
        w_len += sprintf(rw_buf + w_len, "\t");
        for(i = 0; i < len; i++)
        {
            w_len += sprintf(rw_buf + w_len, "0x%02x, ", p_mem[i + pos]);
        }

        if(pos + len >= file_size)
        {
            /* End of the bin file */
            rw_buf[w_len - 2] = ' ';
            w_len += sprintf(rw_buf + w_len, " /* [%08x] */\n", pos);
        }
        else
        {
            w_len += sprintf(rw_buf + w_len, " /* [%08x] */\n", pos);
        }
        
        fwrite(rw_buf, 1, strlen(rw_buf), fp_c);

        pos += len;

        if(pos >= file_size)
        {
            break;
        }
    }
    memset(rw_buf, 0x00, RW_BUF_LEN);
    sprintf(rw_buf, "};\n\n");
    fwrite(rw_buf, 1, strlen(rw_buf), fp_c);
    
    fclose(fp_c);
    free(p_mem);
    
    printf("Change done\r\n");
	return 0;
}

