/**
   @filename :
   test_i2c.c
**/
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/ioctl.h>
#include <linux/rtc.h>
#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#include "i2c-dev.h"

#define TIMEOUT        	4
#define RETRY         	2
#define READ_ONLY    	1

#define I2C_DEFAULT_TIMEOUT        	1
#define I2C_DEFAULT_RETRY        	2

#define I2C_M_WR        			0                //定义写标志
#define MAX_MSG_NR        			2                //根据AT24C02手册, 最大消息数为2
#define EEPROM_BLOCK_SIZE    		256              //每个block容量256 bytes
#define EEPROM_PAGE_SIZE    		16               //用页写，AT24C16页大小为16字节
#define I2C_MSG_SIZE         		(sizeof(struct i2c_msg))

typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned int            u32;
typedef unsigned long long      u64;
typedef signed char             s8;
typedef short                   s16;            
typedef int                     s32;
typedef long long               s64;

static unsigned bcd2bin(unsigned char val);

static unsigned bcd2bin(unsigned char val);

static int
__i2c_send(int fd, struct i2c_rdwr_ioctl_data *data);

static int
__i2c_set(int fd, unsigned int timeout, unsigned int retry);

static int
i2c_read_data(u16 addr, u16 offset, u8 *val);

static int
i2c_write_data(u16 addr, u16 offset, u8 val);

static int
i2c_open(unsigned char* dev, unsigned int timeout, unsigned int retry);

static int fd ;
static int addr = 0x50;  // EEPROM addr
static int offset;
static int offset_end;

static int
i2c_read_data(u16 addr, u16 offset, u8 *val)
{
    int i,ret = 0;
	u16 addr_pad;
	u16 off;

    struct i2c_rdwr_ioctl_data *data;

    if ((data = (struct i2c_rdwr_ioctl_data *)malloc(sizeof(struct i2c_rdwr_ioctl_data))) == NULL)
		return -1;

    data->nmsgs = 2;
    if ((data->msgs = (struct i2c_msg *)malloc(data->nmsgs * sizeof(struct i2c_msg))) == NULL) {
        ret = -1;
        goto errexit3;
    }
    if ((data->msgs[0].buf = (unsigned char *)malloc( EEPROM_PAGE_SIZE + 1 )) == NULL) {
        ret = -1;
        goto errexit2;
    }
    if ((data->msgs[1].buf = (unsigned char *)malloc( EEPROM_PAGE_SIZE + 1 )) == NULL) {
        ret = -1;
        goto errexit1;
    }

	addr_pad = offset/256;
	off = offset%256;
	addr |= addr_pad;

    data->msgs[0].addr   = addr;
    data->msgs[0].flags  = I2C_M_WR;
    data->msgs[0].len    = 1;
    data->msgs[0].buf[0] = off;

    data->msgs[1].addr   = addr;
    data->msgs[1].flags  = I2C_M_RD;
    data->msgs[1].len    = 1;   
    data->msgs[1].buf[0] = 0;

    if ((ret = __i2c_send(fd, data)) < 0)   
        goto errexit0;


    for(i = 0 ;i < data->msgs[1].len; i++)
        val[i] = data->msgs[1].buf[i];

errexit0:
    free(data->msgs[1].buf);
errexit1:
    free(data->msgs[0].buf);
errexit2:
    free(data->msgs);
errexit3:
    free(data);

    return ret;
}

static int
i2c_write_data(u16 addr, u16 offset, u8 val)
{
    int ret = 0;
	u16 addr_pad;
	u16 off;

    struct i2c_rdwr_ioctl_data *data;

    if ((data = (struct i2c_rdwr_ioctl_data *)malloc(sizeof(struct i2c_rdwr_ioctl_data))) == NULL)
        return -1;

    data->nmsgs = 1;
    if ((data->msgs = (struct i2c_msg *)malloc(data->nmsgs * sizeof(struct i2c_msg))) == NULL) {
        ret = -1;
        goto errexit2;
    }
    if ((data->msgs[0].buf = (unsigned char *)malloc( 2 * (EEPROM_PAGE_SIZE + 1))) == NULL) {
        ret = -1;
        goto errexit1;
    }

	addr_pad = offset/256;
	off = offset%256;
	addr |= addr_pad;

    data->msgs[0].addr   = addr;
    data->msgs[0].flags  = 0;
    data->msgs[0].len    = 2;
    data->msgs[0].buf[0] = off;
    data->msgs[0].buf[1] = val;

    if ((ret = __i2c_send(fd, data)) < 0){
        ret = -1;
        goto errexit0;
    }

errexit0:
    free(data->msgs[0].buf);
errexit1:
    free(data->msgs);
errexit2:
    free(data);

    return ret;
}

static int
i2c_open(unsigned char* dev, unsigned int timeout, unsigned int retry)
{
    if ((fd = open(dev, O_RDWR)) < 0)
        return fd;
    __i2c_set(fd, timeout, retry);

    return fd;
}

static int
__i2c_send(int fd, struct i2c_rdwr_ioctl_data *data)
{
    if (fd < 0)
        return -1;

    if (data == NULL)
        return -1;

    if (data->msgs == NULL || data->nmsgs == 0)
        return -1;
   
    return ioctl(fd, I2C_RDWR, (unsigned long)data) ;
}

static int
__i2c_set(int fd, unsigned int timeout, unsigned int retry)
{
    if (fd == 0 )
        return -1;

    ioctl(fd, I2C_TIMEOUT, timeout ? timeout : I2C_DEFAULT_TIMEOUT);
    ioctl(fd, I2C_RETRIES, retry ? retry : I2C_DEFAULT_RETRY);
   
    return 0;
}

static void
i2c_close(int fd)
{
    if (fd < 0)
        return;

    close(fd);
}

static unsigned bcd2bin(unsigned char val)
{      
	return (val & 0x0f) + (val >> 4) * 10; 
}

static unsigned char bin2bcd(unsigned val)
{      
	return ((val / 10) << 4) + val % 10;
}

inline int read_data(u16 addr, u16 offset, u8 *val){
    int ret;
   
    ret = i2c_read_data(addr,offset,val);
    if(ret < 0){
        printf("%s error!\n",__FUNCTION__);
        exit(-1);
    }
	
    return 0;
}

inline int write_data(u16 addr, u16 offset, u8 val){
    int ret;
   
    ret = i2c_write_data(addr,offset,val);
    if(ret < 0){
        printf("%s:error = %s\n",__FUNCTION__,strerror(errno));
        exit(-1);
    }
    usleep(10000);    // 重要！
	
    return 0;
}

static int eep_write(u16 addr, u16 offset, char* argv){
    //u8 data = (u8)strtoul(argv,0,16);

    int i,j,len    = 0;
    char* data    = argv;
   
    while( *data++ ){
        len++;
    }
    data = argv;

    printf("len = %d\n",len);

    for( i = offset, j = 0; i < offset + len; i++, j++ ){
        write_data(addr,i,data[j]);
    }
	
    printf("write success,data = %s\n",argv);
	
    return 0;
}

static int eep_read(u16 addr, u16 offset_start, u16 offset_end, int flags){
   
    int i;
    u8 buf[256] = {0};

    if( flags ){
        read_data(addr,offset_end,&buf[0]);
        printf("read success,data[%d] = %c\n",offset,buf[0]);
    }else{
        printf("read success,data = ");
        for(i = offset_start ; i <= offset_end ; i++){       
            read_data(addr,i,&buf[i]);       
            printf("%c",buf[i]);   
        }
        printf("\n");
    }   
   
    return 0;
}

static void error_info(void){
    printf("cmd error! ");
    printf("Please input --help or -h  for help information\n");
    exit(-1);
}   

static void help_info(void){
	printf("\nUsage: eep OPTIONS OFFSET [OFFSET_END] [DATA]\n");
	printf("\nDisplay the contents of EEPROM ,or write DATA to EEPROM\n");
	printf("\nOptions:\n");
	printf("\n\t-r OFFST \t\t\t\tDisplay the contents of EEPROM from 0x00 to OFFSET\n");
	printf("\n\t-r OFFSET OFFSET_END\t\t\tDisplay the contents of EEPROM from OFFSET to OFFSET_END\n");
	printf("\n\t-s OFFSET DATA\t\t\t\twrite DATA to EEPROM form OFFSET\n");
    printf("\n\t-h or --help\t\t\t\tshow help information\n");
    printf("\nOFFSET\tFORMAT: 0xXX  0xXX can change from 00 to ff\n");
    printf("\nFOR EXAMPLE:\n");
    printf("\n\teep -r  0x08\t\t\t\tDisplay the contents of EEPROM from 0x00 to 0x08\n");
    printf("\n\teep -ro 0x08\t\t\t\tDisplay the contents of EEPROM from 0x08\n");
    printf("\n\teep -r  0x02 0x80\t\t\tDisplay the contents of EEPROM from address 0x20 to x080\n");
    printf("\n\teep -s  0x04 \"hello world!\"\t\tWrite \"hello world!\" to EEPROM from 0x04\n\n");
}

static int i2c_path(char * argv){

    fd = i2c_open(argv,TIMEOUT,RETRY);
    if( fd < 0 ){   
        printf("i2c_open error!\n");
        exit(-1);
    }   
    return 0;
}

static int i2c_offs(char *argv){
    return (u16)strtoul(argv,0,16);
}

#define TEST_I2C_PATH	"/dev/i2c-0"

int main(int argc,char* argv[]){

    int flags;
   
    switch(argc) {
    case 2 :
		if(!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")){
			help_info();
		}else
            error_info();
		break;
	case 3 :
        if(!strcmp(argv[1],"-r" )|| !strcmp(argv[1],"-ro" )){
            flags    = (!strcmp(argv[1],"-ro")) ? READ_ONLY : 0 ;
			i2c_path(TEST_I2C_PATH);
            offset    = i2c_offs(argv[2]);   
            eep_read(addr,0,offset,flags);
        }else
            error_info();   
		break;
	case 4 :
        if(!strcmp(argv[1],"-r")){
			i2c_path(TEST_I2C_PATH);
            offset        = i2c_offs(argv[2]);   
            offset_end    = i2c_offs(argv[3]);   
            eep_read(addr,offset,offset_end,0);
        }else if(!strcmp(argv[1],"-s")){
            i2c_path(TEST_I2C_PATH);
            offset        = i2c_offs(argv[2]);
            eep_write(addr,offset,argv[3]);   
        }else
            error_info();   
		break;
	case 5 :
		if(!strcmp(argv[1],"-d")){
			i2c_path(argv[2]);
			if(!strcmp(argv[3],"-r") || !strcmp(argv[3],"-ro")){
				i2c_offs(argv[4]);
                flags   = (!strcmp(argv[3],"-ro")) ? READ_ONLY : 0 ;
				offset  = i2c_offs(argv[4]);   
				eep_read(addr,0,offset,flags);
            }else
                error_info();
		}else
            error_info();
		break;
	case 6 :
		if(!strcmp(argv[1],"-d")){
			i2c_path(argv[2]);
			if(!strcmp(argv[3],"-r")){
				offset        = i2c_offs(argv[4]);
                offset_end    = i2c_offs(argv[5]);
                eep_read(addr,offset,offset_end,0);
			}else if(!strcmp(argv[3],"-s")){
                offset        = i2c_offs(argv[4]);
                eep_write(addr,offset,argv[5]);
            }else
                error_info();
		}else
            error_info();   
		break;
		
	default:
		error_info();   
	}

    return 0;
}
