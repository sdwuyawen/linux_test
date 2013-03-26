/*
 * test_tty.c
 * 
 * Copyright 2012 paul <paul@paul-desktop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>


#define FALSE 0     
#define TRUE 1             

static volatile int STOP = FALSE;        
static int BAUDRATE;
/*
 * 
 */
int main(int argc, char **argv) {
	int baudrate;
	int fd, c, res;
	struct termios oldtio, newtio;
	char buf[255];	

	if (argc < 3) {
		printf("%s tty_device baudrate\n", argv[0]);
		return -1;
	}

	baudrate = atoi(argv[2]);

	printf("tty : %s, rate=%d\n", argv[1], baudrate);

	/* Open modem device for reading and writing and not as controlling
	 tty because we don't want to get killed if linenoise sends CTRL-C.
	 开启设备用于读写，但是不要以控制 tty 的模式，因为我们并不希望在发送 Ctrl-C
	 后结束此进程
	 */
	fd = open(argv[1], O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror(argv[1]);
		exit(-1);
	}
	tcgetattr(fd, &oldtio); /* save current serial port settings */
	// 储存当前的串口设置
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
	// 清空新的串口设置结构体
	/*
	 BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	 CRTSCTS : output hardware flow control (only used if the cable has all
	 ecessary lines. See sect. 7 of Serial-HOWTO)
	 CS8     : 8n1 (8bit,no parity,1 stopbit)
	 CLOCAL  : local connection, no modem contol
	 CREAD   : enable receiving characters
	 BAUDRATE: 设置串口的传输速率bps, 也可以使用 cfsetispeed 和 cfsetospeed 来设置
	 CRTSCTS : 输出硬件流控(只能在具完整线路的缆线下工作，参考 Serial-HOWTO 第七节)
	 CS8     : 8n1 (每一帧8比特数据,无奇偶校验位,1 比特停止位)
	 CLOCAL  : 本地连接，无调制解调器控制
	 CREAD   : 允许接收数据
	 */	
	switch (baudrate) {
		case 9600:
			BAUDRATE = B9600;
			break;
			
		case 38400:
			BAUDRATE = B38400;
			break;	
			
		case 57600:
			BAUDRATE = B57600;
			break;
		
		case 115200:	
		default:
			BAUDRATE = B115200;
			break;
	}
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
		
	/*
	 IGNPAR  : ignore bytes with parity errors
	 ICRNL   : map CR to NL (otherwise a CR input on the other computer will not
	 terminate input) otherwise make device raw (no other input processing)
	 IGNPAR  : 忽略奇偶校验出错的字节
	 ICRNL   : 把 CR 映像成 NL (否则从其它机器传来的 CR 无法终止输入)或者就把设备设
	 为 raw 状态（没有额外的输入处理）
	 */
	newtio.c_iflag = IGNPAR | ICRNL;

	/*
	 Raw output.  Raw 模式输出
	 */
	newtio.c_oflag = 0;

	/*
	 ICANON  : enable canonical input
	 disable all echo functionality, and don't send signals to calling program
	 ICANON : 启动 标准输出， 关闭所有回显echo 功能，不向程序发送信号
	 */
	newtio.c_lflag = ICANON;

	/*
	 initialize all control characters
	 default values can be found in /usr/include/termios.h, and
	 are given in the comments, but we don't need them here
	 初始化所有的控制字符， 默认值可以在 /usr/include/termios.h 找到，
	 并 且做了注解，不过这里我们并不需要考虑这些
	 */
	newtio.c_cc[VINTR] = 0; /* Ctrl-c */
	newtio.c_cc[VQUIT] = 0; /* Ctrl-/ */
	newtio.c_cc[VERASE] = 0; /* del */
	newtio.c_cc[VKILL] = 0; /* @ */
	newtio.c_cc[VEOF] = 4; /* Ctrl-d */
	newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
	/* 不使用字符间的计时器 */
	newtio.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
	/* 阻塞，直到读取到一个字符 */
	newtio.c_cc[VSWTC] = 0; /* '/0' */
	newtio.c_cc[VSTART] = 0; /* Ctrl-q */
	newtio.c_cc[VSTOP] = 0; /* Ctrl-s */
	newtio.c_cc[VSUSP] = 0; /* Ctrl-z */
	newtio.c_cc[VEOL] = 0; /* '/0' */
	newtio.c_cc[VREPRINT] = 0; /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0; /* Ctrl-u */
	newtio.c_cc[VWERASE] = 0; /* Ctrl-w */
	newtio.c_cc[VLNEXT] = 0; /* Ctrl-v */
	newtio.c_cc[VEOL2] = 0; /* '/0' */

	/*
	 now clean the modem line and activate the settings for the port
	 清空数据线，启动新的串口设置
	 */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
#if 0
	/*
	 terminal settings done, now handle input
	 In this example, inputting a 'z' at the beginning of a line will
	 exit the program.
	 终端设置完成，现在就可以处理数 据了
	 在本程序中，在一行的开始输入一个 'z' 会终止该程序
	 */
	while (STOP == FALSE) { /* loop until we have a terminating condition */
		// 循环直到满足终止条件
		/* read blocks program execution until a line terminating character is
		 input, even if more than 255 chars are input. If the number
		 of characters read is smaller than the number of chars available,
		 subsequent reads will return the remaining chars. res will be set
		 to the actual number of characters actually read
		 即使 输入超过 255 个字节，读取的程序段还是会一直等到行结束符出现才会停止。
		 如果读到的字符少于应刚获得的字符数，则剩下的字符串会在下一次读取时读到。
		 res 用来获得每次真正读到的字节数
		 */
		res = read(fd, buf, 255);
		buf[res] = 0; /* set end of string, so we can printf */
		// 设置字符串结束符，从而可以顺利使用 printf
		printf(":%s:%d/n", buf, res);
		if (buf[0] == 'z')
			STOP = TRUE;
	}
#else
	//send data 
	sprintf(buf, "hello world\n");
	write(fd, buf, strlen(buf));
#endif
	
	/* restore the old port settings 恢复旧的串口设置 */
	tcsetattr(fd, TCSANOW, &oldtio);

	return 0;
}
