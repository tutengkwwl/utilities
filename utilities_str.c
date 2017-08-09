/*******************************************************************************

 * File Name     : utilities_str.c
 * Author        : Soul
 * Date          : 2017-08-09
 * Description   : .C file function description
 * Version       : 1.0
 * 
 * Record        :
 * Modification  : Created file // 2017-08-09

*******************************************************************************/



#include "stdint.h"
#include "stdio.h"
#include "string.h"


uint32_t is_hex_char(uint8_t ch, uint8_t *p_hex)
{
    if(ch >= '0' && ch <='9')
    {
        *p_hex = ch - '0';
        return 1;
    }
    else if(ch >= 'a' && ch <='f')
    {
        *p_hex = ch - 'a' + 0x0a;
        return 1;
    }
    else if(ch >= 'A' && ch <='F')
    {
        *p_hex = ch - 'A' + 0x0a;
        return 1;
    }
    
    *p_hex = 0x00;
    return 0;
}





uint32_t str_to_hex(uint8_t *p_str, uint8_t *p_hex_buf)
{
    uint32_t index, pos;
    uint8_t  tmp;

    // 4-Bits position
    pos = 0;
    
    for(index = 0;; index++)
    {
        if(is_hex_char(p_str[index], &tmp))
        {
            // put 4-bits to p_hex_buf 'queue'
            if(pos % 2)
            {
                // Low 4-bits
                p_hex_buf[pos / 2] |= (tmp & 0x0f);
            }
            else
            {
                // high 4-bits
                p_hex_buf[pos / 2] = 0x00;
                p_hex_buf[pos / 2] |= ((tmp << 4) & 0xf0);
            }
            pos++;
        }
        else
        {
            // Not a hex character
            if(pos % 2)
            {
                // One byte not generate, only has high 4-bits.
                // So, switch high 4-bits to low.
                p_hex_buf[pos / 2] = (p_hex_buf[pos / 2] >> 4) & 0x0f;
                pos++;
            }

            if('\0' == p_str[index])
            {
                break;
            }
        }
    }

    return (pos + 1) / 2;
}


int main(void)
{
    uint8_t buf[1024];
    uint8_t hex[1024];

    int len, pos;
    
L_MAIN_LOOP:

    memset(buf, 0x00, 1024);
    pos = 0;
    printf(">");
L_LOOP_INNER:
    
    scanf("%c", &buf[pos]);
    if(buf[pos] != '\n')
    {
        pos++;
        goto L_LOOP_INNER;
    }

    len = str_to_hex(buf, hex);
    printf("Data:");
    int i = 0;
    for(; i < len; i++)
    {
        printf("%02x ", hex[i]);
    }
    printf("\r\n");


    goto L_MAIN_LOOP;
}

