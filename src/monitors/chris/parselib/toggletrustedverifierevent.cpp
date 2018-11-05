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
#include "toggletrustedverifierevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CToggleTrustedVerifierEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextToggletrustedverifiereventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextToggletrustedverifiereventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CToggleTrustedVerifierEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextToggletrustedverifiereventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextToggletrustedverifiereventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CToggleTrustedVerifierEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CToggleTrustedVerifierEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_verifier" ) { _verifier = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_isTrustedVerifier" ) { _isTrustedVerifier = str_2_Bool(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CToggleTrustedVerifierEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CToggleTrustedVerifierEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _verifier;
    archive >> _isTrustedVerifier;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CToggleTrustedVerifierEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _verifier;
    archive << _isTrustedVerifier;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CToggleTrustedVerifierEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CToggleTrustedVerifierEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CToggleTrustedVerifierEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CToggleTrustedVerifierEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CToggleTrustedVerifierEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CToggleTrustedVerifierEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CToggleTrustedVerifierEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CToggleTrustedVerifierEvent, "_verifier", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CToggleTrustedVerifierEvent, "_isTrustedVerifier", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CToggleTrustedVerifierEvent, "schema");
    HIDE_FIELD(CToggleTrustedVerifierEvent, "deleted");
    HIDE_FIELD(CToggleTrustedVerifierEvent, "showing");
    HIDE_FIELD(CToggleTrustedVerifierEvent, "cname");

    builtIns.push_back(_biCToggleTrustedVerifierEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextToggletrustedverifiereventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CToggleTrustedVerifierEvent *tog = reinterpret_cast<const CToggleTrustedVerifierEvent *>(dataPtr);
    if (tog) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tog);
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
bool CToggleTrustedVerifierEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CToggleTrustedVerifierEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextToggletrustedverifiereventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_verifier" ) return addr_2_Str(_verifier);
            if ( fieldName % "_isTrustedVerifier" ) return int_2_Str(_isTrustedVerifier);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CToggleTrustedVerifierEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

