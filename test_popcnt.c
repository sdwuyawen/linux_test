/**
   @filename :
		test_popcnt.c
**/
#include <stdio.h>
#include <stdint.h>

uint32_t pop_count(uint32_t u)
{
	uint32_t u1;
	
	printf("0 : 0x%x\n", u);
	u1 = u;
    u = ((u&0x55555555) + ((u>>1)&0x55555555)); /* bit(0) + bit(1)>>1 */
	
	printf("1 : 0x%x : 0x%x, 0x%x\n", u, (u1&0x55555555), ((u1>>1)&0x55555555));
	u1 = u;
    u = ((u&0x33333333) + ((u>>2)&0x33333333)); /* bit(0~1) + bit(2~3)>>2 */
	
	printf("2 : 0x%x : 0x%x, 0x%x\n", u, (u1&0x33333333), ((u1>>2)&0x33333333));
	u1 = u;
    u = ((u&0x0f0f0f0f) + ((u>>4)&0x0f0f0f0f)); /* bit(0~3) + bit(4~7)>>4 */
	
	printf("3 : 0x%x : 0x%x, 0x%x\n", u, (u1&0x0f0f0f0f), ((u1>>4)&0x0f0f0f0f));
	u1 = u;
    u = ((u&0x00ff00ff) + ((u>>8)&0x00ff00ff)); /* bit(0~7) + bit(8~15)>>8 */
	
	printf("4 : 0x%x : 0x%x, 0x%x\n", u, (u1&0x00ff00ff), ((u1>>8)&0x00ff00ff));
	u1 = u;
    u = ((u&0x0000ffff) + (u>>16)); /* bit(0~15) + bit(16~31)>>16 */
	
	printf("5 : 0x%x : 0x%x, 0x%x\n", u, (u1&0x0000ffff), (u1>>16));
	
    return u;
}


int main(int argc, char *argv[])
{
	uint32_t data;

	if (argc < 2) {
		return -1;
	}

	data = strtoul(argv[1], NULL, 16);

	pop_count(data);
	
    return 0;
}

