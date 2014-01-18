/* i2c_api.h
 * Author: fsysky
 * mail: fsysky@163.com
 */
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

//======================================================================================================
int IIC_Open(char *dev);
int IIC_Close(void);
int IIC_Write(int chip_addr, int addr, int data);
int IICBurstRead(int chip_addr, int start_addr, int end_addr, void *data);
int IIC_Read(int chip_addr, int addr);

//======================================================================================================
static int fd = -1;

int IIC_Open(char *dev)
{

        fd = open(dev, O_RDWR);
        if (fd < 0) {
                printf("open %s failed\n", dev);
                exit(1);
        }
        return 0;
}

int IIC_Close(void)
{
        return close(fd);
}

int IIC_Write(int chip_addr, int addr, int data)
{
        struct i2c_msg msgs[1];
        struct i2c_rdwr_ioctl_data ioctl_data;
        int ret;
        __u8 buf[2];

        buf[0] = addr;
        buf[1] = data;
        msgs[0].addr = chip_addr;
        msgs[0].flags = 0;
        msgs[0].len = 2;
        msgs[0].buf = buf;

        ioctl_data.nmsgs = 1;
        ioctl_data.msgs = msgs;
        ret = ioctl(fd, I2C_RDWR, &ioctl_data);
        if (ret < 0) {
                printf("%s: ioctl return: %d\n", __func__, ret);
        }

        return ret;
}

int IICBurstRead(int chip_addr, int start_addr, int end_addr, void *data)
{
        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data ioctl_data;
        int ret;

        msgs[0].addr = chip_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = data;
        ((__u8 *)data)[0] = start_addr;
        msgs[1].addr = chip_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = end_addr-start_addr+1;
        msgs[1].buf = data;
        ioctl_data.nmsgs = 2;
        ioctl_data.msgs = msgs;
        ret = ioctl(fd, I2C_RDWR, &ioctl_data);
        if (ret < 0) {
                printf("%s: ioctl return: %d\n", __func__, ret);
        }

        return ret;
}

int IIC_Read(int chip_addr, int addr)
{
        __u8 value = -1;

        IICBurstRead(chip_addr, addr, addr, &value);

        return value;
}

/*
* 请注意确认 IC 所挂的 I2C 总线编号，不能搞错。
*/
#define IIC_BUS "/dev/i2c-0"

int main(int argc, char *argv[])
{
        int ret;
        int reg, value, addr;
        IIC_Open(IIC_BUS);

        if (argc == 4) {
                addr = strtoul(argv[1], 0, 0);
                reg = strtoul(argv[2], 0, 0);
                value = strtoul(argv[3], 0, 0);
                IIC_Write(addr, reg, value);
                printf("addr: %x set 0x%x = 0x%x -> 0x%x\n", addr, reg, value, IIC_Read(addr, reg));
        }
        if (argc == 3) {
                addr = strtoul(argv[1], 0, 0);
                reg = strtoul(argv[2], 0, 0);
                printf("addr: %x get 0x%x = 0x%x\n", addr, reg, IIC_Read(addr, reg));
        }

        IIC_Close();
        return 0;
}
