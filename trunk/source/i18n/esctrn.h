/*
**********************************************************************
*   Copyright (c) 2001-2002, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*   Date        Name        Description
*   11/20/2001  aliu        Creation.
**********************************************************************
*/
#ifndef ESCTRN_H
#define ESCTRN_H

#include "unicode/translit.h"

U_NAMESPACE_BEGIN

/**
 * A transliterator that converts Unicode characters to an escape
 * form.  Examples of escape forms are "U+4E01" and "&#x10FFFF;".
 * Escape forms have a prefix and suffix, either of which may be
 * empty, a radix, typically 16 or 10, a minimum digit count,
 * typically 1, 4, or 8, and a boolean that specifies whether
 * supplemental characters are handled as 32-bit code points or as two
 * 16-bit code units.  Most escape forms handle 32-bit code points,
 * but some, such as the Java form, intentionally break them into two
 * surrogate pairs, for backward compatibility.
 *
 * <p>Some escape forms actually have two different patterns, one for
 * BMP characters (0..FFFF) and one for supplements (>FFFF).  To
 * handle this, a second EscapeTransliterator may be defined that
 * specifies the pattern to be produced for supplementals.  An example
 * of a form that requires this is the C form, which uses "\\uFFFF"
 * for BMP characters and "\\U0010FFFF" for supplementals.
 *
 * <p>This class is package private.  It registers several standard
 * variants with the system which are then accessed via their IDs.
 *
 * @author Alan Liu
 */
class U_I18N_API EscapeTransliterator : public Transliterator {

 private:

    /**
     * The prefix of the escape form; may be empty, but usually isn't.
     */
    UnicodeString prefix;

    /**
     * The prefix of the escape form; often empty.
     */
    UnicodeString suffix;

    /**
     * The radix to display the number in.  Typically 16 or 10.  Must
     * be in the range 2 to 36.
     */
    int32_t radix;

    /**
     * The minimum number of digits.  Typically 1, 4, or 8.  Values
     * less than 1 are equivalent to 1.
     */
    int32_t minDigits;

    /**
     * If true, supplementals are handled as 32-bit code points.  If
     * false, they are handled as two 16-bit code units.
     */
    UBool grokSupplementals;

    /**
     * The form to be used for supplementals.  If this is null then
     * the same form is used for BMP characters and supplementals.  If
     * this is not null and if grokSupplementals is true then the
     * prefix, suffix, radix, and minDigits of this object are used
     * for supplementals.  This pointer is owned.
     */
    EscapeTransliterator* supplementalHandler;

    /**
     * The address of this static class variable serves as this class's ID
     * for ICU "poor man's RTTI".
     */
    static const char fgClassID;

 public:

    /**
     * Registers standard variants with the system.  Called by
     * Transliterator during initialization.
     */
    static void registerIDs();

    /**
     * Constructs an escape transliterator with the given ID and
     * parameters.  See the class member documentation for details.
     */
    EscapeTransliterator(const UnicodeString& ID,
                         const UnicodeString& prefix, const UnicodeString& suffix,
                         int32_t radix, int32_t minDigits,
                         UBool grokSupplementals,
                         EscapeTransliterator* adoptedSupplementalHandler);

    /**
     * Copy constructor.
     */
    EscapeTransliterator(const EscapeTransliterator&);

    /**
     * Destructor.
     */
    virtual ~EscapeTransliterator();

    /**
     * Transliterator API.
     */
    virtual Transliterator* clone() const;

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     *
     * @draft ICU 2.2
     */
    virtual inline UClassID getDynamicClassID() const { return getStaticClassID(); }

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     *
     * @draft ICU 2.2
     */
    static inline UClassID getStaticClassID() { return (UClassID)&fgClassID; }

 protected:

    /**
     * Implements {@link Transliterator#handleTransliterate}.
     */
    void handleTransliterate(Replaceable& text, UTransPosition& offset,
                             UBool isIncremental) const;

 private:

    /**
     * Factory methods
     */
    static Transliterator* _createUnicode(const UnicodeString& ID, Token context);
    static Transliterator* _createJava(const UnicodeString& ID, Token context);
    static Transliterator* _createC(const UnicodeString& ID, Token context);
    static Transliterator* _createXML(const UnicodeString& ID, Token context);
    static Transliterator* _createXML10(const UnicodeString& ID, Token context);
    static Transliterator* _createPerl(const UnicodeString& ID, Token context);
};

U_NAMESPACE_END

#endif
