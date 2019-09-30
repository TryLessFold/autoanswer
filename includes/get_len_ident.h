#ifndef __GET_LEN_IDENT__
#define __GET_LEN_IDENT__

#define GLI_SUCC 0

/* Get length of identificator from url adress to len_ident
 * If the url is wrong, return -1
 */

int get_len_ident(char url[], int len_url, int *len_ident);

#endif //__GET_LEN_IDENT__