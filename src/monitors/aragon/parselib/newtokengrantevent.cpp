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
#include "newtokengrantevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewTokenGrantEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextNewtokengranteventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextNewtokengranteventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNewTokenGrantEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextNewtokengranteventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextNewtokengranteventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CNewTokenGrantEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewTokenGrantEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "cliff" ) { cliff = str_2_Uint(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = str_2_Addr(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "start" ) { start = str_2_Uint(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "to" ) { to = str_2_Addr(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "vesting" ) { vesting = str_2_Uint(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewTokenGrantEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewTokenGrantEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> from;
    archive >> to;
    archive >> value;
    archive >> start;
    archive >> cliff;
    archive >> vesting;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewTokenGrantEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << from;
    archive << to;
    archive << value;
    archive << start;
    archive << cliff;
    archive << vesting;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CNewTokenGrantEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CNewTokenGrantEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CNewTokenGrantEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CNewTokenGrantEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "value", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "start", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "cliff", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewTokenGrantEvent, "vesting", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewTokenGrantEvent, "schema");
    HIDE_FIELD(CNewTokenGrantEvent, "deleted");
    HIDE_FIELD(CNewTokenGrantEvent, "showing");
    HIDE_FIELD(CNewTokenGrantEvent, "cname");

    builtIns.push_back(_biCNewTokenGrantEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextNewtokengranteventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CNewTokenGrantEvent *newp = reinterpret_cast<const CNewTokenGrantEvent *>(dataPtr);
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, newp);
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
bool CNewTokenGrantEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CNewTokenGrantEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextNewtokengranteventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "cliff" ) return uint_2_Str(cliff);
            break;
        case 'f':
            if ( fieldName % "from" ) return addr_2_Str(from);
            break;
        case 's':
            if ( fieldName % "start" ) return uint_2_Str(start);
            break;
        case 't':
            if ( fieldName % "to" ) return addr_2_Str(to);
            break;
        case 'v':
            if ( fieldName % "value" ) return bnu_2_Str(value);
            if ( fieldName % "vesting" ) return uint_2_Str(vesting);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewTokenGrantEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

