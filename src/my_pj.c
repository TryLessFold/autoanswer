#include<my_pj.h>

/* Display error and exit application */
void error_exit(const char *title,
				pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(1);
}
