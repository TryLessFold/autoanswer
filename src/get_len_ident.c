#include <get_len_ident.h>

int get_len_ident(char uri[],
                int len_uri,
                int *len_ident)
{
    (*len_ident) = 0;
    if (len_uri <= 1)
        return -1;
    for (int i = 0; i < len_uri; i++)
    {
        if (uri[i] == ':')
        {
            while (uri[i] != '@')
            {
                i++;
                if (i >= len_uri)
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