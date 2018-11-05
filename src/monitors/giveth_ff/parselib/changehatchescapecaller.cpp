/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "changehatchescapecaller.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CChangeHatchEscapeCaller, CTransaction);

//---------------------------------------------------------------------------
static string_q nextChangehatchescapecallerChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextChangehatchescapecallerChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CChangeHatchEscapeCaller::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextChangehatchescapecallerChunk, this);
}

//---------------------------------------------------------------------------
string_q nextChangehatchescapecallerChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CChangeHatchEscapeCaller *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CChangeHatchEscapeCaller::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newEscapeHatchCaller" ) { _newEscapeHatchCaller = str_2_Addr(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CChangeHatchEscapeCaller::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CChangeHatchEscapeCaller::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _newEscapeHatchCaller;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CChangeHatchEscapeCaller::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _newEscapeHatchCaller;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CChangeHatchEscapeCallerArray& array) {
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
CArchive& operator<<(CArchive& archive, const CChangeHatchEscapeCallerArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CChangeHatchEscapeCaller::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CChangeHatchEscapeCaller, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CChangeHatchEscapeCaller, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CChangeHatchEscapeCaller, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CChangeHatchEscapeCaller, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CChangeHatchEscapeCaller, "_newEscapeHatchCaller", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CChangeHatchEscapeCaller, "schema");
    HIDE_FIELD(CChangeHatchEscapeCaller, "deleted");
    HIDE_FIELD(CChangeHatchEscapeCaller, "showing");
    HIDE_FIELD(CChangeHatchEscapeCaller, "cname");

    builtIns.push_back(_biCChangeHatchEscapeCaller);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextChangehatchescapecallerChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CChangeHatchEscapeCaller *cha = reinterpret_cast<const CChangeHatchEscapeCaller *>(dataPtr);
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, cha);
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
bool CChangeHatchEscapeCaller::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CChangeHatchEscapeCaller::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextChangehatchescapecallerChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newEscapeHatchCaller" ) return addr_2_Str(_newEscapeHatchCaller);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CChangeHatchEscapeCaller& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

