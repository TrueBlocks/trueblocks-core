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
#include "lmdbval.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLmdbVal, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextLmdbvalChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextLmdbvalChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CLmdbVal::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["lmdbval_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextLmdbvalChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLmdbvalChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLmdbVal*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CLmdbVal::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextLmdbvalChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'd':
//            if (fieldName % "data") {
//                string_q ret;
//                for (size_t i{0}; i<size; i++) {
//                    ret += chr_2_
//
//                }
//                return chr_2_Hex(data);
//            }
            break;
        case 's':
            if (fieldName % "size") {
                return uint_2_Str(size);
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
bool CLmdbVal::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "data") {
                data = (char*)fieldValue.c_str();
                return true;
            }
            break;
        case 's':
            if (fieldName % "size") {
                size = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CLmdbVal::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLmdbVal::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> size;
    // archive >> data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLmdbVal::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << size;
    // archive << data;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbValArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLmdbValArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLmdbVal::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLmdbVal, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLmdbVal, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLmdbVal, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbVal, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbVal, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CLmdbVal, "size", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbVal, "data", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CLmdbVal, "data");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLmdbVal, "schema");
    HIDE_FIELD(CLmdbVal, "deleted");
    HIDE_FIELD(CLmdbVal, "showing");
    HIDE_FIELD(CLmdbVal, "cname");

    builtIns.push_back(_biCLmdbVal);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLmdbvalChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CLmdbVal* lmd = reinterpret_cast<const CLmdbVal*>(dataPtr);
    if (lmd) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, lmd);
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
bool CLmdbVal::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLmdbVal& lmd) {
    lmd.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbVal& lmd) {
    lmd.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLmdbVal& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LMDBVAL =
    "[{SIZE}]\t"
    "[{DATA}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
