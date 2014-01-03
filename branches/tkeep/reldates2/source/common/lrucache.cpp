/*
*******************************************************************************
* Copyright (C) 2014, International Business Machines Corporation and         
* others. All Rights Reserved.                                                
*******************************************************************************
*                                                                             
* File LRUCACHE.CPP                                                             
*******************************************************************************
*/

#include "lrucache.h"
#include "uhash.h"
#include "cstring.h"

U_NAMESPACE_BEGIN

// Named CacheEntry2 to avoid conflict with CacheEntry in serv.cpp
// Don't know how to make truly private class that the linker can't see.
class CacheEntry2 : public UMemory {
public:
    CacheEntry2 *moreRecent;
    CacheEntry2 *lessRecent;
    char *localeId;
    const SharedObject *cachedData;
    UErrorCode status;  // This is the error if any from creating cachedData.

    CacheEntry2();
    ~CacheEntry2();

    void unlink();
    void uninit();
    UBool init(const char *localeId, SharedObject *dataToAdopt, UErrorCode err);
private:
    CacheEntry2(const CacheEntry2& other);
    CacheEntry2 &operator=(const CacheEntry2& other);
};

CacheEntry2::CacheEntry2() 
    : moreRecent(NULL), lessRecent(NULL), localeId(NULL), cachedData(NULL),
      status(U_ZERO_ERROR) {
}

CacheEntry2::~CacheEntry2() {
    uninit();
}

void CacheEntry2::unlink() {
    if (moreRecent != NULL) {
        moreRecent->lessRecent = lessRecent;
    }
    if (lessRecent != NULL) {
        lessRecent->moreRecent = moreRecent;
    }
    moreRecent = NULL;
    lessRecent = NULL;
}

void CacheEntry2::uninit() {
    SharedObject::clearPtr(cachedData);
    status = U_ZERO_ERROR;
    if (localeId != NULL) {
        uprv_free(localeId);
    }
    localeId = NULL;
}

UBool CacheEntry2::init(const char *locId, SharedObject *dataToAdopt, UErrorCode err) {
    uninit();
    localeId = (char *) uprv_malloc(strlen(locId) + 1);
    if (localeId == NULL) {
        delete dataToAdopt;
        return FALSE;
    }
    uprv_strcpy(localeId, locId);
    SharedObject::copyPtr(dataToAdopt, cachedData);
    status = err;
    return TRUE;
}

void LRUCache::moveToMostRecent(CacheEntry2 *entry) {
    entry->unlink();
    entry->moreRecent = mostRecentlyUsedMarker;
    entry->lessRecent = mostRecentlyUsedMarker->lessRecent;
    mostRecentlyUsedMarker->lessRecent->moreRecent = entry;
    mostRecentlyUsedMarker->lessRecent = entry;
}

SharedObject *LRUCache::safeCreate(const char *localeId, UErrorCode &status) {
    SharedObject *result = create(localeId, status);

    // Safe guard to ensure that some error is reported for missing data in
    // case subclass forgets to set status.
    if (result == NULL && U_SUCCESS(status)) {
        status = U_MEMORY_ALLOCATION_ERROR;
        return NULL;
    } 

    // Safe guard to ensure that if subclass reports an error and returns
    // data that we don't leak memory.
    if (result != NULL && U_FAILURE(status)) {
        delete result;
        return NULL;
    }
    return result;
}

UBool LRUCache::init(const char *localeId, CacheEntry2 *entry) {
    UErrorCode status = U_ZERO_ERROR;
    SharedObject *result = safeCreate(localeId, status);
    return entry->init(localeId, result, status);
}

UBool LRUCache::contains(const char *localeId) const {
    return (uhash_get(localeIdToEntries, localeId) != NULL);
}


const SharedObject *LRUCache::_get(const char *localeId, UErrorCode &status) {
    CacheEntry2 *entry = (CacheEntry2 *) uhash_get(localeIdToEntries, localeId);
    if (entry != NULL) {
        moveToMostRecent(entry);
    } else {
        // Its a cache miss.

        if (uhash_count(localeIdToEntries) < maxSize) {
            entry = new CacheEntry2;
        } else {
            entry = leastRecentlyUsedMarker->moreRecent;
            uhash_remove(localeIdToEntries, entry->localeId);
            entry->unlink();
            entry->uninit();
        }
 
        // entry is an uninitialized, unlinked cache entry 
        // or entry is null if memory could not be allocated.
        if (entry != NULL) {
            if (!init(localeId, entry)) {
                delete entry;
                entry = NULL;
            }
        }

        // Entry is initialized, but unlinked or entry is null on
        // memory allocation error.
        if (entry != NULL) {
            // Add to hashtable
            uhash_put(localeIdToEntries, entry->localeId, entry, &status);
            if (U_FAILURE(status)) {
                delete entry;
                entry = NULL;
            }
        }
        if (entry != NULL) {
            moveToMostRecent(entry);
        }
    }
    if (entry == NULL) {
        status = U_MEMORY_ALLOCATION_ERROR;
        return NULL;
    }

    // If we get here our data is cached.
    if (U_FAILURE(entry->status)) {
        status = entry->status;
        return NULL;
    }
    return entry->cachedData;
}

LRUCache::LRUCache(int32_t size, UErrorCode &status) :
        mostRecentlyUsedMarker(NULL),
        leastRecentlyUsedMarker(NULL),
        localeIdToEntries(NULL),
        maxSize(size) {
    if (U_FAILURE(status)) {
        return;
    }
    mostRecentlyUsedMarker = new CacheEntry2;
    leastRecentlyUsedMarker = new CacheEntry2;
    if (mostRecentlyUsedMarker == NULL || leastRecentlyUsedMarker == NULL) {
        delete mostRecentlyUsedMarker;
        delete leastRecentlyUsedMarker;
        mostRecentlyUsedMarker = leastRecentlyUsedMarker = NULL;
        status = U_MEMORY_ALLOCATION_ERROR;
        return;
    }
    mostRecentlyUsedMarker->moreRecent = NULL;
    mostRecentlyUsedMarker->lessRecent = leastRecentlyUsedMarker;
    leastRecentlyUsedMarker->moreRecent = mostRecentlyUsedMarker;
    leastRecentlyUsedMarker->lessRecent = NULL;
    localeIdToEntries = uhash_openSize(
        uhash_hashChars,
        uhash_compareChars,
        NULL,
        maxSize + maxSize / 5,
        &status);
    if (U_FAILURE(status)) {
        return;
    }
}

LRUCache::~LRUCache() {
    uhash_close(localeIdToEntries);
    for (CacheEntry2 *i = mostRecentlyUsedMarker; i != NULL;) {
        CacheEntry2 *next = i->lessRecent;
        delete i;
        i = next;
    }
}

SimpleLRUCache::~SimpleLRUCache() {
}

SharedObject *SimpleLRUCache::create(const char *localeId, UErrorCode &status) {
    return createFunc(localeId, status);
}

U_NAMESPACE_END
