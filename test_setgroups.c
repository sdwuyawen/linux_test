/**
 *	@filename :
 * 		test_setgroups.c 
 * 
 * 	@note :
 * 
 **/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>

int main()
{
	gid_t gid = 500; //500是study用户的id

	//降低权限
	setgroups(1, &gid); //将当前进程的附加进程组id设为500
	setegid(gid);
	seteuid(gid);
	printf("%d\n", chdir("/root"));

	//升高权限
	gid = 0; //0是root用户的id
	setegid(gid);
	seteuid(gid);
	setgroups(1, &gid); //将当前进程的附加进程组id设为500
	printf("%d\n", chdir("/root"));
	return 0;
}
