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
#include "removeclaim.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRemoveClaim, CTransaction);

//---------------------------------------------------------------------------
static string_q nextRemoveclaimChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextRemoveclaimChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CRemoveClaim::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRemoveclaimChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRemoveclaimChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CRemoveClaim *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CRemoveClaim::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "issuer" ) { issuer = str_2_Addr(fieldValue); return true; }
            break;
        case 'k':
            if ( fieldName % "key" ) { key = toLower(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "subject" ) { subject = str_2_Addr(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CRemoveClaim::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CRemoveClaim::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> issuer;
    archive >> subject;
    archive >> key;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRemoveClaim::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << issuer;
    archive << subject;
    archive << key;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRemoveClaimArray& array) {
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
CArchive& operator<<(CArchive& archive, const CRemoveClaimArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CRemoveClaim::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CRemoveClaim, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CRemoveClaim, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CRemoveClaim, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CRemoveClaim, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CRemoveClaim, "issuer", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CRemoveClaim, "subject", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CRemoveClaim, "key", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRemoveClaim, "schema");
    HIDE_FIELD(CRemoveClaim, "deleted");
    HIDE_FIELD(CRemoveClaim, "showing");
    HIDE_FIELD(CRemoveClaim, "cname");

    builtIns.push_back(_biCRemoveClaim);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRemoveclaimChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CRemoveClaim *rem = reinterpret_cast<const CRemoveClaim *>(dataPtr);
    if (rem) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rem);
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
bool CRemoveClaim::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CRemoveClaim::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextRemoveclaimChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "issuer" ) return addr_2_Str(issuer);
            break;
        case 'k':
            if ( fieldName % "key" ) return key;
            break;
        case 's':
            if ( fieldName % "subject" ) return addr_2_Str(subject);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRemoveClaim& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

