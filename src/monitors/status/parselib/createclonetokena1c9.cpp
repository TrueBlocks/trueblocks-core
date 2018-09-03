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
#include "createclonetokena1c9.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCreateCloneTokena1c9, CTransaction);

//---------------------------------------------------------------------------
static string_q nextCreateclonetokena1c9Chunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextCreateclonetokena1c9Chunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CCreateCloneTokena1c9::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextCreateclonetokena1c9Chunk, this);
}

//---------------------------------------------------------------------------
string_q nextCreateclonetokena1c9Chunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCreateCloneTokena1c9 *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CCreateCloneTokena1c9::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_parentToken" ) { _parentToken = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_snapshotBlock" ) { _snapshotBlock = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_tokenName" ) { _tokenName = fieldValue; return true; }
            if ( fieldName % "_decimalUnits" ) { _decimalUnits = (uint32_t)str_2_Uint(fieldValue); return true; }
            if ( fieldName % "_tokenSymbol" ) { _tokenSymbol = fieldValue; return true; }
            if ( fieldName % "_transfersEnabled" ) { _transfersEnabled = str_2_Bool(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCreateCloneTokena1c9::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCreateCloneTokena1c9::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _parentToken;
    archive >> _snapshotBlock;
    archive >> _tokenName;
    archive >> _decimalUnits;
    archive >> _tokenSymbol;
    archive >> _transfersEnabled;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCreateCloneTokena1c9::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _parentToken;
    archive << _snapshotBlock;
    archive << _tokenName;
    archive << _decimalUnits;
    archive << _tokenSymbol;
    archive << _transfersEnabled;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCreateCloneTokena1c9Array& array) {
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
CArchive& operator<<(CArchive& archive, const CCreateCloneTokena1c9Array& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCreateCloneTokena1c9::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CCreateCloneTokena1c9, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_parentToken", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_snapshotBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_tokenName", T_TEXT, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_decimalUnits", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_tokenSymbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CCreateCloneTokena1c9, "_transfersEnabled", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCreateCloneTokena1c9, "schema");
    HIDE_FIELD(CCreateCloneTokena1c9, "deleted");
    HIDE_FIELD(CCreateCloneTokena1c9, "showing");
    HIDE_FIELD(CCreateCloneTokena1c9, "cname");

    builtIns.push_back(_biCCreateCloneTokena1c9);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCreateclonetokena1c9Chunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CCreateCloneTokena1c9 *cre = reinterpret_cast<const CCreateCloneTokena1c9 *>(dataPtr);
    if (cre) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, cre);
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
bool CCreateCloneTokena1c9::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CCreateCloneTokena1c9::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextCreateclonetokena1c9Chunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_parentToken" ) return addr_2_Str(_parentToken);
            if ( fieldName % "_snapshotBlock" ) return bnu_2_Str(_snapshotBlock);
            if ( fieldName % "_tokenName" ) return _tokenName;
            if ( fieldName % "_decimalUnits" ) return uint_2_Str(_decimalUnits);
            if ( fieldName % "_tokenSymbol" ) return _tokenSymbol;
            if ( fieldName % "_transfersEnabled" ) return int_2_Str(_transfersEnabled);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCreateCloneTokena1c9& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

