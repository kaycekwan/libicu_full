/*
* Copyright (C) 2003, International Business Machines Corporation and others. All Rights Reserved.
********************************************************************************
*
* File JAPANCAL.H
*
* Modification History:
*
*   Date        Name        Description
*   05/13/2003  srl          copied from gregocal.h
********************************************************************************
*/

#ifndef JAPANCAL_H
#define JAPANCAL_H

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "unicode/calendar.h"
#include "unicode/gregocal.h"

U_NAMESPACE_BEGIN

/**
 * Concrete class which provides the Japanese calendar.
 * <P>
 *  <i>(stuff pasted here from java)
 * <p>
 * @internal
 */
class U_I18N_API JapaneseCalendar : public GregorianCalendar {
public:

    /**
     * Useful constants for JapaneseCalendar.  Only one Era.
     * @internal
     */
  //    enum EEras {
  //       BE
  //    };

    /**
     * Constructs a JapaneseCalendar based on the current time in the default time zone
     * with the given locale.
     *
     * @param aLocale  The given locale.
     * @param success  Indicates the status of JapaneseCalendar object construction.
     *                 Returns U_ZERO_ERROR if constructed successfully.
     * @stable ICU 2.0
     */
    JapaneseCalendar(const Locale& aLocale, UErrorCode& success);


    /**
     * Destructor
     * @internal
     */
    virtual ~JapaneseCalendar();

    /**
     * Copy constructor
     * @param source    the object to be copied.
     * @internal
     */
    JapaneseCalendar(const JapaneseCalendar& source);

    /**
     * Default assignment operator
     * @param right    the object to be copied.
     * @internal
     */
    JapaneseCalendar& operator=(const JapaneseCalendar& right);

    /**
     * Create and return a polymorphic copy of this calendar.
     * @return    return a polymorphic copy of this calendar.
     * @internal
     */
    virtual Calendar* clone(void) const;

    

public:

    /**
     * Override Calendar Returns a unique class ID POLYMORPHICALLY. Pure virtual
     * override. This method is to implement a simple version of RTTI, since not all C++
     * compilers support genuine RTTI. Polymorphic operator==() and clone() methods call
     * this method.
     *
     * @return   The class ID for this object. All objects of a given class have the
     *           same class ID. Objects of other classes have different class IDs.
     * @internal
     */
    virtual UClassID getDynamicClassID(void) const;

    /**
     * Return the class ID for this class. This is useful only for comparing to a return
     * value from getDynamicClassID(). For example:
     *
     *      Base* polymorphic_pointer = createPolymorphicObject();
     *      if (polymorphic_pointer->getDynamicClassID() ==
     *          Derived::getStaticClassID()) ...
     *
     * @return   The class ID for all objects of this class.
     * @internal
     */
    static inline UClassID getStaticClassID(void);

    /**
     * return the calendar type, "gregorian".
     *
     * @return calendar type
     * @internal
     */
    virtual const char * getType() const;

private:
    JapaneseCalendar(); // default constructor not implemented

    static const char fgClassID;

 protected:
    //    virtual int32_t monthLength(int32_t month) const; 
    //    virtual int32_t monthLength(int32_t month, int32_t year) const; 
    //    int32_t getGregorianYear(UErrorCode& status);
    //    int32_t getMaximum(UCalendarDateFields field) const;
    //    int32_t getLeastMaximum(UCalendarDateFields field) const;
    //    virtual int32_t internalGetEra() const;
    //    virtual void timeToFields(UDate theTime, UBool quick, UErrorCode& status);
};

inline UClassID
JapaneseCalendar::getStaticClassID(void)
{ return (UClassID)&fgClassID; }

inline UClassID
JapaneseCalendar::getDynamicClassID(void) const
{ return JapaneseCalendar::getStaticClassID(); }


U_NAMESPACE_END

#endif /* #if !UCONFIG_NO_FORMATTING */

#endif // _GREGOCAL
//eof

