#ifndef __GET_IDENT__
#define __GET_IDENT__

#define GI_SUCC 0

#include <get_len_ident.h>

/*Copy SIP identificator from url[] to ident[].
 *If wrong URL, return -1.
 *If len_ident < lenght of identificator return -2,
 *also change len_ident to desired value.
 */

int get_ident(char url[], int len_url, char ident[], int *len_ident);

#endif //__GET_IDENT__