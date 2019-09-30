#include <get_len_ident.h>

int get_len_ident(char url[], int len_url, int *len_ident)
{
    (*len_ident) = 0;
    if (len_url <= 1)
        return -1;
    for (int i = 0; i < len_url; i++)
    {
        if (url[i] == ':')
        {
            while (url[i] != '@')
            {
                i++;
                if (i >= len_url)
                {
                    return -1;
                }
                (*len_ident)++;
            }
            (*len_ident)--;
            return 0;
        }
    }
    return -1;
}