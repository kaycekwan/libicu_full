/*
**********************************************************************
*   Copyright (C) 2001, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*   Date        Name        Description
*   05/24/01    aliu        Creation.
**********************************************************************
*/

#include "tolowtrn.h"
#include "unicode/ustring.h"
#include "ustr_imp.h"
#include "cpputils.h"
#include "unicode/uchar.h"

U_NAMESPACE_BEGIN

const char LowercaseTransliterator::_ID[] = "Any-Lower";

/**
 * Constructs a transliterator.
 */
LowercaseTransliterator::LowercaseTransliterator(const Locale& theLoc) : Transliterator(_ID, 0),
    loc(theLoc) , buffer(0) {
    buffer = new UChar[u_getMaxCaseExpansion()];
}

/**
 * Destructor.
 */
LowercaseTransliterator::~LowercaseTransliterator() {
    delete [] buffer;
}

/**
 * Copy constructor.
 */
LowercaseTransliterator::LowercaseTransliterator(const LowercaseTransliterator& o) :
    Transliterator(o),
    loc(o.loc), buffer(0) {
    buffer = new UChar[u_getMaxCaseExpansion()];
}

/**
 * Assignment operator.
 */
LowercaseTransliterator& LowercaseTransliterator::operator=(
                             const LowercaseTransliterator& o) {
    Transliterator::operator=(o);
    loc = o.loc;
    uprv_arrayCopy((const UChar*)o.buffer, 0, this->buffer, 0, u_getMaxCaseExpansion());
    return *this;
}

/**
 * Transliterator API.
 */
Transliterator* LowercaseTransliterator::clone(void) const {
    return new LowercaseTransliterator(*this);
}

/**
 * Implements {@link Transliterator#handleTransliterate}.
 */
void LowercaseTransliterator::handleTransliterate(Replaceable& text,
                                 UTransPosition& offsets, 
                                 UBool isIncremental) const
{
    int32_t textPos = offsets.start;
    if (textPos >= offsets.limit) return;

    // get string for context

    UnicodeString original;
    text.extractBetween(offsets.contextStart, offsets.contextLimit, original);
    
    UCharIterator iter;
    uiter_setReplaceable(&iter, &text);
    iter.start = offsets.contextStart;
    iter.limit = offsets.contextLimit;

    // Walk through original string
    // If there is a case change, modify corresponding position in replaceable
    
    int32_t i = textPos - offsets.contextStart;
    int32_t limit = offsets.limit - offsets.contextStart;
    UChar32 cp;
    int32_t oldLen;
    
    for (; i < limit; ) { 
        UTF_GET_CHAR(original.getBuffer(), 0, i, original.length(), cp);
        oldLen = UTF_CHAR_LENGTH(cp);
        i += oldLen;
        iter.index = i; // Point _past_ current char
        int32_t newLen = u_internalToLower(cp, &iter, buffer, u_getMaxCaseExpansion(), loc.getName());
        if (newLen >= 0) {
            UnicodeString temp(buffer, newLen);
            text.handleReplaceBetween(textPos, textPos + oldLen, temp);
            if (newLen != oldLen) {
                textPos += newLen;
                offsets.limit += newLen - oldLen;
                offsets.contextLimit += newLen - oldLen;
                continue;
            }
        }
        textPos += oldLen;
    }
    offsets.start = offsets.limit;
}
U_NAMESPACE_END

