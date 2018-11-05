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
#include "setclaim.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSetClaim, CTransaction);

//---------------------------------------------------------------------------
static string_q nextSetclaimChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextSetclaimChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CSetClaim::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextSetclaimChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSetclaimChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CSetClaim *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CSetClaim::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'k':
            if ( fieldName % "key" ) { key = toLower(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "subject" ) { subject = str_2_Addr(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CSetClaim::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CSetClaim::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> subject;
    archive >> key;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSetClaim::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << subject;
    archive << key;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSetClaimArray& array) {
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
CArchive& operator<<(CArchive& archive, const CSetClaimArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CSetClaim::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CSetClaim, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CSetClaim, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CSetClaim, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CSetClaim, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CSetClaim, "subject", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CSetClaim, "key", T_TEXT, ++fieldNum);
    ADD_FIELD(CSetClaim, "value", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CSetClaim, "schema");
    HIDE_FIELD(CSetClaim, "deleted");
    HIDE_FIELD(CSetClaim, "showing");
    HIDE_FIELD(CSetClaim, "cname");

    builtIns.push_back(_biCSetClaim);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSetclaimChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CSetClaim *set = reinterpret_cast<const CSetClaim *>(dataPtr);
    if (set) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, set);
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
bool CSetClaim::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CSetClaim::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextSetclaimChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'k':
            if ( fieldName % "key" ) return key;
            break;
        case 's':
            if ( fieldName % "subject" ) return addr_2_Str(subject);
            break;
        case 'v':
            if ( fieldName % "value" ) return value;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CSetClaim& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

