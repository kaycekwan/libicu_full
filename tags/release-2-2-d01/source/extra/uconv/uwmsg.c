/*
**********************************************************************
* Copyright (C) 1998-2000, International Business Machines Corporation 
* and others.  All Rights Reserved.
**********************************************************************
*
* File uwmsg.c
*
* Modification History:
*
*   Date        Name        Description
*   06/14/99    stephen     Creation.
*******************************************************************************
*/

#include "unicode/ucnv.h"
#include "unicode/ustring.h"
#include "unicode/umsg.h"
#include "unicode/uwmsg.h"
#include "unicode/ures.h"
#include "cstring.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 128

/* Print a ustring to the specified FILE* in the default codepage */
static void
uprint(const UChar *s,
       int32_t sourceLen,
       FILE *f,
       UErrorCode *status)
{
    /* converter */
    UConverter *converter;
    char buf [BUF_SIZE];
    const UChar *mySource;
    const UChar *mySourceEnd;
    char *myTarget;
    int32_t arraySize;

    if(s == 0) return;

    /* set up the conversion parameters */
    mySource     = s;
    mySourceEnd  = mySource + sourceLen;
    myTarget     = buf;
    arraySize    = BUF_SIZE;

    /* open a default converter */
    converter = ucnv_open(0, status);

    /* if we failed, clean up and exit */
    if(U_FAILURE(*status)) goto finish;

    /* perform the conversion */
    do {
        /* reset the error code */
        *status = U_ZERO_ERROR;

        /* perform the conversion */
        ucnv_fromUnicode(converter, &myTarget,  myTarget + arraySize,
            &mySource, mySourceEnd, NULL,
            TRUE, status);

        /* Write the converted data to the FILE* */
        fwrite(buf, sizeof(char), myTarget - buf, f);

        /* update the conversion parameters*/
        myTarget     = buf;
        arraySize    = BUF_SIZE;
    }
    while(*status == U_BUFFER_OVERFLOW_ERROR); 

finish:

    /* close the converter */
    ucnv_close(converter);
}

static const char *gPath = 0;
static UResourceBundle *gBundle = NULL;

U_CAPI UResourceBundle *u_wmsg_setPath(const char *path, UErrorCode *err)
{
  if(U_FAILURE(*err))
  {
    return 0;
  }

  if(gBundle != NULL)
  {
    *err = U_ILLEGAL_ARGUMENT_ERROR;
    return 0;
  }
  else
  {
    UResourceBundle *b = NULL;
    b = ures_open(path, NULL, err);
    if(U_FAILURE(*err))
    {
         return 0;
    }

    gPath = uprv_strdup(path);
    gBundle = b;
  }
  
  return gBundle;
}

/* Format a message and print it's output to fp */
U_CAPI int u_wmsg(FILE *fp, const char *tag, ... )
{
    const UChar *msg;
    int32_t      msgLen;
    UErrorCode  err = U_ZERO_ERROR;
    va_list ap;
    UChar   result[4096];
    int32_t resultLength  = 4096;

    if(gBundle == NULL)
    {
#if 0
        fprintf(stderr, "u_wmsg: No path set!!\n"); /* FIXME: codepage?? */
#endif
        return -1;
    }

    msg = ures_getStringByKey(gBundle, tag, &msgLen, &err);

    if(U_FAILURE(err))
    {
#if 0
        fprintf(stderr, "u_wmsg: failed to load tag [%s] [%s] [%s]!!\n", tag,  u_errorName(err), gPath);
#endif
        return -1;
    }

    va_start(ap, tag);

    resultLength = u_vformatMessage(uloc_getDefault(), msg, msgLen, result, resultLength, ap, &err);

    va_end(ap);

    if(U_FAILURE(err))
    {
#if 0
        fprintf(stderr, "u_wmsg: failed to format %s:%s, err %s\n",
            uloc_getDefault(),
            tag,
            u_errorName(err));
#endif
        err = U_ZERO_ERROR;
        uprint(msg,msgLen, fp, &err);
        return -1;
    }

    uprint(result, resultLength, fp, &err);

    if(U_FAILURE(err))
    {
#if 0
        fprintf(stderr, "u_wmsg: failed to print %s: %s, err %s\n",
            uloc_getDefault(),
            tag,
            u_errorName(err));
#endif
        return -1;
    }

    return 0;
}

/* these will break if the # of messages change. simply add or remove 0's .. */
UChar * gInfoMessages[U_ERROR_WARNING_LIMIT-U_ERROR_WARNING_START] = 
    { 0,0 };

UChar * gErrMessages[U_ERROR_LIMIT] = 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

static const UChar *fetchErrorName(UErrorCode err)
{
    if(err>=0)
        return gErrMessages[err];
    else
        return gInfoMessages[err-U_ERROR_WARNING_START];
}

U_CAPI const UChar *u_wmsg_errorName(UErrorCode err)
{
    UChar *msg;
    int32_t msgLen;
    UErrorCode subErr = U_ZERO_ERROR;
    const char *textMsg = NULL;

    /* try the cache */
    msg = (UChar*)fetchErrorName(err);

    if(msg)
    {
        return msg;
    }

    if(gBundle == NULL)
    {
        msg = NULL;
    }
    else
    {
        const char *errname = u_errorName(err);
        if (errname) {
            msg = (UChar*)ures_getStringByKey(gBundle, errname, &msgLen, &subErr);
            if(U_FAILURE(subErr))
            {
                msg = NULL;
            }
        }
    }

    if(msg == NULL)  /* Couldn't find it anywhere.. */
    {
        char error[128];
        textMsg = u_errorName(err);
        if (!textMsg) {
            sprintf(error, "UNDOCUMENTED ICU ERROR %d", err);
            textMsg = error;
        }
        msg = (UChar*)malloc((strlen(textMsg)+1)*sizeof(msg[0]));
        u_charsToUChars(textMsg, msg, (int32_t)(strlen(textMsg)+1));
    }

    if(err>=0)
        gErrMessages[err] = msg;
    else
        gInfoMessages[err-U_ERROR_WARNING_START] = msg;

    return msg;
}
