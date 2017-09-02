/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "acctcacheitem.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAcctCacheItem, CBaseNode, dataVersion);

//---------------------------------------------------------------------------
static SFString nextAcctcacheitemChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextAcctcacheitemChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CAcctCacheItem::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAcctcacheitemChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAcctcacheitemChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CAcctCacheItem *acc = (const CAcctCacheItem *)data;
    if (acc) {
        // Give customized code a chance to override first
        SFString ret = nextAcctcacheitemChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'b':
                if ( fieldIn % "blockNum" ) return asStringU(acc->blockNum);
                break;
            case 't':
                if ( fieldIn % "transIndex" ) return asStringU(acc->transIndex);
                break;
            case 'w':
                if ( fieldIn % "which" ) return asString(acc->which);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, acc);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAcctCacheItem::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNum" ) { blockNum = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "transIndex" ) { transIndex = toUnsigned(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "which" ) { which = (int32_t)toLong(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAcctCacheItem::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAcctCacheItem::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CAcctCacheItem*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> blockNum;
    archive >> transIndex;
    archive >> which;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAcctCacheItem::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << blockNum;
    archive << transIndex;
    archive << which;

    return true;
}

//---------------------------------------------------------------------------
void CAcctCacheItem::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAcctCacheItem, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "blockNum", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "transIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "which", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAcctCacheItem, "schema");
    HIDE_FIELD(CAcctCacheItem, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAcctcacheitemChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CAcctCacheItem *acc = (const CAcctCacheItem *)data;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAcctCacheItem::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAcctCacheItem::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
ostream& operator<<(ostream& os, const CAcctCacheItem& cacheItem) {
    os << cacheItem.blockNum << "." << cacheItem.transIndex << "." << cacheItem.which;
    return os;
}
// EXISTING_CODE

