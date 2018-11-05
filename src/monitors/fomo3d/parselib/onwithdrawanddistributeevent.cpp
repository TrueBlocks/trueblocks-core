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
#include "onwithdrawanddistributeevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COnWithdrawAndDistributeEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextOnwithdrawanddistributeeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOnwithdrawanddistributeeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COnWithdrawAndDistributeEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOnwithdrawanddistributeeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOnwithdrawanddistributeeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COnWithdrawAndDistributeEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COnWithdrawAndDistributeEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountWon" ) { amountWon = str_2_Wei(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "compressedData" ) { compressedData = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "compressedIDs" ) { compressedIDs = str_2_Wei(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "ethOut" ) { ethOut = str_2_Wei(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "genAmount" ) { genAmount = str_2_Wei(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "newPot" ) { newPot = str_2_Wei(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "playerAddress" ) { playerAddress = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "playerName" ) { playerName = toLower(fieldValue); return true; }
            if ( fieldName % "P3DAmount" ) { P3DAmount = str_2_Wei(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "winnerAddr" ) { winnerAddr = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "winnerName" ) { winnerName = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COnWithdrawAndDistributeEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COnWithdrawAndDistributeEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> playerAddress;
    archive >> playerName;
    archive >> ethOut;
    archive >> compressedData;
    archive >> compressedIDs;
    archive >> winnerAddr;
    archive >> winnerName;
    archive >> amountWon;
    archive >> newPot;
    archive >> P3DAmount;
    archive >> genAmount;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COnWithdrawAndDistributeEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << playerAddress;
    archive << playerName;
    archive << ethOut;
    archive << compressedData;
    archive << compressedIDs;
    archive << winnerAddr;
    archive << winnerName;
    archive << amountWon;
    archive << newPot;
    archive << P3DAmount;
    archive << genAmount;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COnWithdrawAndDistributeEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const COnWithdrawAndDistributeEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COnWithdrawAndDistributeEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(COnWithdrawAndDistributeEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "playerAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "playerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "ethOut", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "compressedData", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "compressedIDs", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "winnerAddr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "winnerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "amountWon", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "newPot", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "P3DAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnWithdrawAndDistributeEvent, "genAmount", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COnWithdrawAndDistributeEvent, "schema");
    HIDE_FIELD(COnWithdrawAndDistributeEvent, "deleted");
    HIDE_FIELD(COnWithdrawAndDistributeEvent, "showing");
    HIDE_FIELD(COnWithdrawAndDistributeEvent, "cname");

    builtIns.push_back(_biCOnWithdrawAndDistributeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOnwithdrawanddistributeeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COnWithdrawAndDistributeEvent *onw = reinterpret_cast<const COnWithdrawAndDistributeEvent *>(dataPtr);
    if (onw) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, onw);
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
bool COnWithdrawAndDistributeEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COnWithdrawAndDistributeEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOnwithdrawanddistributeeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountWon" ) return bnu_2_Str(amountWon);
            break;
        case 'c':
            if ( fieldName % "compressedData" ) return bnu_2_Str(compressedData);
            if ( fieldName % "compressedIDs" ) return bnu_2_Str(compressedIDs);
            break;
        case 'e':
            if ( fieldName % "ethOut" ) return bnu_2_Str(ethOut);
            break;
        case 'g':
            if ( fieldName % "genAmount" ) return bnu_2_Str(genAmount);
            break;
        case 'n':
            if ( fieldName % "newPot" ) return bnu_2_Str(newPot);
            break;
        case 'p':
            if ( fieldName % "playerAddress" ) return addr_2_Str(playerAddress);
            if ( fieldName % "playerName" ) return playerName;
            if ( fieldName % "P3DAmount" ) return bnu_2_Str(P3DAmount);
            break;
        case 'w':
            if ( fieldName % "winnerAddr" ) return addr_2_Str(winnerAddr);
            if ( fieldName % "winnerName" ) return winnerName;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COnWithdrawAndDistributeEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

