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

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAcctCacheItem, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextAcctcacheitemChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAcctcacheitemChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAcctCacheItem::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAcctcacheitemChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAcctcacheitemChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CAcctCacheItem *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

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
            if ( fieldName % "which" ) { which = toLong32(fieldValue); return true; }
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

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNum;
    archive >> transIndex;
    archive >> which;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAcctCacheItem::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
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
    ADD_FIELD(CAcctCacheItem, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "blockNum", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "transIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "which", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAcctCacheItem, "schema");
    HIDE_FIELD(CAcctCacheItem, "deleted");
    HIDE_FIELD(CAcctCacheItem, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAcctcacheitemChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CAcctCacheItem *acc = (const CAcctCacheItem *)dataPtr;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAcctCacheItem::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAcctCacheItem::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAcctCacheItem::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAcctcacheitemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNum" ) return asStringU(blockNum);
            break;
        case 't':
            if ( fieldName % "transIndex" ) return asStringU(transIndex);
            break;
        case 'w':
            if ( fieldName % "which" ) return asString(which);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAcctCacheItem& item) {
    // EXISTING_CODE
    if (sizeof(item) != 0) { // do this to always go through here, but avoid a warning
        os << item.blockNum << "." << item.transIndex << "." << item.which;
        return os;
    }
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CAcctCacheItem::CAcctCacheItem(SFString& line) {

    line.ReplaceAll(".","\t");

    SFString val = nextTokenClear(line,'\t');
    blockNum = toUnsigned(val);

    val = nextTokenClear(line,'\t');
    transIndex = toUnsigned(val);

    val = nextTokenClear(line,'\t');
    which = (int32_t)toUnsigned(val);
}
// EXISTING_CODE
}  // namespace qblocks

