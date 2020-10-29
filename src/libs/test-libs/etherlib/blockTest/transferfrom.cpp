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
#include "transferfrom.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QTransferFrom, CTransaction);

//---------------------------------------------------------------------------
static string_q nextTransferfromChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTransferfromChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void QTransferFrom::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["transferfrom_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTransferfromChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTransferfromChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const QTransferFrom*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q QTransferFrom::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTransferfromChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if (fieldName % "_from") {
                return addr_2_Str(_from);
            }
            if (fieldName % "_to") {
                return addr_2_Str(_to);
            }
            if (fieldName % "_value") {
                return bnu_2_Str(_value);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if (fieldName % "_from") {
                _from = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "_to") {
                _to = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "_value") {
                _value = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QTransferFrom::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _from;
    archive >> _to;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _from;
    archive << _to;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, QTransferFromArray& array) {
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
CArchive& operator<<(CArchive& archive, const QTransferFromArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QTransferFrom::registerClass(void) {
    // only do this once
    if (HAS_FIELD(QTransferFrom, "schema"))
        return;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QTransferFrom, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(QTransferFrom, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(QTransferFrom, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(QTransferFrom, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(QTransferFrom, "_from", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(QTransferFrom, "_to", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(QTransferFrom, "_value", T_UINT256, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QTransferFrom, "schema");
    HIDE_FIELD(QTransferFrom, "deleted");
    HIDE_FIELD(QTransferFrom, "showing");
    HIDE_FIELD(QTransferFrom, "cname");

    builtIns.push_back(_biQTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTransferfromChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const QTransferFrom* tra = reinterpret_cast<const QTransferFrom*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tra);
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
bool QTransferFrom::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QTransferFrom& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRANSFERFROM = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
