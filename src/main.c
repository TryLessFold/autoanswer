#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pjsua-lib/pjsua.h>
int main()
{
	pj_status_t status;
	status = pjsua_create();
	return 0;
}
