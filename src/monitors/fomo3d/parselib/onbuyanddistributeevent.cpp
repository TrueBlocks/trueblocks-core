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
#include "onbuyanddistributeevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COnBuyAndDistributeEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextOnbuyanddistributeeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOnbuyanddistributeeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COnBuyAndDistributeEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOnbuyanddistributeeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOnbuyanddistributeeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COnBuyAndDistributeEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COnBuyAndDistributeEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
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
            if ( fieldName % "ethIn" ) { ethIn = str_2_Wei(fieldValue); return true; }
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
void COnBuyAndDistributeEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COnBuyAndDistributeEvent::Serialize(CArchive& archive) {

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
    archive >> ethIn;
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
bool COnBuyAndDistributeEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << playerAddress;
    archive << playerName;
    archive << ethIn;
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
CArchive& operator>>(CArchive& archive, COnBuyAndDistributeEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const COnBuyAndDistributeEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COnBuyAndDistributeEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(COnBuyAndDistributeEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "playerAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "playerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "ethIn", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "compressedData", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "compressedIDs", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "winnerAddr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "winnerName", T_TEXT, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "amountWon", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "newPot", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "P3DAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(COnBuyAndDistributeEvent, "genAmount", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COnBuyAndDistributeEvent, "schema");
    HIDE_FIELD(COnBuyAndDistributeEvent, "deleted");
    HIDE_FIELD(COnBuyAndDistributeEvent, "showing");
    HIDE_FIELD(COnBuyAndDistributeEvent, "cname");

    builtIns.push_back(_biCOnBuyAndDistributeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOnbuyanddistributeeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COnBuyAndDistributeEvent *onb = reinterpret_cast<const COnBuyAndDistributeEvent *>(dataPtr);
    if (onb) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, onb);
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
bool COnBuyAndDistributeEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COnBuyAndDistributeEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOnbuyanddistributeeventChunk_custom(fieldName, this);
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
            if ( fieldName % "ethIn" ) return bnu_2_Str(ethIn);
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
ostream& operator<<(ostream& os, const COnBuyAndDistributeEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

