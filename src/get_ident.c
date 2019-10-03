#include <get_ident.h>

int get_ident(char uri[], 
            int len_uri,
            char ident[],
            int *len_ident)
{
    int tmp_ident = 0;
    int i = 0, j = 0;
    if (get_len_ident(uri, len_uri, &tmp_ident) != GI_SUCC)
    {
        return -1;
    }
    if (tmp_ident > *len_ident){
        *len_ident = tmp_ident;
        return -2;
    }
    while (uri[i] != ':')
        i++;
    i++;
    for (j = 0; j < tmp_ident; j++, i++)
    {
        ident[j] = uri[i];
    }
    ident[j] = '\0';
    return 0;
}