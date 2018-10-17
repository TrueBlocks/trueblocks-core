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
#include "addressperblock.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAddressPerBlock, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAddressperblockChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAddressperblockChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAddressPerBlock::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAddressperblockChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAddressperblockChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAddressPerBlock *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAddressPerBlock::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'b':
            if ( fieldName % "block_number" ) { block_number = fieldValue; return true; }
            break;
        case 'i':
            if ( fieldName % "id" ) { id = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "reason" ) { reason = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "tx_index" ) { tx_index = fieldValue; return true; }
            if ( fieldName % "trace_id" ) { trace_id = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAddressPerBlock::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAddressPerBlock::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> id;
    archive >> block_number;
    archive >> tx_index;
    archive >> trace_id;
    archive >> address;
    archive >> reason;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAddressPerBlock::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << id;
    archive << block_number;
    archive << tx_index;
    archive << trace_id;
    archive << address;
    archive << reason;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAddressPerBlockArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAddressPerBlockArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAddressPerBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAddressPerBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "id", T_TEXT, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "block_number", T_TEXT, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "tx_index", T_TEXT, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "trace_id", T_TEXT, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAddressPerBlock, "reason", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAddressPerBlock, "schema");
    HIDE_FIELD(CAddressPerBlock, "deleted");
    HIDE_FIELD(CAddressPerBlock, "showing");
    HIDE_FIELD(CAddressPerBlock, "cname");

    builtIns.push_back(_biCAddressPerBlock);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAddressperblockChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAddressPerBlock *add = reinterpret_cast<const CAddressPerBlock *>(dataPtr);
    if (add) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, add);
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
bool CAddressPerBlock::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAddressPerBlock::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAddressperblockChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'b':
            if ( fieldName % "block_number" ) return block_number;
            break;
        case 'i':
            if ( fieldName % "id" ) return id;
            break;
        case 'r':
            if ( fieldName % "reason" ) return reason;
            break;
        case 't':
            if ( fieldName % "tx_index" ) return tx_index;
            if ( fieldName % "trace_id" ) return trace_id;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAddressPerBlock& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

