/*
*******************************************************************************
* Copyright (C) 2013, International Business Machines
* Corporation and others.  All Rights Reserved.
*******************************************************************************
* collationdatareader.cpp
*
* created on: 2013feb07
* created by: Markus W. Scherer
*/

#include "unicode/utypes.h"

#if !UCONFIG_NO_COLLATION

#include "unicode/ucol.h"
#include "unicode/udata.h"
#include "unicode/uscript.h"
#include "cmemory.h"
#include "collation.h"
#include "collationdata.h"
#include "collationdatareader.h"
#include "collationfastlatin.h"
#include "collationkeys.h"
#include "collationrootelements.h"
#include "collationsettings.h"
#include "collationtailoring.h"
#include "uassert.h"
#include "ucmndata.h"
#include "utrie2.h"

U_NAMESPACE_BEGIN

namespace {

int32_t getIndex(const int32_t *indexes, int32_t length, int32_t i) {
    return (i < length) ? indexes[i] : -1;
}

}  // namespace

void
CollationDataReader::read(const CollationTailoring *base, const uint8_t *inBytes, int32_t inLength,
                          CollationTailoring &tailoring, UErrorCode &errorCode) {
    if(U_FAILURE(errorCode)) { return; }
    if(base != NULL) {
        if(inBytes == NULL || (0 <= inLength && inLength < 24)) {
            errorCode = U_ILLEGAL_ARGUMENT_ERROR;
            return;
        }
        const DataHeader *header = reinterpret_cast<const DataHeader *>(inBytes);
        if(!(header->dataHeader.magic1 == 0xda && header->dataHeader.magic2 == 0x27 &&
                isAcceptable(tailoring.version, NULL, NULL, &header->info))) {
            errorCode = U_INVALID_FORMAT_ERROR;
            return;
        }
        if(base->getUCAVersion() != tailoring.getUCAVersion()) {
            errorCode = U_COLLATOR_VERSION_MISMATCH;
            return;
        }
        int32_t headerLength = header->dataHeader.headerSize;
        inBytes += headerLength;
        if(inLength >= 0) {
            inLength -= headerLength;
        }
    }

    if(inBytes == NULL || (0 <= inLength && inLength < 8)) {
        errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return;
    }
    const int32_t *inIndexes = reinterpret_cast<const int32_t *>(inBytes);
    int32_t indexesLength = inIndexes[IX_INDEXES_LENGTH];
    if(indexesLength < 2 || (0 <= inLength && inLength < indexesLength * 4)) {
        errorCode = U_INVALID_FORMAT_ERROR;  // Not enough indexes.
        return;
    }

    if(!tailoring.ensureOwnedData(errorCode)) { return; }
    CollationData &data = *tailoring.ownedData;
    CollationSettings &settings = tailoring.settings;
    // Assume that data and settings are in initial state,
    // with NULL pointers and 0 lengths.

    const CollationData *baseData = base == NULL ? NULL : base->data;
    data.base = baseData;
    int32_t options = inIndexes[IX_OPTIONS];
    data.numericPrimary = options & 0xff000000;
    settings.options = options & 0xffff;

    // Set pointers to non-empty data parts.
    // Do this in order of their byte offsets. (Should help porting to Java.)

    int32_t index;  // one of the indexes[] slots
    int32_t offset;  // byte offset for the index part
    int32_t length;  // number of bytes in the index part

    if(indexesLength > IX_TOTAL_SIZE) {
        length = inIndexes[IX_TOTAL_SIZE];
    } else if(indexesLength > IX_REORDER_CODES_OFFSET) {
        length = inIndexes[indexesLength - 1];
    } else {
        length = 0;  // only indexes, and inLength was already checked for them
    }
    if(0 <= inLength && inLength < length) {
        errorCode = U_INVALID_FORMAT_ERROR;
        return;
    }

    index = IX_REORDER_CODES_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 4) {
        settings.reorderCodes = reinterpret_cast<const int32_t *>(inBytes + offset);
        settings.reorderCodesLength = length / 4;
    }

    // There should be a reorder table only if there are reorder codes.
    // However, when there are reorder codes the reorder table may be omitted to reduce
    // the data size.
    index = IX_REORDER_TABLE_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 256) {
        settings.reorderTable = inBytes + offset;
    } else {
        // If we have reorder codes, then build the reorderTable at the end,
        // when the CollationData is otherwise complete.
    }

    index = IX_TRIE_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 8) {
        data.trie = tailoring.trie = utrie2_openFromSerialized(
            UTRIE2_32_VALUE_BITS, inBytes + offset, length, NULL,
            &errorCode);
        if(U_FAILURE(errorCode)) { return; }
    } else if(baseData != NULL) {
        // Copy all mappings from the base data.
        // The trie value indexes into the arrays must match those arrays.
        data.trie = baseData->trie;
        data.ce32s = baseData->ce32s;
        data.ces = baseData->ces;
        data.contexts = baseData->contexts;
    } else {
        errorCode = U_INVALID_FORMAT_ERROR;  // No mappings.
        return;
    }

    index = IX_CES_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 8) {
        if(data.ces != NULL) {
            errorCode = U_INVALID_FORMAT_ERROR;  // Tailored ces without tailored trie.
            return;
        }
        data.ces = reinterpret_cast<const int64_t *>(inBytes + offset);
        data.cesLength = length / 8;
    }

    index = IX_CE32S_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 4) {
        if(data.ce32s != NULL) {
            errorCode = U_INVALID_FORMAT_ERROR;  // Tailored ce32s without tailored trie.
            return;
        }
        data.ce32s = reinterpret_cast<const uint32_t *>(inBytes + offset);
        data.ce32sLength = length / 4;
    }

    int32_t jamoCE32sStart = getIndex(inIndexes, indexesLength, IX_JAMO_CE32S_START);
    if(jamoCE32sStart >= 0) {
        if(data.ce32s == NULL) {
            errorCode = U_INVALID_FORMAT_ERROR;  // Index into non-existent ce32s[].
            return;
        }
        data.jamoCE32s = data.ce32s + jamoCE32sStart;
    } else if(baseData != NULL) {
        data.jamoCE32s = baseData->jamoCE32s;
    } else {
        errorCode = U_INVALID_FORMAT_ERROR;  // No Jamo CE32s for Hangul processing.
        return;
    }

    index = IX_ROOT_ELEMENTS_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 4) {
        length /= 4;
        data.rootElements = reinterpret_cast<const uint32_t *>(inBytes + offset);
        data.rootElementsLength = length;
        if(length <= CollationRootElements::IX_SEC_TER_BOUNDARIES) {
            errorCode = U_INVALID_FORMAT_ERROR;
            return;
        }
        uint32_t commonSecTer = data.rootElements[CollationRootElements::IX_COMMON_SEC_AND_TER_CE];
        if(commonSecTer != Collation::COMMON_SEC_AND_TER_CE) {
            errorCode = U_INVALID_FORMAT_ERROR;
            return;
        }
        uint32_t secTerBoundaries = data.rootElements[CollationRootElements::IX_SEC_TER_BOUNDARIES];
        if((secTerBoundaries >> 24) < CollationKeys::SEC_COMMON_HIGH) {
            // [fixed last secondary common byte] is too low,
            // and secondary weights would collide with compressed common secondaries.
            errorCode = U_INVALID_FORMAT_ERROR;
            return;
        }
    }

    index = IX_CONTEXTS_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 2) {
        if(data.contexts != NULL) {
            errorCode = U_INVALID_FORMAT_ERROR;  // Tailored contexts without tailored trie.
            return;
        }
        data.contexts = reinterpret_cast<const UChar *>(inBytes + offset);
        data.contextsLength = length / 2;
    }

    index = IX_UNSAFE_BWD_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 2) {
        if(baseData == NULL) {
            // Create the unsafe-backward set for the root collator.
            // Include all non-zero combining marks and trail surrogates.
            // We do this at load time, rather than at build time,
            // to simplify Unicode version bootstrapping:
            // The root data builder only needs the new FractionalUCA.txt data,
            // but it need not be built with a version of ICU already updated to
            // the corresponding new Unicode Character Database.
            // TODO: Optimize, and reduce dependencies,
            // by enumerating the Normalizer2Impl data more directly.
            tailoring.unsafeBackwardSet = new UnicodeSet(
                UNICODE_STRING_SIMPLE("[[:^lccc=0:][\\udc00-\\udfff]]"), errorCode);
            if(tailoring.unsafeBackwardSet == NULL) {
                errorCode = U_MEMORY_ALLOCATION_ERROR;
                return;
            }
            if(U_FAILURE(errorCode)) { return; }
        } else {
            // Copy the root collator's set contents but don't copy/clone the set as a whole because
            // that would copy the isFrozen state too.
            tailoring.unsafeBackwardSet = new UnicodeSet();
            if(tailoring.unsafeBackwardSet == NULL) {
                errorCode = U_MEMORY_ALLOCATION_ERROR;
                return;
            }
            tailoring.unsafeBackwardSet->addAll(*baseData->unsafeBackwardSet);
        }
        // Add the ranges from the data file to the unsafe-backward set.
        USerializedSet sset;
        const uint16_t *unsafeData = reinterpret_cast<const uint16_t *>(inBytes + offset);
        if(!uset_getSerializedSet(&sset, unsafeData, length / 2)) {
            errorCode = U_INVALID_FORMAT_ERROR;
            return;
        }
        int32_t count = uset_getSerializedRangeCount(&sset);
        for(int32_t i = 0; i < count; ++i) {
            UChar32 start, end;
            uset_getSerializedRange(&sset, i, &start, &end);
            tailoring.unsafeBackwardSet->add(start, end);
        }
        // Mark each lead surrogate as "unsafe"
        // if any of its 1024 associated supplementary code points is "unsafe".
        UChar32 c = 0x10000;
        for(UChar lead = 0xd800; lead < 0xdc00; ++lead, c += 0x400) {
            if(!tailoring.unsafeBackwardSet->containsNone(c, c + 0x3ff)) {
                tailoring.unsafeBackwardSet->add(lead);
            }
        }
        tailoring.unsafeBackwardSet->freeze();
        data.unsafeBackwardSet = tailoring.unsafeBackwardSet;
    } else if(baseData != NULL) {
        // No tailoring-specific data: Alias the root collator's set.
        data.unsafeBackwardSet = baseData->unsafeBackwardSet;
    } else {
        errorCode = U_INVALID_FORMAT_ERROR;  // No unsafeBackwardSet.
        return;
    }

    // If the fast Latin format version is different,
    // or the version is set to 0 for "no fast Latin table",
    // then just always use the normal string comparison path.
    data.fastLatinTable = NULL;
    data.fastLatinTableLength = 0;
    if(((options >> 16) & 0xff) == CollationFastLatin::VERSION) {
        index = IX_FAST_LATIN_TABLE_OFFSET;
        offset = getIndex(inIndexes, indexesLength, index);
        length = getIndex(inIndexes, indexesLength, index + 1) - offset;
        if(length > 0) {
            data.fastLatinTable = reinterpret_cast<const uint16_t *>(inBytes + offset);
            data.fastLatinTableLength = length / 2;
            if((*data.fastLatinTable >> 8) != CollationFastLatin::VERSION) {
                errorCode = U_INVALID_FORMAT_ERROR;  // header vs. table version mismatch
                return;
            }
        } else if(baseData != NULL) {
            data.fastLatinTable = baseData->fastLatinTable;
            data.fastLatinTableLength = baseData->fastLatinTableLength;
        }
    }

    index = IX_SCRIPTS_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 2) {
        data.scripts = reinterpret_cast<const uint16_t *>(inBytes + offset);
        data.scriptsLength = length / 2;
    } else if(baseData != NULL) {
        data.scripts = baseData->scripts;
        data.scriptsLength = baseData->scriptsLength;
    }

    index = IX_COMPRESSIBLE_BYTES_OFFSET;
    offset = getIndex(inIndexes, indexesLength, index);
    length = getIndex(inIndexes, indexesLength, index + 1) - offset;
    if(length >= 256) {
        data.compressibleBytes = reinterpret_cast<const UBool *>(inBytes + offset);
    } else if(baseData != NULL) {
        data.compressibleBytes = baseData->compressibleBytes;
    } else {
        errorCode = U_INVALID_FORMAT_ERROR;  // No compressibleBytes[].
        return;
    }

    // Set variableTop from options and scripts data.
    settings.variableTop = data.getVariableTopForMaxVariable(settings.getMaxVariable());
    if(settings.variableTop == 0) {
        errorCode = U_INVALID_FORMAT_ERROR;
        return;
    }

    if(settings.reorderCodes != NULL && settings.reorderTable == NULL) {
        data.makeReorderTable(settings.reorderCodes, settings.reorderCodesLength,
                              tailoring.reorderTable, errorCode);
        if(U_FAILURE(errorCode)) { return; }
        settings.reorderTable = tailoring.reorderTable;
    }
}

UBool U_CALLCONV
CollationDataReader::isAcceptable(void *context,
                                  const char * /* type */, const char * /*name*/,
                                  const UDataInfo *pInfo) {
    if(
        pInfo->size >= 20 &&
        pInfo->isBigEndian == U_IS_BIG_ENDIAN &&
        pInfo->charsetFamily == U_CHARSET_FAMILY &&
        pInfo->dataFormat[0] == 0x55 &&  // dataFormat="UCol"
        pInfo->dataFormat[1] == 0x43 &&
        pInfo->dataFormat[2] == 0x6f &&
        pInfo->dataFormat[3] == 0x6c &&
        pInfo->formatVersion[0] == 4
    ) {
        UVersionInfo *version = static_cast<UVersionInfo *>(context);
        if(version != NULL) {
            uprv_memcpy(version, pInfo->dataVersion, 4);
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

U_NAMESPACE_END

#endif  // !UCONFIG_NO_COLLATION
