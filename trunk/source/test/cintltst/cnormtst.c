/********************************************************************
 * COPYRIGHT: 
 * Copyright (c) 1997-2001, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
/********************************************************************************
*
* File CNORMTST.C
*
* Modification History:
*        Name                     Description            
*     Madhu Katragadda            Ported for C API
*     synwee                      added test for quick check
*     synwee                      added test for checkFCD
*********************************************************************************/
/*tests for u_normalization*/
#include <stdlib.h>
#include "unicode/utypes.h"
#include "unicode/ucol.h"
#include "unicode/uloc.h"
#include "cintltst.h"
#include "cnormtst.h"
#include "ccolltst.h"
#include "unicode/ustring.h"
#include <time.h>

#define ARRAY_LENGTH(array) (sizeof (array) / sizeof (*array))

static UCollator *myCollation;

static void
TestAPI(void);

static void
TestNormCoverage(void);

const static char* canonTests[][3] = {
    /* Input*/                    /*Decomposed*/                /*Composed*/
    { "cat",                    "cat",                        "cat"                    },
    { "\\u00e0ardvark",            "a\\u0300ardvark",            "\\u00e0ardvark",        },

    { "\\u1e0a",                "D\\u0307",                    "\\u1e0a"                }, /* D-dot_above*/
    { "D\\u0307",                "D\\u0307",                    "\\u1e0a"                }, /* D dot_above*/
    
    { "\\u1e0c\\u0307",            "D\\u0323\\u0307",            "\\u1e0c\\u0307"        }, /* D-dot_below dot_above*/
    { "\\u1e0a\\u0323",            "D\\u0323\\u0307",            "\\u1e0c\\u0307"        }, /* D-dot_above dot_below */
    { "D\\u0307\\u0323",        "D\\u0323\\u0307",            "\\u1e0c\\u0307"        }, /* D dot_below dot_above */
    
    { "\\u1e10\\u0307\\u0323",    "D\\u0327\\u0323\\u0307",    "\\u1e10\\u0323\\u0307"    }, /*D dot_below cedilla dot_above*/
    { "D\\u0307\\u0328\\u0323",    "D\\u0328\\u0323\\u0307",    "\\u1e0c\\u0328\\u0307"    }, /* D dot_above ogonek dot_below*/

    { "\\u1E14",                "E\\u0304\\u0300",            "\\u1E14"                }, /* E-macron-grave*/
    { "\\u0112\\u0300",            "E\\u0304\\u0300",            "\\u1E14"                }, /* E-macron + grave*/
    { "\\u00c8\\u0304",            "E\\u0300\\u0304",            "\\u00c8\\u0304"        }, /* E-grave + macron*/
    
    { "\\u212b",                "A\\u030a",                    "\\u00c5"                }, /* angstrom_sign*/
    { "\\u00c5",                "A\\u030a",                    "\\u00c5"                }, /* A-ring*/
    
    { "\\u00C4ffin",            "A\\u0308ffin",                "\\u00C4ffin"                    },
    { "\\u00C4\\uFB03n",        "A\\u0308\\uFB03n",            "\\u00C4\\uFB03n"                },

    { "Henry IV",                "Henry IV",                    "Henry IV"                },
    { "Henry \\u2163",            "Henry \\u2163",            "Henry \\u2163"            },

    { "\\u30AC",                "\\u30AB\\u3099",            "\\u30AC"                }, /* ga (Katakana)*/
    { "\\u30AB\\u3099",            "\\u30AB\\u3099",            "\\u30AC"                }, /*ka + ten*/
    { "\\uFF76\\uFF9E",            "\\uFF76\\uFF9E",            "\\uFF76\\uFF9E"        }, /* hw_ka + hw_ten*/
    { "\\u30AB\\uFF9E",            "\\u30AB\\uFF9E",            "\\u30AB\\uFF9E"        }, /* ka + hw_ten*/
    { "\\uFF76\\u3099",            "\\uFF76\\u3099",            "\\uFF76\\u3099"        },  /* hw_ka + ten*/
    { "A\\u0300\\u0316",           "A\\u0316\\u0300",           "\\u00C0\\u0316"        }  /* hw_ka + ten*/
};

const static char* compatTests[][3] = {
    /* Input*/                        /*Decomposed    */                /*Composed*/
    { "cat",                        "cat",                            "cat"                },

    { "\\uFB4f",                    "\\u05D0\\u05DC",                "\\u05D0\\u05DC"    }, /* Alef-Lamed vs. Alef, Lamed*/

    { "\\u00C4ffin",                "A\\u0308ffin",                    "\\u00C4ffin"             },
    { "\\u00C4\\uFB03n",            "A\\u0308ffin",                    "\\u00C4ffin"                }, /* ffi ligature -> f + f + i*/

    { "Henry IV",                    "Henry IV",                        "Henry IV"            },
    { "Henry \\u2163",                "Henry IV",                        "Henry IV"            },

    { "\\u30AC",                    "\\u30AB\\u3099",                "\\u30AC"            }, /* ga (Katakana)*/
    { "\\u30AB\\u3099",                "\\u30AB\\u3099",                "\\u30AC"            }, /*ka + ten*/
    
    { "\\uFF76\\u3099",                "\\u30AB\\u3099",                "\\u30AC"            }, /* hw_ka + ten*/

    /*These two are broken in Unicode 2.1.2 but fixed in 2.1.5 and later*/
    { "\\uFF76\\uFF9E",                "\\u30AB\\u3099",                "\\u30AC"            }, /* hw_ka + hw_ten*/
    { "\\u30AB\\uFF9E",                "\\u30AB\\u3099",                "\\u30AC"            } /* ka + hw_ten*/
    
};

void addNormTest(TestNode** root)
{
    addTest(root, &TestAPI, "tscoll/cnormtst/TestAPI");
    addTest(root, &TestDecomp, "tscoll/cnormtst/TestDecomp");
    addTest(root, &TestCompatDecomp, "tscoll/cnormtst/TestCompatDecomp");
    addTest(root, &TestCanonDecompCompose, "tscoll/cnormtst/TestCanonDecompCompose");
    addTest(root, &TestCompatDecompCompose, "tscoll/cnormtst/CompatDecompCompose");
    addTest(root, &TestNull, "tscoll/cnormtst/TestNull");
    addTest(root, &TestQuickCheck, "tscoll/cnormtst/TestQuickCheck");
    addTest(root, &TestCheckFCD, "tscoll/cnormtst/TestCheckFCD");
    addTest(root, &TestNormCoverage, "tscoll/cnormtst/TestNormCoverage");
}

void TestDecomp() 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t x, neededLen, resLen;
    UChar *source=NULL, *result=NULL; 
    status = U_ZERO_ERROR;
    myCollation = ucol_open("en_US", &status);
    if(U_FAILURE(status)){
        log_err("ERROR: in creation of rule based collator: %s\n", myErrorName(status));
        return;
    }
    resLen=0;
    log_verbose("Testing unorm_normalize with  Decomp canonical\n");
    for(x=0; x < ARRAY_LENGTH(canonTests); x++)
    {
        source=CharsToUChars(canonTests[x][0]);
        neededLen= unorm_normalize(source, u_strlen(source), UNORM_NFD, UCOL_IGNORE_HANGUL, NULL, 0, &status); 
        if(status==U_BUFFER_OVERFLOW_ERROR)
        {
            status=U_ZERO_ERROR;
            resLen=neededLen+1;
            result=(UChar*)malloc(sizeof(UChar*) * resLen);
            unorm_normalize(source, u_strlen(source), UNORM_NFD, UCOL_IGNORE_HANGUL, result, resLen, &status); 
        }
        if(U_FAILURE(status)){
            log_err("ERROR in unorm_normalize at %s:  %s\n", austrdup(source), myErrorName(status) );
        }
        assertEqual(result, canonTests[x][1], x);
        free(result);
        free(source);
    }
    ucol_close(myCollation);
}

void TestCompatDecomp() 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t x, neededLen, resLen;
    UChar *source=NULL, *result=NULL; 
    status = U_ZERO_ERROR;
    myCollation = ucol_open("en_US", &status);
    if(U_FAILURE(status)){
        log_err("ERROR: in creation of rule based collator: %s\n", myErrorName(status));
        return;
    }
    resLen=0;
    log_verbose("Testing unorm_normalize with  Decomp compat\n");
    for(x=0; x < ARRAY_LENGTH(compatTests); x++)
    {
        source=CharsToUChars(compatTests[x][0]);
        neededLen= unorm_normalize(source, u_strlen(source), UNORM_NFKD, UCOL_IGNORE_HANGUL, NULL, 0, &status); 
        if(status==U_BUFFER_OVERFLOW_ERROR)
        {
            status=U_ZERO_ERROR;
            resLen=neededLen+1;
            result=(UChar*)malloc(sizeof(UChar*) * resLen);
            unorm_normalize(source, u_strlen(source), UNORM_NFKD,UCOL_IGNORE_HANGUL, result, resLen, &status); 
        }
        if(U_FAILURE(status)){
            log_err("ERROR in unorm_normalize at %s:  %s\n", austrdup(source), myErrorName(status) );
        }
        assertEqual(result, compatTests[x][1], x);
        free(result);
        free(source);
    }
    ucol_close(myCollation);            
}

void TestCanonDecompCompose() 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t x, neededLen, resLen;
    UChar *source=NULL, *result=NULL; 
    status = U_ZERO_ERROR;
    myCollation = ucol_open("en_US", &status);
    if(U_FAILURE(status)){
        log_err("ERROR: in creation of rule based collator: %s\n", myErrorName(status));
        return;
    }
    resLen=0;
    log_verbose("Testing unorm_normalize with Decomp can compose compat\n");
    for(x=0; x < ARRAY_LENGTH(canonTests); x++)
    {
        source=CharsToUChars(canonTests[x][0]);
        neededLen= unorm_normalize(source, u_strlen(source), UNORM_NFC, UCOL_IGNORE_HANGUL, NULL, 0, &status); 
        if(status==U_BUFFER_OVERFLOW_ERROR)
        {
            status=U_ZERO_ERROR;
            resLen=neededLen+1;
            result=(UChar*)malloc(sizeof(UChar*) * resLen);
            unorm_normalize(source, u_strlen(source), UNORM_NFC, UCOL_IGNORE_HANGUL, result, resLen, &status); 
        }
        if(U_FAILURE(status)){
            log_err("ERROR in unorm_normalize at %s:  %s\n", austrdup(source),myErrorName(status) );
        }
        assertEqual(result, canonTests[x][2], x);
        free(result);
        free(source);
    }
    ucol_close(myCollation);            
}

void TestCompatDecompCompose() 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t x, neededLen, resLen;
    UChar *source=NULL, *result=NULL;
    status = U_ZERO_ERROR;
    myCollation = ucol_open("en_US", &status);
    if(U_FAILURE(status)){
        log_err("ERROR: in creation of rule based collator: %s\n", myErrorName(status));
        return;
    }
    resLen=0;
    log_verbose("Testing unorm_normalize with compat decomp compose can\n");
    for(x=0; x < ARRAY_LENGTH(compatTests); x++)
    {
        source=CharsToUChars(compatTests[x][0]);
        neededLen= unorm_normalize(source, u_strlen(source), UNORM_NFKC, UCOL_IGNORE_HANGUL, NULL, 0, &status); 
        if(status==U_BUFFER_OVERFLOW_ERROR)
        {
            status=U_ZERO_ERROR;
            resLen=neededLen+1;
            result=(UChar*)malloc(sizeof(UChar*) * resLen);
            unorm_normalize(source, u_strlen(source), UNORM_NFKC, UCOL_IGNORE_HANGUL, result, resLen, &status); 
        }
        if(U_FAILURE(status)){
            log_err("ERROR in unorm_normalize at %s:  %s\n", austrdup(source), myErrorName(status) );
        }
        assertEqual(result, compatTests[x][2], x);
        free(result);
        free(source);
    }
    ucol_close(myCollation);            
}


/*
static void assertEqual(const UChar* result, const UChar* expected, int32_t index)
{
    if(u_strcmp(result, expected)!=0){
        log_err("ERROR in decomposition at index = %d. EXPECTED: %s , GOT: %s\n", index, austrdup(expected),
            austrdup(result) );
    }
}
*/

static void assertEqual(const UChar* result, const char* expected, int32_t index)
{
    UChar *expectedUni = CharsToUChars(expected);
    if(u_strcmp(result, expectedUni)!=0){
        log_err("ERROR in decomposition at index = %d. EXPECTED: %s , GOT: %s\n", index, expected,
            austrdup(result) );
    }
    free(expectedUni);
}

static void TestNull_check(UChar *src, int32_t srcLen, 
                    UChar *exp, int32_t expLen,
                    UNormalizationMode mode,
                    const char *name)
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t len, i;

    UChar   result[50];


    status = U_ZERO_ERROR;

    for(i=0;i<50;i++)
      {
        result[i] = 0xFFFD;
      }

    len = unorm_normalize(src, srcLen, mode, 0, result, 50, &status); 

    if(U_FAILURE(status)) {
      log_err("unorm_normalize(%s) with 0x0000 failed: %s\n", name, u_errorName(status));
    } else if (len != expLen) {
      log_err("unorm_normalize(%s) with 0x0000 failed: Expected len %d, got %d\n", name, expLen, len);
    } 

    {
      for(i=0;i<len;i++){
        if(exp[i] != result[i]) {
          log_err("unorm_normalize(%s): @%d, expected \\u%04X got \\u%04X\n",
                  name,
                  i,
                  exp[i],
                  result[i]);
          return;
        }
        log_verbose("     %d: \\u%04X\n", i, result[i]);
      }
    }
    
    log_verbose("unorm_normalize(%s) with 0x0000: OK\n", name);
}

void TestNull() 
{

    UChar   source_comp[] = { 0x0061, 0x0000, 0x0044, 0x0307 };
    int32_t source_comp_len = 4;
    UChar   expect_comp[] = { 0x0061, 0x0000, 0x1e0a };
    int32_t expect_comp_len = 3;

    UChar   source_dcmp[] = { 0x1e0A, 0x0000, 0x0929 };
    int32_t source_dcmp_len = 3;
    UChar   expect_dcmp[] = { 0x0044, 0x0307, 0x0000, 0x0928, 0x093C };
    int32_t expect_dcmp_len = 5;
    
    TestNull_check(source_comp,
                   source_comp_len,
                   expect_comp,
                   expect_comp_len,
                   UNORM_NFC,
                   "UNORM_NFC");

    TestNull_check(source_dcmp,
                   source_dcmp_len,
                   expect_dcmp,
                   expect_dcmp_len,
                   UNORM_NFD,
                   "UNORM_NFD");

    TestNull_check(source_comp,
                   source_comp_len,
                   expect_comp,
                   expect_comp_len,
                   UNORM_NFKC,
                   "UNORM_NFKC");


}

static void TestQuickCheckResultNO() 
{
  const UChar CPNFD[] = {0x00C5, 0x0407, 0x1E00, 0x1F57, 0x220C, 
                         0x30AE, 0xAC00, 0xD7A3, 0xFB36, 0xFB4E};
  const UChar CPNFC[] = {0x0340, 0x0F93, 0x1F77, 0x1FBB, 0x1FEB, 
                          0x2000, 0x232A, 0xF900, 0xFA1E, 0xFB4E};
  const UChar CPNFKD[] = {0x00A0, 0x02E4, 0x1FDB, 0x24EA, 0x32FE, 
                           0xAC00, 0xFB4E, 0xFA10, 0xFF3F, 0xFA2D};
  const UChar CPNFKC[] = {0x00A0, 0x017F, 0x2000, 0x24EA, 0x32FE, 
                           0x33FE, 0xFB4E, 0xFA10, 0xFF3F, 0xFA2D};


  const int SIZE = 10;

  int count = 0;
  UErrorCode error = U_ZERO_ERROR;

  for (; count < SIZE; count ++)
  {
    if (unorm_quickCheck(&(CPNFD[count]), 1, UNORM_NFD, &error) != 
                                                              UNORM_NO)
    {
      log_err("ERROR in NFD quick check at U+%04x\n", CPNFD[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFC[count]), 1, UNORM_NFC, &error) != 
                                                              UNORM_NO)
    {
      log_err("ERROR in NFC quick check at U+%04x\n", CPNFC[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFKD[count]), 1, UNORM_NFKD, &error) != 
                                                              UNORM_NO)
    {
      log_err("ERROR in NFKD quick check at U+%04x\n", CPNFKD[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFKC[count]), 1, UNORM_NFKC, &error) != 
                                                              UNORM_NO)
    {
      log_err("ERROR in NFKC quick check at U+%04x\n", CPNFKC[count]);
      return;
    }
  }
}

 
static void TestQuickCheckResultYES() 
{
  const UChar CPNFD[] = {0x00C6, 0x017F, 0x0F74, 0x1000, 0x1E9A, 
                         0x2261, 0x3075, 0x4000, 0x5000, 0xF000};
  const UChar CPNFC[] = {0x0400, 0x0540, 0x0901, 0x1000, 0x1500, 
                         0x1E9A, 0x3000, 0x4000, 0x5000, 0xF000};
  const UChar CPNFKD[] = {0x00AB, 0x02A0, 0x1000, 0x1027, 0x2FFB, 
                          0x3FFF, 0x4FFF, 0xA000, 0xF000, 0xFA27};
  const UChar CPNFKC[] = {0x00B0, 0x0100, 0x0200, 0x0A02, 0x1000, 
                          0x2010, 0x3030, 0x4000, 0xA000, 0xFA0E};

  const int SIZE = 10;
  int count = 0;
  UErrorCode error = U_ZERO_ERROR;

  UChar cp = 0;
  while (cp < 0xA0)
  {
    if (unorm_quickCheck(&cp, 1, UNORM_NFD, &error) != UNORM_YES)
    {
      log_err("ERROR in NFD quick check at U+%04x\n", cp);
      return;
    }
    if (unorm_quickCheck(&cp, 1, UNORM_NFC, &error) != 
                                                             UNORM_YES)
    {
      log_err("ERROR in NFC quick check at U+%04x\n", cp);
      return;
    }
    if (unorm_quickCheck(&cp, 1, UNORM_NFKD, &error) != UNORM_YES)
    {
      log_err("ERROR in NFKD quick check at U+%04x\n", cp);
      return;
    }
    if (unorm_quickCheck(&cp, 1, UNORM_NFKC, &error) != 
                                                             UNORM_YES)
    {
      log_err("ERROR in NFKC quick check at U+%04x\n", cp);
      return;
    }
    cp ++;
  }

  for (; count < SIZE; count ++)
  {
    if (unorm_quickCheck(&(CPNFD[count]), 1, UNORM_NFD, &error) != 
                                                             UNORM_YES)
    {
      log_err("ERROR in NFD quick check at U+%04x\n", CPNFD[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFC[count]), 1, UNORM_NFC, &error) 
                                                          != UNORM_YES)
    {
      log_err("ERROR in NFC quick check at U+%04x\n", CPNFC[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFKD[count]), 1, UNORM_NFKD, &error) != 
                                                             UNORM_YES)
    {
      log_err("ERROR in NFKD quick check at U+%04x\n", CPNFKD[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFKC[count]), 1, UNORM_NFKC, &error) != 
                                                             UNORM_YES)
    {
      log_err("ERROR in NFKC quick check at U+%04x\n", CPNFKC[count]);
      return;
    }
  }
}

static void TestQuickCheckResultMAYBE() 
{
  const UChar CPNFC[] = {0x0306, 0x0654, 0x0BBE, 0x102E, 0x1161, 
                         0x116A, 0x1173, 0x1175, 0x3099, 0x309A};
  const UChar CPNFKC[] = {0x0300, 0x0654, 0x0655, 0x09D7, 0x0B3E, 
                          0x0DCF, 0xDDF, 0x102E, 0x11A8, 0x3099};


  const int SIZE = 10;

  int count = 0;
  UErrorCode error = U_ZERO_ERROR;

  /* NFD and NFKD does not have any MAYBE codepoints */
  for (; count < SIZE; count ++)
  {
    if (unorm_quickCheck(&(CPNFC[count]), 1, UNORM_NFC, &error) != 
                                                           UNORM_MAYBE)
    {
      log_err("ERROR in NFC quick check at U+%04x\n", CPNFC[count]);
      return;
    }
    if (unorm_quickCheck(&(CPNFKC[count]), 1, UNORM_NFKC, &error) != 
                                                           UNORM_MAYBE)
    {
      log_err("ERROR in NFKC quick check at U+%04x\n", CPNFKC[count]);
      return;
    }
  }
}

static void TestQuickCheckStringResult() 
{
  int count;
  UChar *d = NULL;
  UChar *c = NULL;
  UErrorCode error = U_ZERO_ERROR;

  for (count = 0; count < ARRAY_LENGTH(canonTests); count ++)
  {
    d = CharsToUChars(canonTests[count][1]);
    c = CharsToUChars(canonTests[count][2]);
    if (unorm_quickCheck(d, u_strlen(d), UNORM_NFD, &error) != 
                                                            UNORM_YES)
    {
      log_err("ERROR in NFD quick check for string at count %d\n", count);
      return;
    }

    if (unorm_quickCheck(c, u_strlen(c), UNORM_NFC, &error) == 
                                                            UNORM_NO)
    {
      log_err("ERROR in NFC quick check for string at count %d\n", count);
      return;
    }

    free(d);
    free(c);
  }

  for (count = 0; count < ARRAY_LENGTH(compatTests); count ++)
  {
    d = CharsToUChars(compatTests[count][1]);
    c = CharsToUChars(compatTests[count][2]);
    if (unorm_quickCheck(d, u_strlen(d), UNORM_NFKD, &error) != 
                                                            UNORM_YES)
    {
      log_err("ERROR in NFKD quick check for string at count %d\n", count);
      return;
    }

    if (unorm_quickCheck(c, u_strlen(c), UNORM_NFKC, &error) != 
                                                            UNORM_YES)
    {
      log_err("ERROR in NFKC quick check for string at count %d\n", count);
      return;
    }

    free(d);
    free(c);
  }  
}

void TestQuickCheck() 
{
  TestQuickCheckResultNO();
  TestQuickCheckResultYES();
  TestQuickCheckResultMAYBE();
  TestQuickCheckStringResult(); 
}

void TestCheckFCD() 
{
  UErrorCode status = U_ZERO_ERROR;
  static const UChar FAST_[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
                         0x0A}; 
  static const UChar FALSE_[] = {0x0001, 0x0002, 0x02EA, 0x03EB, 0x0300, 0x0301, 
                          0x02B9, 0x0314, 0x0315, 0x0316};
  static const UChar TRUE_[] = {0x0030, 0x0040, 0x0440, 0x056D, 0x064F, 0x06E7, 
                         0x0050, 0x0730, 0x09EE, 0x1E10};

  static const UChar datastr[][5] = 
  { {0x0061, 0x030A, 0x1E05, 0x0302, 0},
    {0x0061, 0x030A, 0x00E2, 0x0323, 0},
    {0x0061, 0x0323, 0x00E2, 0x0323, 0},
    {0x0061, 0x0323, 0x1E05, 0x0302, 0} };
  static const UBool result[] = {UNORM_YES, UNORM_NO, UNORM_NO, UNORM_YES};

  static const UChar datachar[] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
                            0x6a,
                            0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 
                            0xea,
                            0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0305, 0x0306, 
                            0x0307, 0x0308, 0x0309, 0x030a, 
                            0x0320, 0x0321, 0x0322, 0x0323, 0x0324, 0x0325, 0x0326,
                            0x0327, 0x0328, 0x0329, 0x032a,
                            0x1e00, 0x1e01, 0x1e02, 0x1e03, 0x1e04, 0x1e05, 0x1e06, 
                            0x1e07, 0x1e08, 0x1e09, 0x1e0a};

  int count = 0;
  
  if (unorm_quickCheck(FAST_, 10, UNORM_FCD, &status) != UNORM_YES)
    log_err("unorm_quickCheck(FCD) failed: expected value for fast unorm_quickCheck is UNORM_YES\n");
  if (unorm_quickCheck(FALSE_, 10, UNORM_FCD, &status) != UNORM_NO)
    log_err("unorm_quickCheck(FCD) failed: expected value for error unorm_quickCheck is UNORM_NO\n");
  if (unorm_quickCheck(TRUE_, 10, UNORM_FCD, &status) != UNORM_YES)
    log_err("unorm_quickCheck(FCD) failed: expected value for correct unorm_quickCheck is UNORM_YES\n");

  if (U_FAILURE(status))
    log_err("unorm_quickCheck(FCD) failed: %s\n", u_errorName(status));

  while (count < 4)
  {
    UBool fcdresult = unorm_quickCheck(datastr[count], 4, UNORM_FCD, &status);
    if (U_FAILURE(status)) {
      log_err("unorm_quickCheck(FCD) failed: exception occured at data set %d\n", count);
      break;
    }
    else {
      if (result[count] != fcdresult) {
        log_err("unorm_quickCheck(FCD) failed: Data set %d expected value %d\n", count, 
                 result[count]);
      }
    }
    count ++;
  }

  /* random checks of long strings */
  status = U_ZERO_ERROR;
  srand((unsigned)time( NULL ));

  for (count = 0; count < 50; count ++)
  {
    int size = 0;
    UBool testresult = UNORM_YES;
    UChar data[20];
    UChar norm[100];
    UChar nfd[100];
    int normsize = 0;
    int nfdsize = 0;
    
    while (size != 19) {
      data[size] = datachar[(rand() * 50) / RAND_MAX];
      log_verbose("0x%x", data[size]);
      normsize += unorm_normalize(data + size, 1, UNORM_NFD, UCOL_IGNORE_HANGUL, 
                                  norm + normsize, 100 - normsize, &status);       
      if (U_FAILURE(status)) {
        log_err("unorm_quickCheck(FCD) failed: exception occured at data generation\n");
        break;
      }
      size ++;
    }
    log_verbose("\n");

    nfdsize = unorm_normalize(data, size, UNORM_NFD, UCOL_IGNORE_HANGUL, 
                              nfd, 100, &status);       
    if (U_FAILURE(status)) {
      log_err("unorm_quickCheck(FCD) failed: exception occured at normalized data generation\n");
    }

    if (nfdsize != normsize || u_memcmp(nfd, norm, nfdsize) != 0) {
      testresult = UNORM_NO;
    }
    if (testresult == UNORM_YES) {
      log_verbose("result UNORM_YES\n");
    }
    else {
      log_verbose("result UNORM_NO\n");
    }

    if (unorm_quickCheck(data, size, UNORM_FCD, &status) != testresult || U_FAILURE(status)) {
      log_err("unorm_quickCheck(FCD) failed: expected %d for random data\n", testresult);
    }
  }
}

static void
TestAPI() {
    static const UChar in[]={ 0x68, 0xe4 };
    UChar out[20]={ 0xffff, 0xffff, 0xffff, 0xffff };
    UErrorCode errorCode;
    int32_t length;

    /* try preflighting */
    errorCode=U_ZERO_ERROR;
    length=unorm_normalize(in, 2, UNORM_NFD, 0, NULL, 0, &errorCode);
    if(errorCode!=U_BUFFER_OVERFLOW_ERROR || length!=3) {
        log_err("unorm_normalize(pure preflighting NFD)=%ld failed with %s\n", length, u_errorName(errorCode));
        return;
    }

    errorCode=U_ZERO_ERROR;
    length=unorm_normalize(in, 2, UNORM_NFD, 0, out, 3, &errorCode);
    if(U_FAILURE(errorCode)) {
        log_err("unorm_normalize(NFD)=%ld failed with %s\n", length, u_errorName(errorCode));
        return;
    }
    if(length!=3 || out[2]!=0x308 || out[3]!=0xffff) {
        log_err("unorm_normalize(NFD ma<umlaut>)=%ld failed with out[]=U+%04x U+%04x U+%04x U+%04x\n", length, out[0], out[1], out[2], out[3]);
        return;
    }
}

/* test cases to improve test code coverage */
enum {
    HANGUL_K_KIYEOK=0x3131,         /* NFKD->Jamo L U+1100 */
    HANGUL_K_WEO=0x315d,            /* NFKD->Jamo V U+116f */
    HANGUL_K_KIYEOK_SIOS=0x3133,    /* NFKD->Jamo T U+11aa */

    HANGUL_KIYEOK=0x1100,           /* Jamo L U+1100 */
    HANGUL_WEO=0x116f,              /* Jamo V U+116f */
    HANGUL_KIYEOK_SIOS=0x11aa,      /* Jamo T U+11aa */

    HANGUL_AC00=0xac00,             /* Hangul syllable = Jamo LV U+ac00 */
    HANGUL_SYLLABLE=0xac00+14*28+3, /* Hangul syllable = U+1100 * U+116f * U+11aa */

    MUSICAL_VOID_NOTEHEAD=0x1d157,
    MUSICAL_HALF_NOTE=0x1d15e,  /* NFC/NFD->Notehead+Stem */
    MUSICAL_STEM=0x1d165,       /* cc=216 */
    MUSICAL_STACCATO=0x1d17c    /* cc=220 */
};

static void
TestNormCoverage() {
    static UChar input[2000], expect[3000], output[3000];
    UErrorCode errorCode;
    int32_t i, length, inLength, expectLength, hangulPrefixLength, preflightLength;

    /* create a long and nasty string with NFKC-unsafe characters */
    inLength=0;

    /* 3 Jamos L/V/T, all 8 combinations normal/compatibility */
    input[inLength++]=HANGUL_KIYEOK;
    input[inLength++]=HANGUL_WEO;
    input[inLength++]=HANGUL_KIYEOK_SIOS;

    input[inLength++]=HANGUL_KIYEOK;
    input[inLength++]=HANGUL_WEO;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    input[inLength++]=HANGUL_KIYEOK;
    input[inLength++]=HANGUL_K_WEO;
    input[inLength++]=HANGUL_KIYEOK_SIOS;

    input[inLength++]=HANGUL_KIYEOK;
    input[inLength++]=HANGUL_K_WEO;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_WEO;
    input[inLength++]=HANGUL_KIYEOK_SIOS;

    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_WEO;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_K_WEO;
    input[inLength++]=HANGUL_KIYEOK_SIOS;

    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_K_WEO;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    /* Hangul LV with normal/compatibility Jamo T */
    input[inLength++]=HANGUL_AC00;
    input[inLength++]=HANGUL_KIYEOK_SIOS;

    input[inLength++]=HANGUL_AC00;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    /* compatibility Jamo L, V */
    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_K_WEO;

    hangulPrefixLength=inLength;

    input[inLength++]=UTF16_LEAD(MUSICAL_HALF_NOTE);
    input[inLength++]=UTF16_TRAIL(MUSICAL_HALF_NOTE);
    for(i=0; i<200; ++i) {
        input[inLength++]=UTF16_LEAD(MUSICAL_STACCATO);
        input[inLength++]=UTF16_TRAIL(MUSICAL_STACCATO);
        input[inLength++]=UTF16_LEAD(MUSICAL_STEM);
        input[inLength++]=UTF16_TRAIL(MUSICAL_STEM);
    }

    /* (compatibility) Jamo L, T do not compose */
    input[inLength++]=HANGUL_K_KIYEOK;
    input[inLength++]=HANGUL_K_KIYEOK_SIOS;

    /* quick checks */
    errorCode=U_ZERO_ERROR;
    if(UNORM_NO!=unorm_quickCheck(input, inLength, UNORM_NFD, &errorCode) || U_FAILURE(errorCode)) {
        log_err("error unorm_quickCheck(long input, UNORM_NFD)!=NO (%s)\n", u_errorName(errorCode));
    }
    errorCode=U_ZERO_ERROR;
    if(UNORM_NO!=unorm_quickCheck(input, inLength, UNORM_NFKD, &errorCode) || U_FAILURE(errorCode)) {
        log_err("error unorm_quickCheck(long input, UNORM_NFKD)!=NO (%s)\n", u_errorName(errorCode));
    }
    errorCode=U_ZERO_ERROR;
    if(UNORM_NO!=unorm_quickCheck(input, inLength, UNORM_NFC, &errorCode) || U_FAILURE(errorCode)) {
        log_err("error unorm_quickCheck(long input, UNORM_NFC)!=NO (%s)\n", u_errorName(errorCode));
    }
    errorCode=U_ZERO_ERROR;
    if(UNORM_NO!=unorm_quickCheck(input, inLength, UNORM_NFKC, &errorCode) || U_FAILURE(errorCode)) {
        log_err("error unorm_quickCheck(long input, UNORM_NFKC)!=NO (%s)\n", u_errorName(errorCode));
    }
    errorCode=U_ZERO_ERROR;
    if(UNORM_NO!=unorm_quickCheck(input, inLength, UNORM_FCD, &errorCode) || U_FAILURE(errorCode)) {
        log_err("error unorm_quickCheck(long input, UNORM_FCD)!=NO (%s)\n", u_errorName(errorCode));
    }

    /* NFKC */
    expectLength=0;
    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_SYLLABLE;

    expect[expectLength++]=HANGUL_AC00+3;

    expect[expectLength++]=HANGUL_AC00+3;

    expect[expectLength++]=HANGUL_AC00+14*28;

    expect[expectLength++]=UTF16_LEAD(MUSICAL_VOID_NOTEHEAD);
    expect[expectLength++]=UTF16_TRAIL(MUSICAL_VOID_NOTEHEAD);
    expect[expectLength++]=UTF16_LEAD(MUSICAL_STEM);
    expect[expectLength++]=UTF16_TRAIL(MUSICAL_STEM);
    for(i=0; i<200; ++i) {
        expect[expectLength++]=UTF16_LEAD(MUSICAL_STEM);
        expect[expectLength++]=UTF16_TRAIL(MUSICAL_STEM);
    }
    for(i=0; i<200; ++i) {
        expect[expectLength++]=UTF16_LEAD(MUSICAL_STACCATO);
        expect[expectLength++]=UTF16_TRAIL(MUSICAL_STACCATO);
    }

    expect[expectLength++]=HANGUL_KIYEOK;
    expect[expectLength++]=HANGUL_KIYEOK_SIOS;

    /* try destination overflow first */
    errorCode=U_ZERO_ERROR;
    preflightLength=unorm_normalize(input, inLength,
                           UNORM_NFKC, 0,
                           output, 100, /* too short */
                           &errorCode);
    if(errorCode!=U_BUFFER_OVERFLOW_ERROR) {
        log_err("error unorm_normalize(long input, output too short, UNORM_NFKC) did not overflow but %s\n", u_errorName(errorCode));
    }

    /* real NFKC */
    errorCode=U_ZERO_ERROR;
    length=unorm_normalize(input, inLength,
                           UNORM_NFKC, 0,
                           output, sizeof(output)/U_SIZEOF_UCHAR,
                           &errorCode);
    if(U_FAILURE(errorCode)) {
        log_err("error unorm_normalize(long input, UNORM_NFKC) failed with %s\n", u_errorName(errorCode));
    } else if(length!=expectLength || u_memcmp(output, expect, length)!=0) {
        log_err("error unorm_normalize(long input, UNORM_NFKC) produced wrong result\n");
        for(i=0; i<length; ++i) {
            if(output[i]!=expect[i]) {
                log_err("    NFKC[%d]==U+%04lx expected U+%04lx\n", i, output[i], expect[i]);
                break;
            }
        }
    }
    if(length!=preflightLength) {
        log_err("error unorm_normalize(long input, UNORM_NFKC)==%ld but preflightLength==%ld\n", length, preflightLength);
    }

    /* FCD */
    u_memcpy(expect, input, hangulPrefixLength);
    expectLength=hangulPrefixLength;

    expect[expectLength++]=UTF16_LEAD(MUSICAL_VOID_NOTEHEAD);
    expect[expectLength++]=UTF16_TRAIL(MUSICAL_VOID_NOTEHEAD);
    expect[expectLength++]=UTF16_LEAD(MUSICAL_STEM);
    expect[expectLength++]=UTF16_TRAIL(MUSICAL_STEM);
    for(i=0; i<200; ++i) {
        expect[expectLength++]=UTF16_LEAD(MUSICAL_STEM);
        expect[expectLength++]=UTF16_TRAIL(MUSICAL_STEM);
    }
    for(i=0; i<200; ++i) {
        expect[expectLength++]=UTF16_LEAD(MUSICAL_STACCATO);
        expect[expectLength++]=UTF16_TRAIL(MUSICAL_STACCATO);
    }

    expect[expectLength++]=HANGUL_K_KIYEOK;
    expect[expectLength++]=HANGUL_K_KIYEOK_SIOS;

    errorCode=U_ZERO_ERROR;
    length=unorm_normalize(input, inLength,
                           UNORM_FCD, 0,
                           output, sizeof(output)/U_SIZEOF_UCHAR,
                           &errorCode);
    if(U_FAILURE(errorCode)) {
        log_err("error unorm_normalize(long input, UNORM_FCD) failed with %s\n", u_errorName(errorCode));
    } else if(length!=expectLength || u_memcmp(output, expect, length)!=0) {
        log_err("error unorm_normalize(long input, UNORM_FCD) produced wrong result\n");
        for(i=0; i<length; ++i) {
            if(output[i]!=expect[i]) {
                log_err("    FCD[%d]==U+%04lx expected U+%04lx\n", i, output[i], expect[i]);
                break;
            }
        }
    }
}
