/*
 * @(#)ClassDefinitionTables.cpp	1.5 00/03/15
 *
 * (C) Copyright IBM Corp. 1998, 1999, 2000 - All Rights Reserved
 *
 */

#include "LETypes.h"
#include "OpenTypeTables.h"
#include "OpenTypeUtilities.h"
#include "ClassDefinitionTables.h"
#include "LESwaps.h"

le_int32 ClassDefinitionTable::getGlyphClass(LEGlyphID glyphID)
{
    switch(SWAPW(classFormat)) {
    case 0:
        return 0;

    case 1:
    {
        ClassDefFormat1Table *f1Table = (ClassDefFormat1Table *) this;

        return f1Table->getGlyphClass(glyphID);
    }

    case 2:
    {
        ClassDefFormat2Table *f2Table = (ClassDefFormat2Table *) this;

        return f2Table->getGlyphClass(glyphID);
    }

    default:
        return 0;
    }
}

le_bool ClassDefinitionTable::hasGlyphClass(le_int32 glyphClass)
{
    switch(SWAPW(classFormat)) {
    case 0:
        return 0;

    case 1:
    {
        ClassDefFormat1Table *f1Table = (ClassDefFormat1Table *) this;

        return f1Table->hasGlyphClass(glyphClass);
    }

    case 2:
    {
        ClassDefFormat2Table *f2Table = (ClassDefFormat2Table *) this;

        return f2Table->hasGlyphClass(glyphClass);
    }

    default:
        return 0;
    }
}

le_int32 ClassDefFormat1Table::getGlyphClass(LEGlyphID glyphID)
{
    LEGlyphID firstGlyph = SWAPW(startGlyph);
    LEGlyphID lastGlyph  = firstGlyph + SWAPW(glyphCount);

    if (glyphID > firstGlyph && glyphID < lastGlyph) {
        return SWAPW(classValueArray[glyphID - firstGlyph]);
    }

    return 0;
}

le_bool ClassDefFormat1Table::hasGlyphClass(le_int32 glyphClass)
{
    le_uint16 count  = SWAPW(glyphCount);
    int i;

    for (i = 0; i < count; i += 1) {
        if (SWAPW(classValueArray[i]) == glyphClass) {
            return true;
        }
    }

    return false;
}

le_int32 ClassDefFormat2Table::getGlyphClass(LEGlyphID glyphID)
{
    le_uint16 rangeCount = SWAPW(classRangeCount);
    le_int32  rangeIndex =
        OpenTypeUtilities::getGlyphRangeIndex(glyphID, classRangeRecordArray, rangeCount);

    if (rangeIndex < 0) {
        return 0;
    }

    return SWAPW(classRangeRecordArray[rangeIndex].rangeValue);
}

le_bool ClassDefFormat2Table::hasGlyphClass(le_int32 glyphClass)
{
    le_uint16 rangeCount = SWAPW(classRangeCount);
    int i;

    for (i = 0; i < rangeCount; i += 1) {
        if (SWAPW(classRangeRecordArray[i].rangeValue) == glyphClass) {
            return true;
        }
    }

    return false;
}
