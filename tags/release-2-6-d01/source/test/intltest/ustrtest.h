/********************************************************************
 * COPYRIGHT: 
 * Copyright (c) 1997-2002, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/

#ifndef UNICODESTRINGTEST_H
#define UNICODESTRINGTEST_H

#include "intltest.h"

/**
 * Perform API and functionality tests for class UnicodeString
 **/
class UnicodeStringTest: public IntlTest {
public:
    UnicodeStringTest() {}
    virtual ~UnicodeStringTest() {}
    
    void runIndexedTest( int32_t index, UBool exec, const char* &name, char* par = NULL );

    /**
     * Test some basic methods (insert, remove, replace, ...)
     **/
    void TestBasicManipulation(void);
    /**
     * Test the methods for comparison
     **/
    void TestCompare(void);
    /**
     * Test the methods for extracting
     **/
    void TestExtract(void);
    /**
     * More extensively test methods for removing and replacing
     **/
    void TestRemoveReplace(void);
    /**
     * Test language specific case conversions
     **/
    void TestSearching(void);
    /**
     * Test methods for padding, trimmimg and truncating
     **/
    void TestSpacePadding(void);
    /**
     * Test methods startsWith and endsWith
     **/
    void TestPrefixAndSuffix(void);
    /**
     * Test method findAndReplace
     **/
    void TestFindAndReplace(void);
    /**
     * Test method reverse
     **/
    void TestReverse(void);
    /**
     * Test a few miscellaneous methods (isBogus, hashCode,...)
     **/
    void TestMiscellaneous(void);
    /**
     * Test the functionality of allocating UnicodeStrings on the stack
     **/
    void TestStackAllocation(void);
    /**
     * Test the unescape() function.
     */
    void TestUnescape(void);

    void _testUnicodeStringHasMoreChar32Than(const UnicodeString &s, int32_t start, int32_t length, int32_t number);
    void TestCountChar32(void);
    void TestBogus();
};

class StringCaseTest: public IntlTest {
public:
    StringCaseTest() {}
    virtual ~StringCaseTest() {}
    
    void runIndexedTest(int32_t index, UBool exec, const char *&name, char *par=0);

    void TestCaseConversion();
#if !UCONFIG_NO_BREAK_ITERATION
    void TestTitleCasing();
#endif
};

#endif
