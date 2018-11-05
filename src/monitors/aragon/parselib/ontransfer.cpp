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
#include "ontransfer.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COnTransfer, CTransaction);

//---------------------------------------------------------------------------
static string_q nextOntransferChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOntransferChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COnTransfer::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOntransferChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOntransferChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COnTransfer *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COnTransfer::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) { _from = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_to" ) { _to = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_amount" ) { _amount = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COnTransfer::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COnTransfer::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _from;
    archive >> _to;
    archive >> _amount;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COnTransfer::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _from;
    archive << _to;
    archive << _amount;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COnTransferArray& array) {
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
CArchive& operator<<(CArchive& archive, const COnTransferArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COnTransfer::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(COnTransfer, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COnTransfer, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnTransfer, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COnTransfer, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COnTransfer, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnTransfer, "_to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(COnTransfer, "_amount", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COnTransfer, "schema");
    HIDE_FIELD(COnTransfer, "deleted");
    HIDE_FIELD(COnTransfer, "showing");
    HIDE_FIELD(COnTransfer, "cname");

    builtIns.push_back(_biCOnTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOntransferChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COnTransfer *ont = reinterpret_cast<const COnTransfer *>(dataPtr);
    if (ont) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, ont);
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
bool COnTransfer::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COnTransfer::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOntransferChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) return addr_2_Str(_from);
            if ( fieldName % "_to" ) return addr_2_Str(_to);
            if ( fieldName % "_amount" ) return bnu_2_Str(_amount);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COnTransfer& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

