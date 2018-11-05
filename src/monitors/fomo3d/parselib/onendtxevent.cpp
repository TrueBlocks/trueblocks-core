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
#include "onendtxevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COnEndTxEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextOnendtxeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOnendtxeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COnEndTxEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOnendtxeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOnendtxeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COnEndTxEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COnEndTxEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountWon" ) { amountWon = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "airDropPot" ) { airDropPot = str_2_Wei(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "compressedData" ) { compressedData = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "compressedIDs" ) { compressedIDs = str_2_Wei(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "ethIn" ) { ethIn = str_2_Wei(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "genAmount" ) { genAmount = str_2_Wei(fieldValue); return true; }
            break;
        case 'k':
            if ( fieldName % "keysBought" ) { keysBought = str_2_Wei(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "newPot" ) { newPot = str_2_Wei(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "playerName" ) { playerName = toLower(fieldValue); return true; }
            if ( fieldName % "playerAddress" ) { playerAddress = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "P3DAmount" ) { P3DAmount = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "potAmount" ) { potAmount = str_2_Wei(fieldValue); return true; }
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
void COnEndTxEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COnEndTxEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> compressedData;
    archive >> compressedIDs;
    archive >> playerName;
    archive >> playerAddress;
    archive >> ethIn;
    archive >> keysBought;
    archive >> winnerAddr;
    archive >> winnerName;
    archive >> amountWon;
    archive >> newPot;
    archive >> P3DAmount;
    archive >> genAmount;
    archive >> potAmount;
    archive >> airDropPot;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COnEndTxEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << compressedData;
    archive << compressedIDs;
    archive << playerName;
    archive << playerAddress;
    archive << ethIn;
    archive << keysBought;
    archive << winnerAddr;
    archive << winnerName;
    archive << amountWon;
    archive << newPot;
    archive << P3DAmount;
    archive << genAmount;
    archive << potAmount;
    archive << airDropPot;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COnEndTxEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const COnEndTxEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COnEndTxEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(COnEndTxEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "compressedData", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "compressedIDs", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "playerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "playerAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "ethIn", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "keysBought", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "winnerAddr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "winnerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "amountWon", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "newPot", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "P3DAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "genAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "potAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnEndTxEvent, "airDropPot", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COnEndTxEvent, "schema");
    HIDE_FIELD(COnEndTxEvent, "deleted");
    HIDE_FIELD(COnEndTxEvent, "showing");
    HIDE_FIELD(COnEndTxEvent, "cname");

    builtIns.push_back(_biCOnEndTxEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOnendtxeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COnEndTxEvent *one = reinterpret_cast<const COnEndTxEvent *>(dataPtr);
    if (one) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, one);
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
bool COnEndTxEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COnEndTxEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOnendtxeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountWon" ) return bnu_2_Str(amountWon);
            if ( fieldName % "airDropPot" ) return bnu_2_Str(airDropPot);
            break;
        case 'c':
            if ( fieldName % "compressedData" ) return bnu_2_Str(compressedData);
            if ( fieldName % "compressedIDs" ) return bnu_2_Str(compressedIDs);
            break;
        case 'e':
            if ( fieldName % "ethIn" ) return bnu_2_Str(ethIn);
            break;
        case 'g':
            if ( fieldName % "genAmount" ) return bnu_2_Str(genAmount);
            break;
        case 'k':
            if ( fieldName % "keysBought" ) return bnu_2_Str(keysBought);
            break;
        case 'n':
            if ( fieldName % "newPot" ) return bnu_2_Str(newPot);
            break;
        case 'p':
            if ( fieldName % "playerName" ) return playerName;
            if ( fieldName % "playerAddress" ) return addr_2_Str(playerAddress);
            if ( fieldName % "P3DAmount" ) return bnu_2_Str(P3DAmount);
            if ( fieldName % "potAmount" ) return bnu_2_Str(potAmount);
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
ostream& operator<<(ostream& os, const COnEndTxEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

