// from the PDPCLIB, with simplifications to assume nice data.
// written by paul edwards and released into public domain.
// source: https://sourceforge.net/projects/pdos/

#include <stdlib.h>

int _isdigit(unsigned char c)
{
  return (c >= '0') && (c <= '9');
}

double pdos_strtod(const char *nptr, char **endptr)
{
    double x = 0.0;
    double xs= 1.0;
    double es = 1.0;
    double xf = 0.0;
    double xd = 1.0;

    if(*nptr == '-')
    {
        xs = -1;
        nptr++;
    }
    else if(*nptr == '+')
    {
        nptr++;
    }


    while (1)
    {
        if (_isdigit((unsigned char)*nptr))
        {
            x = x * 10 + (*nptr - '0');
            nptr++;
        }
        else
        {
            x = x * xs;
            break;
        }
    }
    if (*nptr == '.')
    {
        nptr++;
        while (1)
        {
            if (_isdigit((unsigned char)*nptr))
            {
                xf = xf * 10 + (*nptr - '0');
                xd = xd * 10;
            }
            else
            {
                x = x + xs * (xf / xd);
                break;
            }
            nptr++;
        }
    }
    if ((*nptr == 'e') || (*nptr == 'E'))
    {
        nptr++;
        if (*nptr == '-')
        {
            es = -1;
            nptr++;
        }
        xd = 1;
        xf = 0;
        while (1)
        {
            if (_isdigit((unsigned char)*nptr))
            {
                xf = xf * 10 + (*nptr - '0');
                nptr++;
            }
            else
            {
                while (xf > 0)
                {
                    xd *= 10;
                    xf--;
                }
                if (es < 0.0)
                {
                    x = x / xd;
                }
                else
                {
                    x = x * xd;
                }
                break;
            }
        }
    }
    if (endptr != NULL)
    {
        *endptr = (char *)nptr;
    }
    return (x);
}

