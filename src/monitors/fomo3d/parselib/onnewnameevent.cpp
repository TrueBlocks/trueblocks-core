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
#include "onnewnameevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COnNewNameEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextOnnewnameeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOnnewnameeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COnNewNameEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOnnewnameeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOnnewnameeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COnNewNameEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COnNewNameEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "affiliateID" ) { affiliateID = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "affiliateAddress" ) { affiliateAddress = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "affiliateName" ) { affiliateName = toLower(fieldValue); return true; }
            if ( fieldName % "amountPaid" ) { amountPaid = str_2_Wei(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "isNewPlayer" ) { isNewPlayer = str_2_Bool(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "playerID" ) { playerID = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "playerAddress" ) { playerAddress = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "playerName" ) { playerName = toLower(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timeStamp" ) { timeStamp = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COnNewNameEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COnNewNameEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> playerID;
    archive >> playerAddress;
    archive >> playerName;
    archive >> isNewPlayer;
    archive >> affiliateID;
    archive >> affiliateAddress;
    archive >> affiliateName;
    archive >> amountPaid;
    archive >> timeStamp;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COnNewNameEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << playerID;
    archive << playerAddress;
    archive << playerName;
    archive << isNewPlayer;
    archive << affiliateID;
    archive << affiliateAddress;
    archive << affiliateName;
    archive << amountPaid;
    archive << timeStamp;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COnNewNameEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const COnNewNameEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COnNewNameEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(COnNewNameEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "playerID", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "playerAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "playerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "isNewPlayer", T_BOOL, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "affiliateID", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "affiliateAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "affiliateName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "amountPaid", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnNewNameEvent, "timeStamp", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COnNewNameEvent, "schema");
    HIDE_FIELD(COnNewNameEvent, "deleted");
    HIDE_FIELD(COnNewNameEvent, "showing");
    HIDE_FIELD(COnNewNameEvent, "cname");

    builtIns.push_back(_biCOnNewNameEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOnnewnameeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COnNewNameEvent *onn = reinterpret_cast<const COnNewNameEvent *>(dataPtr);
    if (onn) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, onn);
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
bool COnNewNameEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COnNewNameEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOnnewnameeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "affiliateID" ) return bnu_2_Str(affiliateID);
            if ( fieldName % "affiliateAddress" ) return addr_2_Str(affiliateAddress);
            if ( fieldName % "affiliateName" ) return affiliateName;
            if ( fieldName % "amountPaid" ) return bnu_2_Str(amountPaid);
            break;
        case 'i':
            if ( fieldName % "isNewPlayer" ) return int_2_Str(isNewPlayer);
            break;
        case 'p':
            if ( fieldName % "playerID" ) return bnu_2_Str(playerID);
            if ( fieldName % "playerAddress" ) return addr_2_Str(playerAddress);
            if ( fieldName % "playerName" ) return playerName;
            break;
        case 't':
            if ( fieldName % "timeStamp" ) return bnu_2_Str(timeStamp);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COnNewNameEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

