#include "cpparglib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

cpparglib::~cpparglib()
{
    for(int i = 0 ; i < argc ; i ++ )
    {
        free(argv[i]);
    }
    free(argv);
}

cpparglib::cpparglib(int argcA, char *argvA[])
{
    argc = argcA;


    argv = (char**)malloc(32*argc);

    for(int i = 0 ; i < argcA ; i ++ )
    {
        argv[i] = (char*)malloc(strlen(argvA[i])+1);
        strcpy(argv[i], argvA[i]);
    }
}


char *cpparglib::parseParameterForValue(char *ParameterName)
{
        int m;


        for ( m = 1 ; m < argc ; m ++ )
        {
                if(0 == strcmp(ParameterName, argv[m]))
                {
                        if(m < argc)
                                return argv[m+1];
                }
        }

        return NULL;
}


bool cpparglib::parseParameter(char *ParameterName)
{
        int m;


        for ( m = 1 ; m < argc ; m ++ )
        {
                if(0 == strcmp(ParameterName, argv[m]))
                {
                        return 1;
                }
        }

        return 0;
}
