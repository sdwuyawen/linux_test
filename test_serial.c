/**
   @filename :
   test_serial.c
**/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <term.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

/**
 **/
#define TEST_BAUDRATE    B57600

/**
 **/
int main(int argc, char *argv[]) {
    int fd;
    unsigned int c_bits;
    
    printf("argc=%d, argv[0]=%s\n", argc, argv[0]);

    if ( argc < 2 ) {
        printf("cmd : %s tty_nam []\n", argv[0]);
        
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if ( fd < 0 ) {
        printf("open %s fail\n", argv[1]);
        return -1;
    }

    if ( isatty(fd) ) {
        struct termios ios;

        tcgetattr(fd, &ios);
        ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
        ios.c_oflag &= (~ONLCR); /* Stop \n -> \r\n translation on output */
        ios.c_iflag &= (~(ICRNL | INLCR)); /* Stop \r -> \n & \n -> \r translation on input */
        ios.c_iflag |= (IGNCR | IXOFF);  /* Ignore \r & XON/XOFF on input */
        cfsetospeed(&ios, TEST_BAUDRATE);
        cfsetispeed(&ios, TEST_BAUDRATE);
        tcsetattr(fd, TCSANOW, &ios);        
    } else {
        close(fd);
        printf("%s is not tty\n", argv[1]);

        return -1;
    }

    printf("take high\n");
    c_bits = TIOCM_DTR;
    ioctl(fd, TIOCMBIC, &c_bits);
    c_bits = TIOCM_RTS;
    ioctl(fd, TIOCMBIC, &c_bits);
    while ( getchar() != 'q' ) {
        sleep(1);
    }
    printf("take low\n");
    c_bits = TIOCM_DTR;
    ioctl(fd, TIOCMBIS, &c_bits);
    c_bits = TIOCM_RTS;
    ioctl(fd, TIOCMBIS, &c_bits);
    while ( getchar() != 'q' ) {
        sleep(1);
    }

    close(fd);
        
    return 0;
}
