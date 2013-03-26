/**
   @filename :
   		test_eeprom.c
**/
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/**
 **/
#define EEPROM_NAME_STR	"/sys/bus/i2c/devices/1-0050/eeprom"

#define EEPROM_BUF_SIZE	256

#define BYTES_PER_LINE	8

/**
 **/
int main(int argc, char *argv[]) {
	int fd;
	int ret;
	uint8_t buffer[EEPROM_BUF_SIZE];
	uint8_t wr_buf[EEPROM_BUF_SIZE];
	int is_wr = 0;
	int i;

	if (argc > 1) {
		is_wr = 1;
	}
	fd = open(EEPROM_NAME_STR, O_RDWR);
	if (fd < 0) {
		printf("Can not open %s\n", EEPROM_NAME_STR);
		return -1;
	}

	if (is_wr) {
		/* write */
		memset(wr_buf, 0, EEPROM_BUF_SIZE);
		for (i=0; i<EEPROM_BUF_SIZE; i++) {
			wr_buf[i] = i;
		}
		ret = write(fd, wr_buf, EEPROM_BUF_SIZE);
		printf("write ret = %d\n", ret);
	} else {
		/* read */
		lseek(fd, 0, 0);
		ret = read(fd, buffer, EEPROM_BUF_SIZE);
		printf("read ret = %d\n", ret);
		if (ret) {
			int i;

			for (i=0; i<ret; i++) {
				if (i%BYTES_PER_LINE == 0) {
					printf("\n");
				}
				printf("0x%x, ", buffer[i]);
			}
			printf("\n");
		}
	}

	close(fd);
    
    return 0;
}

