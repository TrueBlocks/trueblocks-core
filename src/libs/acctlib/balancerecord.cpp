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
#include "balancerecord.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBalanceRecord, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextBalancerecordChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextBalancerecordChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBalanceRecord::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["balancerecord_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBalancerecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBalancerecordChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBalanceRecord *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBalanceRecord::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if ( fieldName % "ether" ) {
        wei = str_2_Wei(fieldValue)* weiPerEther();
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = str_2_Uint(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "wei" ) { wei = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBalanceRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBalanceRecord::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> wei;
//    archive >> address;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBalanceRecord::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << wei;
//    archive << address;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBalanceRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CBalanceRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBalanceRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CBalanceRecord, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CBalanceRecord, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalanceRecord, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBalanceRecord, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBalanceRecord, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CBalanceRecord, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalanceRecord, "wei", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalanceRecord, "address", T_ADDRESS, ++fieldNum);
    HIDE_FIELD(CBalanceRecord, "address");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBalanceRecord, "schema");
    HIDE_FIELD(CBalanceRecord, "deleted");
    HIDE_FIELD(CBalanceRecord, "showing");
    HIDE_FIELD(CBalanceRecord, "cname");

    builtIns.push_back(_biCBalanceRecord);

    // EXISTING_CODE
    ADD_FIELD(CBalanceRecord, "ether", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CBalanceRecord, "ether");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBalancerecordChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CBalanceRecord *bal = reinterpret_cast<const CBalanceRecord *>(dataPtr);
    if (bal) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if ( fieldIn % "dollars" )
                    return getDispBal(bal->blockNumber, bal->wei);
                break;
            case 'e':
                if ( fieldIn % "ether" )
                    return wei_2_Ether(bnu_2_Str(bal->wei));
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, bal);
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
bool CBalanceRecord::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CBalanceRecord::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextBalancerecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'b':
            if ( fieldName % "blockNumber" ) return uint_2_Str(blockNumber);
            break;
        case 'w':
            if ( fieldName % "wei" ) return bnu_2_Str(wei);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBalanceRecord& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

