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
#include "tradeevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTradeEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextTradeeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextTradeeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTradeEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTradeeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTradeeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTradeEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTradeEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) { amountGet = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "amountGive" ) { amountGive = str_2_Wei(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "get" ) { get = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "give" ) { give = str_2_Addr(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "tokenGet" ) { tokenGet = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "tokenGive" ) { tokenGive = str_2_Addr(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTradeEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTradeEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> tokenGet;
    archive >> amountGet;
    archive >> tokenGive;
    archive >> amountGive;
    archive >> get;
    archive >> give;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTradeEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << tokenGet;
    archive << amountGet;
    archive << tokenGive;
    archive << amountGive;
    archive << get;
    archive << give;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTradeEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTradeEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTradeEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CTradeEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTradeEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTradeEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTradeEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CTradeEvent, "tokenGet", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTradeEvent, "amountGet", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTradeEvent, "tokenGive", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTradeEvent, "amountGive", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTradeEvent, "get", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTradeEvent, "give", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTradeEvent, "schema");
    HIDE_FIELD(CTradeEvent, "deleted");
    HIDE_FIELD(CTradeEvent, "showing");
    HIDE_FIELD(CTradeEvent, "cname");

    builtIns.push_back(_biCTradeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTradeeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTradeEvent *tra = reinterpret_cast<const CTradeEvent *>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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
bool CTradeEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CTradeEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextTradeeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) return bnu_2_Str(amountGet);
            if ( fieldName % "amountGive" ) return bnu_2_Str(amountGive);
            break;
        case 'g':
            if ( fieldName % "get" ) return addr_2_Str(get);
            if ( fieldName % "give" ) return addr_2_Str(give);
            break;
        case 't':
            if ( fieldName % "tokenGet" ) return addr_2_Str(tokenGet);
            if ( fieldName % "tokenGive" ) return addr_2_Str(tokenGive);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTradeEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

