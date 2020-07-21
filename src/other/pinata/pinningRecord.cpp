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
#include "pinningRecord.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinningRecord, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextPinningrecordChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinningrecordChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinningRecord::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinningrecord_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPinningrecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinningrecordChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinningRecord*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinningRecord::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinningrecordChunk_custom(fieldName, this);
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
            if (fieldName % "bloom_size") {
                return uint_2_Str(bloom_size);
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                return fileName;
            }
            break;
        case 'i':
            if (fieldName % "index_hash") {
                return index_hash;
            }
            if (fieldName % "index_size") {
                return uint_2_Str(index_size);
            }
            break;
        case 'p':
            if (fieldName % "pinDate") {
                return ts_2_Str(pinDate);
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
bool CPinningRecord::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
            if (fieldName % "bloom_size") {
                bloom_size = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                fileName = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "index_hash") {
                index_hash = fieldValue;
                return true;
            }
            if (fieldName % "index_size") {
                index_size = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "pinDate") {
                pinDate = str_2_Ts(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinningRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinningRecord::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> pinDate;
    archive >> fileName;
    archive >> index_hash;
    archive >> index_size;
    archive >> bloom_hash;
    archive >> bloom_size;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinningRecord::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << pinDate;
    archive << fileName;
    archive << index_hash;
    archive << index_size;
    archive << bloom_hash;
    archive << bloom_size;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinningRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinningRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinningRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinningRecord, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinningRecord, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinningRecord, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinningRecord, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinningRecord, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinningRecord, "pinDate", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CPinningRecord, "fileName", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinningRecord, "index_hash", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinningRecord, "index_size", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CPinningRecord, "bloom_hash", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinningRecord, "bloom_size", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinningRecord, "schema");
    HIDE_FIELD(CPinningRecord, "deleted");
    HIDE_FIELD(CPinningRecord, "showing");
    HIDE_FIELD(CPinningRecord, "cname");

    builtIns.push_back(_biCPinningRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinningrecordChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinningRecord* pin = reinterpret_cast<const CPinningRecord*>(dataPtr);
    if (pin) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pin);
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
bool CPinningRecord::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinningRecord& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinningRecord& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinningRecord& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINNINGRECORD =
    "[{PINDATE}]\t"
    "[{FILENAME}]\t"
    "[{INDEX_HASH}]\t"
    "[{INDEX_SIZE}]\t"
    "[{BLOOM_HASH}]\t"
    "[{BLOOM_SIZE}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
