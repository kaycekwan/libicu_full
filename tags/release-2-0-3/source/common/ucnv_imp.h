/*
**********************************************************************
*   Copyright (C) 1999-2001, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*
*
*  ucnv_imp.h:
*  Contains all internal and external data structure definitions
* Created & Maitained by Bertrand A. Damiba
*
*
*
* ATTENTION:
* ---------
* Although the data structures in this file are open and stack allocatable
* we reserve the right to hide them in further releases.
*/

#ifndef UCNV_IMP_H
#define UCNV_IMP_H

#include "unicode/utypes.h"
#include "ucnv_bld.h"

/* figures out if we need to go to file to read in the data tables.
 */
UConverter *ucnv_createConverter (const char *converterName, UErrorCode * err);

/* Stores the shared data in the SHARED_DATA_HASHTABLE
 */
void ucnv_shareConverterData (UConverterSharedData * data);

/* gets the shared data from the SHARED_DATA_HASHTABLE (might return NULL if it isn't there)
 */
UConverterSharedData *ucnv_getSharedConverterData (const char *name);

/* Deletes (frees) the Shared data it's passed. first it checks the referenceCounter to
 * see if anyone is using it, if not it frees all the memory stemming from sharedConverterData and
 * returns TRUE,
 * otherwise returns FALSE
 */
UBool ucnv_deleteSharedConverterData (UConverterSharedData * sharedConverterData);

/* returns true if "name" is in algorithmicConverterNames
 */
UBool ucnv_isDataBasedConverter (const char *name);

void ucnv_copyPlatformString (char *platformString, UConverterPlatform pltfrm);


#endif /* _UCNV_IMP */
