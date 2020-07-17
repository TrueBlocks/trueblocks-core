/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include <algorithm>
#include "publishrecord.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPublishRecord, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPublishrecordChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPublishrecordChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPublishRecord::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["publishrecord_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPublishrecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPublishrecordChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPublishRecord*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPublishRecord::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPublishrecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloom_hash") {
                return bloom_hash;
            }
            if (fieldName % "bloom_pinned") {
                return bool_2_Str(bloom_pinned);
            }
            break;
        case 'f':
            if (fieldName % "filename") {
                return filename;
            }
            break;
        case 'i':
            if (fieldName % "index_hash") {
                return index_hash;
            }
            if (fieldName % "index_pinned") {
                return bool_2_Str(index_pinned);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CPublishRecord::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloom_hash") {
                bloom_hash = fieldValue;
                return true;
            }
            if (fieldName % "bloom_pinned") {
                bloom_pinned = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "filename") {
                filename = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "index_hash") {
                index_hash = fieldValue;
                return true;
            }
            if (fieldName % "index_pinned") {
                index_pinned = str_2_Bool(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPublishRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPublishRecord::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> filename;
    archive >> index_hash;
    archive >> index_pinned;
    archive >> bloom_hash;
    archive >> bloom_pinned;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPublishRecord::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << filename;
    archive << index_hash;
    archive << index_pinned;
    archive << bloom_hash;
    archive << bloom_pinned;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPublishRecordArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPublishRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPublishRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPublishRecord, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPublishRecord, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPublishRecord, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPublishRecord, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPublishRecord, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPublishRecord, "filename", T_TEXT, ++fieldNum);
    ADD_FIELD(CPublishRecord, "index_hash", T_IPFSHASH, ++fieldNum);
    ADD_FIELD(CPublishRecord, "index_pinned", T_BOOL, ++fieldNum);
    ADD_FIELD(CPublishRecord, "bloom_hash", T_IPFSHASH, ++fieldNum);
    ADD_FIELD(CPublishRecord, "bloom_pinned", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPublishRecord, "schema");
    HIDE_FIELD(CPublishRecord, "deleted");
    HIDE_FIELD(CPublishRecord, "showing");
    HIDE_FIELD(CPublishRecord, "cname");

    builtIns.push_back(_biCPublishRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPublishrecordChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPublishRecord* pub = reinterpret_cast<const CPublishRecord*>(dataPtr);
    if (pub) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pub);
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
bool CPublishRecord::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPublishRecord& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PUBLISHRECORD =
    "[{FILENAME}]\t"
    "[{INDEX_HASH}]\t"
    "[{INDEX_PINNED}]\t"
    "[{BLOOM_HASH}]\t"
    "[{BLOOM_PINNED}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
bool publishNotPinned(const string_q& filename, CPublishRecord& result) {
    return true;
}

//---------------------------------------------------------------------------
bool publishPinned(const string_q& filename, CPublishRecord& result) {
    return true;
}

//---------------------------------------------------------------------------
bool forEveryPinnedItem(IPFSVISITFUNC func, void* data) {
    return true;
}

//---------------------------------------------------------------------------
bool forEveryIPFSItem(IPFSVISITFUNC func, void* data) {
    //    if (!func)
    //        return false;
    //
    //    for (size_t i = 0; i < trans.receipt.logs.size(); i++) {
    //        CLogEntry log = trans.receipt.logs[i];
    //        if (!(*func)(log, data))
    //            return false;
    //    }
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
