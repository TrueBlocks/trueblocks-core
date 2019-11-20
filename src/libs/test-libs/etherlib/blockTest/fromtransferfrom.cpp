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
#include "fromtransferfrom.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QFromTransferFrom, QTransferFrom);

//---------------------------------------------------------------------------
static string_q nextFromtransferfromChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextFromtransferfromChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void QFromTransferFrom::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["fromtransferfrom_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextFromtransferfromChunk, this);
}

//---------------------------------------------------------------------------
string_q nextFromtransferfromChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const QFromTransferFrom*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q QFromTransferFrom::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextFromtransferfromChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'w':
            if (fieldName % "whop") {
                return addr_2_Str(whop);
            }
            if (fieldName % "werp") {
                return addr_2_Str(werp);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return QTransferFrom::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool QFromTransferFrom::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (QTransferFrom::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'w':
            if (fieldName % "whop") {
                whop = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "werp") {
                werp = str_2_Addr(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QFromTransferFrom::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QFromTransferFrom::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    QTransferFrom::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> whop;
    archive >> werp;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QFromTransferFrom::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    QTransferFrom::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << whop;
    archive << werp;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, QFromTransferFromArray& array) {
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
CArchive& operator<<(CArchive& archive, const QFromTransferFromArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QFromTransferFrom::registerClass(void) {
    // only do this once
    if (HAS_FIELD(QFromTransferFrom, "schema"))
        return;

    QTransferFrom::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QFromTransferFrom, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(QFromTransferFrom, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(QFromTransferFrom, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(QFromTransferFrom, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(QFromTransferFrom, "whop", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QFromTransferFrom, "werp", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QFromTransferFrom, "schema");
    HIDE_FIELD(QFromTransferFrom, "deleted");
    HIDE_FIELD(QFromTransferFrom, "showing");
    HIDE_FIELD(QFromTransferFrom, "cname");

    builtIns.push_back(_biQFromTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextFromtransferfromChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const QFromTransferFrom* fro = reinterpret_cast<const QFromTransferFrom*>(dataPtr);
    if (fro) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, fro);
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
bool QFromTransferFrom::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QFromTransferFrom& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_FROMTRANSFERFROM = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
