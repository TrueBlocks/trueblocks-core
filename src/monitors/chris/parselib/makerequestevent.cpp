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
#include "makerequestevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CMakeRequestEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextMakerequesteventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextMakerequesteventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CMakeRequestEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextMakerequesteventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextMakerequesteventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CMakeRequestEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CMakeRequestEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_key" ) { _key = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_date" ) { _date = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_tokenAmount" ) { _tokenAmount = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_dollarPrice" ) { _dollarPrice = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_requestType" ) { _requestType = (uint32_t)str_2_Uint(fieldValue); return true; }
            if ( fieldName % "_requester" ) { _requester = str_2_Addr(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CMakeRequestEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CMakeRequestEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _key;
    archive >> _date;
    archive >> _tokenAmount;
    archive >> _dollarPrice;
    archive >> _requestType;
    archive >> _requester;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMakeRequestEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _key;
    archive << _date;
    archive << _tokenAmount;
    archive << _dollarPrice;
    archive << _requestType;
    archive << _requester;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMakeRequestEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CMakeRequestEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CMakeRequestEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CMakeRequestEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_key", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_date", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_tokenAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_dollarPrice", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_requestType", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMakeRequestEvent, "_requester", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMakeRequestEvent, "schema");
    HIDE_FIELD(CMakeRequestEvent, "deleted");
    HIDE_FIELD(CMakeRequestEvent, "showing");
    HIDE_FIELD(CMakeRequestEvent, "cname");

    builtIns.push_back(_biCMakeRequestEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMakerequesteventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CMakeRequestEvent *mak = reinterpret_cast<const CMakeRequestEvent *>(dataPtr);
    if (mak) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, mak);
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
bool CMakeRequestEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CMakeRequestEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextMakerequesteventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_key" ) return bnu_2_Str(_key);
            if ( fieldName % "_date" ) return bnu_2_Str(_date);
            if ( fieldName % "_tokenAmount" ) return bnu_2_Str(_tokenAmount);
            if ( fieldName % "_dollarPrice" ) return bnu_2_Str(_dollarPrice);
            if ( fieldName % "_requestType" ) return uint_2_Str(_requestType);
            if ( fieldName % "_requester" ) return addr_2_Str(_requester);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CMakeRequestEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

