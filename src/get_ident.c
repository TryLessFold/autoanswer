#include <get_ident.h>

int get_ident(char url[], int len_url, char ident[], int *len_ident)
{
    int tmp_ident = 0;
    int i = 0, j = 0;
    if (get_len_ident(url, len_url, &tmp_ident) != GI_SUCC)
    {
        return -1;
    }
    if (tmp_ident > *len_ident){
        *len_ident = tmp_ident;
        return -2;
    }
    while (url[i] != ':')
        i++;
    i++;
    for (j = 0; j < tmp_ident; j++, i++)
    {
        ident[j] = url[i];
    }
    ident[j] = '\0';
    return 0;
}