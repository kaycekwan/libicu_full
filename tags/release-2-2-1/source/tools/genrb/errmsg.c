/*
*******************************************************************************
*
*   Copyright (C) 1998-2000, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*
* File error.c
*
* Modification History:
*
*   Date        Name        Description
*   05/28/99    stephen     Creation.
*******************************************************************************
*/

#include <stdarg.h>
#include <stdio.h>
#include "cstring.h"
#include "errmsg.h"

void error(uint32_t linenumber, const char *msg, ...)
{
    va_list va;

    va_start(va, msg);
    fprintf(stderr, "%s:%d: ", gCurrentFileName, linenumber);
    vfprintf(stderr, msg, va);
    fprintf(stderr, "\n");
    va_end(va);
}

static UBool gShowWarning = TRUE;

void setShowWarning(UBool val)
{
    gShowWarning = val;
}

UBool getShowWarning(){
    return gShowWarning;
}

void warning(uint32_t linenumber, const char *msg, ...)
{
    if (gShowWarning)
    {
        va_list va;

        va_start(va, msg);
        fprintf(stderr, "%s:%d: warning: ", gCurrentFileName, linenumber);
        vfprintf(stderr, msg, va);
        fprintf(stderr, "\n");
        va_end(va);
    }
}

