/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "acctcacheitem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAcctCacheItem, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAcctcacheitemChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAcctcacheitemChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAcctCacheItem::Format(CExportContext& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAcctcacheitemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAcctcacheitemChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CAcctCacheItem *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAcctCacheItem::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNum" ) { blockNum = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "transIndex" ) { transIndex = toUnsigned(fieldValue); return true; }
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

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CAcctCacheItemArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CAcctCacheItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAcctCacheItem::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAcctCacheItem, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "blockNum", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctCacheItem, "transIndex", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAcctCacheItem, "schema");
    HIDE_FIELD(CAcctCacheItem, "deleted");
    HIDE_FIELD(CAcctCacheItem, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAcctcacheitemChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAcctCacheItem *acc = (const CAcctCacheItem *)dataPtr;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
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
string_q CAcctCacheItem::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAcctcacheitemChunk_custom(fieldName, this);
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
        os << item.blockNum << "." << item.transIndex;
        return os;
    }
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CAcctCacheItem::CAcctCacheItem(string_q& line) {

    replaceAll(line, ".", "\t");

    string_q val = nextTokenClear(line,'\t');
    blockNum = toUnsigned(val);

    val = nextTokenClear(line,'\t');
    transIndex = toUnsigned(val);
}
// EXISTING_CODE
}  // namespace qblocks

