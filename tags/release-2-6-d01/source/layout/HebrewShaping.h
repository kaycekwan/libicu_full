/*
 * %W% %E%
 *
 * (C) Copyright IBM Corp. 1998, 1999, 2000 - All Rights Reserved
 *
 */

#ifndef __HEBREWSHAPING_H
#define __HEBREWSHAPING_H

/**
 * \file
 * \internal
 */

#include "LETypes.h"
#include "OpenTypeTables.h"

U_NAMESPACE_BEGIN

class HebrewShaping /* not : public UObject because all methods are static */ {
public:
    static void shape(const LEUnicode *chars, le_int32 offset, le_int32 charCount, le_int32 charMax,
                      le_bool rightToLeft, const LETag **tags);

    static const le_uint8 glyphSubstitutionTable[];
    static const le_uint8 glyphDefinitionTable[];

private:
    // forbid instantiation
    HebrewShaping();
};

U_NAMESPACE_END
#endif
