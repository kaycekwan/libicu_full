/*
**********************************************************************
*   Copyright (C) 1995-1999, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
 * @version 1.0 23/10/96
 * @author  Helena Shih
 * Based on Taligent international support for java
 * Modification History : 
 *
 * 05/07/97     helena      Added isBogus()
 * 07/15/98     erm         Synched with Java 1.2 CompactShortArray.java.
 * 07/30/98     erm         Added 07/29/98 code review changes.
 * 04/21/99     Damiba      Port to C/New API faster ucmp16_get
 */

#ifndef UCMP16_H
#define UCMP16_H


#include "umemstrm.h"
#include "unicode/utypes.h"

/* 32-bits.
  Bump this whenever the internal structure changes.
*/
#define ICU_UCMP16_VERSION 0x01270000

/* internal constants*/
#define UCMP16_kMaxUnicode_int 65535
#define UCMP16_kUnicodeCount_int (UCMP16_kMaxUnicode_int + 1)
#define UCMP16_kBlockShift_int 7
#define UCMP16_kBlockCount_int (1 << UCMP16_kBlockShift_int)
#define UCMP16_kBlockBytes_int (UCMP16_kBlockCount_int * sizeof(int16_t))
#define UCMP16_kIndexShift_int (16 - UCMP16_kBlockShift_int)
#define UCMP16_kIndexCount_int (1 << UCMP16_kIndexShift_int)
#define UCMP16_kBlockMask_int (UCMP16_kBlockCount_int - 1)

/**
 * class CompactATypeArray : use only on primitive data types
 * Provides a compact way to store information that is indexed by Unicode
 * values, such as character properties, types, keyboard values, etc.This
 * is very useful when you have a block of Unicode data that contains
 * significant values while the rest of the Unicode data is unused in the
 * application or when you have a lot of redundance, such as where all 21,000
 * Han ideographs have the same value.  However, lookup is much faster than a
 * hash table.
 * <P>
 * A compact array of any primitive data type serves two purposes:
 * <UL type = round>
 *     <LI>Fast access of the indexed values.
 *     <LI>Smaller memory footprint.
 * </UL>
 * <P>
 * The index array always points into particular parts of the data array
 * it is initially set up to point at regular block boundaries
 * The following example uses blocks of 4 for simplicity
 * <PRE>
 * Example: Expanded
 * BLOCK  0   1   2   3   4
 * INDEX  0   4   8   12  16 ...
 * ARRAY  abcdeababcdezyabcdea...
 *        |   |   |   |   |   |...
 * </PRE>
 * <P>
 * After compression, the index will point to various places in the data array
 * wherever there is a runs of the same elements as in the original
 * <PRE>
 * Example: Compressed
 * BLOCK  0   1   2   3   4
 * INDEX  0   4   1   8   2 ...
 * ARRAY  abcdeabazyabc...
 * </PRE>
 * <P>
 * If you look at the example, index number 2 in the expanded version points
 * to data position number 8, which has elements "bcde". In the compressed
 * version, index number 2 points to data position 1, which also has "bcde"
 * @see                CompactByteArray
 * @see                CompactIntArray
 * @see                CompactCharArray
 * @see                CompactStringArray
 * @version            $Revision: 1.16 $ 8/25/98
 * @author             Helena Shih
 */

typedef struct CompactShortArray {
  int32_t fStructSize;
  int16_t* fArray;
  uint16_t* fIndex;
  int32_t* fHashes;
  int32_t fCount;
  int16_t fDefaultValue;
  UBool fCompact;    
  UBool fBogus;
  UBool fAlias;
  int32_t kBlockShift;
  int32_t kBlockMask;
  UBool fIAmOwned; /* don't free CSA on close */
} CompactShortArray;


U_CAPI int32_t U_EXPORT2 ucmp16_getkUnicodeCount(void);
U_CAPI int32_t U_EXPORT2 ucmp16_getkBlockCount(void);

/**
 * Construct an empty CompactShortArray with uprv_malloc(). Do not call any of the
 * ucmp16_init*() functions after using this function. They will cause a memory
 * leak.
 *
 * @param defaultValue the default value for all characters not explicitly in the array
 * @see ucmp16_init
 * @see ucmp16_initBogus
 * @return The initialized array.
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_open(int16_t defaultValue);

/**
 * Construct a CompactShortArray from a pre-computed index and values array. The values
 * will be adopted by the CompactShortArray. Memory is allocated with uprv_malloc.
 * Note: for speed, the compact method will only re-use blocks in the values array
 * that are on a block boundary. The pre-computed arrays passed in to this constructor
 * may re-use blocks at any position in the values array. Do not call any of the
 * ucmp16_init*() functions after using this function. They will cause a memory
 * leak. The indexArray and newValues will be uprv_free'd when ucmp16_close() is called.
 *
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initBogus
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_openAdopt(uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue);

/**
 * Construct a CompactShortArray from a pre-computed index and values array. The values
 * will be adopted by the CompactShortArray. Memory is allocated with uprv_malloc.
 * Note: for speed, the compact method will only re-use blocks in the values array
 * that are on a block boundary. The pre-computed arrays passed in to this constructor
 * may re-use blocks at any position in the values array. Do not call any of the
 * ucmp16_init*() functions after using this function. They will cause a memory
 * leak. The indexArray and newValues will be uprv_free'd when ucmp16_close() is called.
 *
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initBogus
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_openAdoptWithBlockShift(uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue,
                                                      int32_t blockShift);

/**
 * Construct a CompactShortArray from a pre-computed index and values array. The values
 * will be aliased by the CompactShortArray. Memory is allocated with uprv_malloc.
 * Note: for speed, the compact method will only re-use blocks in the values array
 * that are on a block boundary. The pre-computed arrays passed in to this constructor
 * may re-use blocks at any position in the values array. Do not call any of the
 * ucmp16_init*() functions after using this function. They will cause a memory
 * leak.
 *
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initBogus
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_openAlias(uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue );

/**
 * Initialize an empty CompactShortArray. Do not call this function if
 * you created the array with any of the ucmp16_open*() funcitons because it
 * will cause a memory leak.
 *
 * @param defaultValue the default value for all characters not explicitly in the array
 * @param array An uninitialized CompactShortArray
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_initAdopt
 * @see ucmp16_initAlias
 */
U_CAPI void U_EXPORT2 ucmp16_init(CompactShortArray* array, int16_t defaultValue);

/**
 * Initialize an empty CompactShortArray to the bogus value. Do not call
 * this function if you created the array with ucmp16_open() because it
 * will cause a memory leak.
 *
 * @param array An uninitialized CompactShortArray
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_isBogus
 * @see ucmp16_init
 * @see ucmp16_initAdopt
 * @see ucmp16_initAlias
 */
U_CAPI void U_EXPORT2 ucmp16_initBogus(CompactShortArray* array);

/**
 * Initialize a CompactShortArray from a pre-computed index and values array. The values
 * will be adopted by the CompactShortArray. No memory is allocated. Note: for speed,
 * the compact method will only re-use blocks in the values array that are on a block
 * boundary. The pre-computed arrays passed in to this constructor may re-use blocks
 * at any position in the values array. The indexArray and newValues will be
 * uprv_free'd when ucmp16_close() is called.
 *
 * @param this_obj the CompactShortArray to be initialized
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initAlias
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_initAdopt(CompactShortArray *this_obj,
                                                      uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue );

/**
 * Initialize a CompactShortArray from a pre-computed index and values array. The values
 * will be adopted by the CompactShortArray. No memory is allocated. Note: for speed,
 * the compact method will only re-use blocks in the values array that are on a block
 * boundary. The pre-computed arrays passed in to this constructor may re-use blocks
 * at any position in the values array. The indexArray and newValues will be
 * uprv_free'd when ucmp16_close() is called.
 *
 * @param this_obj the CompactShortArray to be initialized
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @return The initialized array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initAlias
 * @see ucmp16_initAdopt
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_initAdoptWithBlockShift(CompactShortArray *this_obj,
                                                      uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue,
                                                      int32_t blockShift);

/**
 * Initialize a CompactShortArray from a pre-computed index and values array. The values
 * will be aliased by the CompactShortArray. No memory is allocated. Note: for speed,
 * the compact method will only re-use blocks in the values array that are on a block
 * boundary. The pre-computed arrays passed in to this constructor may re-use blocks
 * at any position in the values array.
 *
 * @param this_obj the CompactShortArray to be initialized
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @return The initialized array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initAdopt
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_initAlias(CompactShortArray *this_obj,
                                                      uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue );


/**
 * Initialize a CompactShortArray from a pre-computed index and values array. The values
 * will be aliased by the CompactShortArray. No memory is allocated. Note: for speed,
 * the compact method will only re-use blocks in the values array that are on a block
 * boundary. The pre-computed arrays passed in to this constructor may re-use blocks
 * at any position in the values array.
 *
 * @param this_obj the CompactShortArray to be initialized
 * @param indexArray the index array to be adopted
 * @param newValues the value array to be adopted
 * @param count the number of entries in the value array
 * @param defaultValue the default value for all characters not explicitly in the array
 * @return The initialized array
 * @see compact
 * @see ucmp16_open
 * @see ucmp16_openAdopt
 * @see ucmp16_openAlias
 * @see ucmp16_init
 * @see ucmp16_initAdopt
 */
U_CAPI  CompactShortArray* U_EXPORT2 ucmp16_initAliasWithBlockShift(CompactShortArray *this_obj,
                                                      uint16_t *indexArray,
                                                      int16_t *newValues,
                                                      int32_t count,
                                                      int16_t defaultValue,
                                                      int32_t blockShift);

/**
 * Free up any allocated memory associated with this compact array.
 * The memory that is uprv_free'd depends on how the array was initialized
 * or opened.
 * 
 * @param array The array to close
 */
U_CAPI  void U_EXPORT2 ucmp16_close(CompactShortArray* array);

/**
 * Returns TRUE if the creation of the compact array fails.
 */
U_CAPI  UBool U_EXPORT2 ucmp16_isBogus(const CompactShortArray* array);

/**
 * Get the mapped value of a Unicode character.
 *
 * @param index the character to get the mapped value with
 * @return the mapped value of the given character
 */
#define ucmp16_get(array, index) (array->fArray[(array->fIndex[(index >> array->kBlockShift)] )+ \
                           (index & array->kBlockMask)])

#define ucmp16_getu(array, index) (uint16_t)ucmp16_get(array, index)


/**
 * Set a new value for a Unicode character.
 * Set automatically expands the array if it is compacted.
 *
 * @param character the character to set the mapped value with
 * @param value the new mapped value
 */
U_CAPI  void U_EXPORT2 ucmp16_set(CompactShortArray *array,
                                  UChar character,
                                  int16_t value);


/**
 * Set new values for a range of Unicode character.
 *
 * @param start the starting offset of the range
 * @param end the ending offset of the range
 * @param value the new mapped value
 */
U_CAPI  void U_EXPORT2 ucmp16_setRange(CompactShortArray* array,
                                       UChar start,
                                       UChar end,
                                       int16_t value);


/**
 * Compact the array. For efficency, this method will only re-use
 * blocks in the values array that are on a block bounday. If you
 * want better compaction, you can do your own compaction and use
 * the constructor that lets you pass in the pre-computed arrays.
 */
U_CAPI  void U_EXPORT2 ucmp16_compact(CompactShortArray* array);

/**
 * Get the default value.
 */
U_CAPI  int16_t U_EXPORT2 ucmp16_getDefaultValue(const CompactShortArray* array);

/**
 * Get the number of elements in the value array.
 * @return the number of elements in the value array.
 */
U_CAPI  uint32_t U_EXPORT2 ucmp16_getCount(const CompactShortArray* array);

/**
 * Get the address of the value array.
 * @return the address of the value array
 */
U_CAPI  const int16_t* U_EXPORT2 ucmp16_getArray(const CompactShortArray* array);

/**
 * Get the address of the index array.
 * @return the address of the index array
 */
U_CAPI  const uint16_t* U_EXPORT2 ucmp16_getIndex(const CompactShortArray* array);


/** INTERNAL USE ONLY **/
U_CAPI  uint32_t U_EXPORT2 ucmp16_flattenMem(const CompactShortArray* array, UMemoryStream *MS);
/** INTERNAL USE ONLY **/
U_CAPI void U_EXPORT2 ucmp16_initFromData(CompactShortArray* array, const uint8_t **source,  UErrorCode *status);

#endif

