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
#include "acceptrequestevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAcceptRequestEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextAcceptrequesteventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAcceptrequesteventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAcceptRequestEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAcceptrequesteventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAcceptrequesteventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAcceptRequestEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAcceptRequestEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_key" ) { _key = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_etherAmount" ) { _etherAmount = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_tokenAmount" ) { _tokenAmount = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_dollarPrice" ) { _dollarPrice = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_acceptor" ) { _acceptor = str_2_Addr(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAcceptRequestEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAcceptRequestEvent::Serialize(CArchive& archive) {

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
    archive >> _etherAmount;
    archive >> _tokenAmount;
    archive >> _dollarPrice;
    archive >> _acceptor;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAcceptRequestEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _key;
    archive << _etherAmount;
    archive << _tokenAmount;
    archive << _dollarPrice;
    archive << _acceptor;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAcceptRequestEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAcceptRequestEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAcceptRequestEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CAcceptRequestEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "_key", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "_etherAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "_tokenAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "_dollarPrice", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcceptRequestEvent, "_acceptor", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAcceptRequestEvent, "schema");
    HIDE_FIELD(CAcceptRequestEvent, "deleted");
    HIDE_FIELD(CAcceptRequestEvent, "showing");
    HIDE_FIELD(CAcceptRequestEvent, "cname");

    builtIns.push_back(_biCAcceptRequestEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAcceptrequesteventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAcceptRequestEvent *acc = reinterpret_cast<const CAcceptRequestEvent *>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAcceptRequestEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAcceptRequestEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAcceptrequesteventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_key" ) return bnu_2_Str(_key);
            if ( fieldName % "_etherAmount" ) return bnu_2_Str(_etherAmount);
            if ( fieldName % "_tokenAmount" ) return bnu_2_Str(_tokenAmount);
            if ( fieldName % "_dollarPrice" ) return bnu_2_Str(_dollarPrice);
            if ( fieldName % "_acceptor" ) return addr_2_Str(_acceptor);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAcceptRequestEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

