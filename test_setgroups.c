/**
 *	@filename :
 * 		test_setgroups.c 
 * 
 * 	@note :
 * 		su ./bin/test_setgroups
 * 		一个非特权用户调用上述两个函数会将其有效用户/组ID设置为其实际用户ID或保存的设置用户ID，
 * 		而只有特权用户才可以将有效用户/组ID设置为参数所提供的ID
 **/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>

int main()
{
	gid_t gid = 500; //500是study用户的id

	//uid   = root
	//euid  = root
	//suid  = root (copy from euid)
	printf("uid=%d, euid=%d\n", getuid(), geteuid());
	//降低权限
	setgroups(1, &gid); //将当前进程的附加进程组id设为500
	setegid(gid);
	seteuid(gid);
	//
	//如果是下面的， 两个chdir()返回-1. uid=euid=suid=500
	//setgid(gid);
	//setuid(gid);
	printf("%d\n", chdir("/root"));
	printf("uid=%d, euid=%d\n", getuid(), geteuid());

	//uid   = root
	//euid  = 500
	//suid  = root
	
	//升高权限
	gid = 0; //0是root用户的id
	setegid(gid);
	seteuid(gid);
	setgroups(1, &gid); //将当前进程的附加进程组id设为500
	printf("%d\n", chdir("/root"));
	printf("uid=%d, euid=%d\n", getuid(), geteuid());
	//uid   = root
	//euid  = root
	//suid  = root
	return 0;
}
