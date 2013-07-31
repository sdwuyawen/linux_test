/*
 * md5sum.c	- Generate/check MD5 Message Digests
 *
 * Compile and link with md5.c.  If you don't have getopt() in your library
 * also include getopt.c.  For MSDOS you can also link with the wildcard
 * initialization function (wildargs.obj for Turbo C and setargv.obj for MSC)
 * so that you can use wildcards on the commandline.
 *
 * Written March 1993 by Branko Lankester
 * Modified June 1993 by Colin Plumb for altered md5.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "md5.h"

#ifdef LINUX
#define	FOPRTXT	"r"
#define	FOPRBIN	"r"
#else
#ifdef VMS
#define	FOPRTXT	"r","ctx=stm"
#define	FOPRBIN	"rb","ctx=stm"
#else
#define	FOPRTXT	"r"
#define	FOPRBIN	"rb"
#endif
#endif

extern char *optarg;
extern int optind;

void usage();
void print_digest();
int mdfile(FILE *fp, unsigned char *digest);
int do_check(FILE *chkf);
int md5sum(char *file, unsigned char *digest);

char *progname;
int verbose = 0;
int bin_mode = 0;

#if 1
void
main(int argc, char *argv[])
{
	int opt, rc = 0;
	int check = 0;
	FILE *fp;
	unsigned char digest[16];
	int index;
	
	progname = argv[0];
	while ((opt = getopt(argc, argv, "cbvp:h")) != -1) {
		switch (opt) {
			case 'c': check = 1; break;
			case 'v': verbose = 1; break;
			case 'b': bin_mode = 1; break;
			default: usage();
		}
	}
	
	index = optind;
	if (check) {
		switch (argc-index) {
			case 0: fp = stdin; break;
			case 1: if ((fp = fopen(argv[index], FOPRTXT)) == NULL) {
					perror(argv[index]);
					exit(2);
				}
				break;
			default: usage();
		}
		exit(do_check(fp));
	}
	
	if (argc == 0) {
		if (mdfile(stdin, digest)) {
			fprintf(stderr, "%s: read error on stdin\n", progname);
			exit(2);
		}
		print_digest(digest);
		printf("\n");
		exit(0);
	}
	for ( ; index < argc; index++) {
		//printf("index = %d, %s\n", index, argv[index]);
		if (bin_mode)
			fp = fopen(argv[index], FOPRBIN);
		else
			fp = fopen(argv[index], FOPRTXT);
		if (fp == NULL) {
			perror(argv[index]);
			rc = 2;
			continue;
		}
		if (mdfile(fp, digest)) {
			fprintf(stderr, "%s: error reading %s\n", progname, argv[index]);
			rc = 2;
		} else {
			print_digest(digest);
			printf(" %c%s\n", bin_mode ? '*' : ' ', argv[index]);
		}
		fclose(fp);
	}
	exit(rc);
}
#else
int main()
{
    unsigned char digest[16];
    char *myfile="/config.txt";
    int md5=md5sum(myfile, digest);
    printf("##%d\n",md5);

    return 0;
}

int md5sum(char *file, unsigned char *digest )
{
     FILE *fp = fopen(file, FOPRTXT);
               
     if (fp == NULL) {
         return -1;
     }
                
     if (mdfile(fp, digest)) {
         printf("md5sum error!\n");
         fclose(fp);
         return -1;
     } 

     print_digest(digest);
     fclose(fp);

     return 0;
}
#endif

void
usage()
{
	fprintf(stderr, "usage: md5sum [-bv] [-c [file]] | [file...]\n");
	fprintf(stderr, "Generates or checks MD5 Message Digests\n");
	fprintf(stderr, "    -c  check message digests (default is generate)\n");
	fprintf(stderr, "    -v  verbose, print file names when checking\n");
	fprintf(stderr, "    -b  read files in binary mode\n");
	fprintf(stderr, "The input for -c should be the list of message digests and file names\n");
	fprintf(stderr, "that is printed on stdout by this program when it generates digests.\n");
	exit(2);
}

int
mdfile(FILE *fp, unsigned char *digest)
{
	unsigned char buf[1024];
	MD5_CTX ctx;
	int n;

	MD5Init(&ctx);
	while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
		MD5Update(&ctx, buf, n);
	MD5Final(digest, &ctx);
	if (ferror(fp))
		return -1;
	return 0;
}

void
print_digest(unsigned char *p)
{
	int i;
	char str[3];
	char md5code[33];
	
	//strcpy(md5code,"");
	memset(md5code,0,33);
	for (i = 0; i < 16; ++i)
	{
		//printf("%02x", *p++);
		sprintf(str,"%02x", *p++);
		strcat(md5code,str);
	}
	printf("%s",md5code);
}

int
hex_digit(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return -1;
}

int
get_md5_line(FILE *fp, unsigned char *digest, char *file)
{
	char buf[1024];
	int i, d1, d2, rc;
	char *p = buf;

	if (fgets(buf, sizeof(buf), fp) == NULL)
		return -1;

	for (i = 0; i < 16; ++i) {
		if ((d1 = hex_digit(*p++)) == -1)
			return 0;
		if ((d2 = hex_digit(*p++)) == -1)
			return 0;
		*digest++ = d1*16 + d2;
	}
	if (*p++ != ' ')
		return 0;
	/*
	 * next char is an attribute char, space means text file
	 * if it's a '*' the file should be checked in binary mode.
	 */
	if (*p == ' ')
		rc = 1;
	else if (*p == '*')
		rc = 2;
	else {
		fprintf(stderr, "%s: unrecognized line: %s", progname, buf);
		return 0;
	}
	++p;
	i = strlen(p);
	if (i < 2 || i > 255)
		return 0;
	p[i-1] = '\0';
	strcpy(file, p);
	return rc;
}

int
do_check(FILE *chkf)
{
	int rc, ex = 0, failed = 0, checked = 0;
	unsigned char chk_digest[16], file_digest[16];
	char filename[256];
	FILE *fp;
	int flen = 14;

	while ((rc = get_md5_line(chkf, chk_digest, filename)) >= 0) {
		if (rc == 0)	/* not an md5 line */
			continue;
		if (verbose) {
			if (strlen(filename) > flen)
				flen = strlen(filename);
			fprintf(stderr, "%-*s ", flen, filename);
		}
		if (bin_mode || rc == 2)
			fp = fopen(filename, FOPRBIN);
		else
			fp = fopen(filename, FOPRTXT);
		if (fp == NULL) {
			fprintf(stderr, "%s: can't open %s\n", progname, filename);
			ex = 2;
			continue;
		}
		if (mdfile(fp, file_digest)) {
			fprintf(stderr, "%s: error reading %s\n", progname, filename);
			ex = 2;
			fclose(fp);
			continue;
		}
		fclose(fp);
		if (memcmp(chk_digest, file_digest, 16) != 0) {
			if (verbose)
				fprintf(stderr, "FAILED\n");
			else
				fprintf(stderr, "%s: MD5 check failed for '%s'\n", progname, filename);
			++failed;
		} else if (verbose)
			fprintf(stderr, "OK\n");
		++checked;
	}
	if (verbose && failed)
		fprintf(stderr, "%s: %d of %d file(s) failed MD5 check\n", progname, failed, checked);
	if (!checked) {
		fprintf(stderr, "%s: no files checked\n", progname);
		return 3;
	}
	if (!ex && failed)
		ex = 1;
	return ex;
}
