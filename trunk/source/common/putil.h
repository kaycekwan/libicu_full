/*
*******************************************************************************
*                                                                             *
* COPYRIGHT:                                                                  *
*   (C) Copyright Taligent, Inc.,  1997                                       *
*   (C) Copyright International Business Machines Corporation,  1997-1999     *
*   Licensed Material - Program-Property of IBM - All Rights Reserved.        *
*   US Government Users Restricted Rights - Use, duplication, or disclosure   *
*   restricted by GSA ADP Schedule Contract with IBM Corp.                    *
*                                                                             *
*******************************************************************************
*
*  FILE NAME : putil.h
*
*   Date        Name        Description
*   05/14/98    nos         Creation (content moved here from utypes.h).
*   06/17/99    erm         Added IEEE_754
*   07/22/98    stephen     Added IEEEremainder, max, min, trunc
*   08/13/98    stephen     Added isNegativeInfinity, isPositiveInfinity
*   08/24/98    stephen     Added longBitsFromDouble
*   03/02/99    stephen     Removed openFile().  Added AS400 support.
*   04/15/99    stephen     Converted to C
*******************************************************************************
*/

#ifndef PUTIL_H
#define PUTIL_H

#include "utypes.h"

/* Define this if your platform supports IEEE 754 floating point */
#define IEEE_754       

/*===========================================================================*/
/* Platform utilities                                                        */
/*===========================================================================*/

/**
 * Platform utilities isolates the platform dependencies of the
 * libarary.  For each platform which this code is ported to, these
 * functions may have to be re-implemented.  */

/* Floating point utilities */
CAPI bool_t  U_EXPORT2  icu_isNaN(double);
CAPI bool_t  U_EXPORT2 icu_isInfinite(double);
CAPI bool_t   U_EXPORT2 icu_isPositiveInfinity(double);
CAPI bool_t   U_EXPORT2 icu_isNegativeInfinity(double);
CAPI double   U_EXPORT2 icu_getNaN(void);
CAPI double   U_EXPORT2 icu_getInfinity(void);

CAPI double   U_EXPORT2 icu_floor(double x);
CAPI double   U_EXPORT2 icu_ceil(double x);
CAPI double   U_EXPORT2 icu_fabs(double x);
CAPI double   U_EXPORT2 icu_modf(double x, double* y);
CAPI double   U_EXPORT2 icu_fmod(double x, double y);
CAPI double   U_EXPORT2 icu_pow10(int32_t x);
CAPI double   U_EXPORT2 icu_IEEEremainder(double x, double y);
CAPI double   U_EXPORT2 icu_fmax(double x, double y);
CAPI double   U_EXPORT2 icu_fmin(double x, double y);
CAPI int32_t  U_EXPORT2 icu_max(int32_t x, int32_t y);
CAPI int32_t  U_EXPORT2 icu_min(int32_t x, int32_t y);
CAPI double   U_EXPORT2 icu_trunc(double d);
CAPI void     U_EXPORT2 icu_longBitsFromDouble(double d, int32_t *hi, uint32_t *lo);

/*
 * Return the floor of the log base 10 of a given double.
 * This method compensates for inaccuracies which arise naturally when
 * computing logs, and always gives the correct value.  The parameter
 * must be positive and finite.
 * (Thanks to Alan Liu for supplying this function.)
 */
/**
 * Returns the common log of the double value d.
 * @param d the double value to apply the common log function for.
 * @return the log of value d.
 */
CAPI int16_t  U_EXPORT2 icu_log10(double d);

/**
 * Returns the number of digits after the decimal point in a double number x.
 * @param x the double number
 */
CAPI int32_t  U_EXPORT2 icu_digitsAfterDecimal(double x);

/**
 * Time zone utilities
 *
 * Wrappers for C runtime library functions relating to timezones.
 * The t_tzset() function (similar to tzset) uses the current setting 
 * of the environment variable TZ to assign values to three global 
 * variables: daylight, timezone, and tzname. These variables have the 
 * following meanings, and are declared in &lt;time.h>.
 *
 *   daylight   Nonzero if daylight-saving-time zone (DST) is specified
 *              in TZ; otherwise, 0. Default value is 1.
 *   timezone   Difference in seconds between coordinated universal
 *              time and local time. E.g., -28,800 for PST (GMT-8hrs)
 *   tzname(0)  Three-letter time-zone name derived from TZ environment
 *              variable. E.g., "PST".
 *   tzname(1)  Three-letter DST zone name derived from TZ environment
 *              variable.  E.g., "PDT". If DST zone is omitted from TZ,
 *              tzname(1) is an empty string.
 *
 * Notes: For example, to set the TZ environment variable to correspond
 * to the current time zone in Germany, you can use one of the
 * following statements:
 *
 *   set TZ=GST1GDT
 *   set TZ=GST+1GDT
 *
 * If the TZ value is not set, t_tzset() attempts to use the time zone
 * information specified by the operating system. Under Windows NT
 * and Windows 95, this information is specified in the Control Panel�s
 * Date/Time application.
 */
CAPI void     U_EXPORT2 icu_tzset(void);
CAPI int32_t  U_EXPORT2 icu_timezone(void);
CAPI char*    U_EXPORT2 icu_tzname(int index);

/* Get UTC (GMT) time measured in seconds since 0:00 on 1/1/70. */
CAPI int32_t  U_EXPORT2 icu_getUTCtime(void);

/* Return the default data directory for this platform.  See Locale. */
CAPI const char*  U_EXPORT2 icu_getDefaultDataDirectory(void);

/* Return the default codepage for this platform and locale */
CAPI const char*  U_EXPORT2 icu_getDefaultCodepage(void);

/* Return the default locale ID string by querying ths system, or
       zero if one cannot be found. */
CAPI const char*  U_EXPORT2 icu_getDefaultLocaleID(void);

/**
 * Retrun true if this platform is big-endian, that is, if the number
 * 0x1234 is stored 0x12, 0x34 in memory.  Return false if this platform
 * is little-endian, and is, if 0x1234 is stored 0x34, 0x12 in memory.
 */
CAPI bool_t           U_EXPORT2 icu_isBigEndian(void);

/*
 * Finds the least double greater than d (if positive == true),
 * or the greatest double less than d (if positive == false).
 *
 * This is a special purpose function defined by the ChoiceFormat API
 * documentation.
 * It is not a general purpose function and not defined for NaN or Infinity
 */
CAPI double           U_EXPORT2 icu_nextDouble(double d, bool_t positive);

#endif




