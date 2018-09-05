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
#include "abirecord.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbiRecord, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAbirecordChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAbirecordChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAbiRecord::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAbirecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAbirecordChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAbiRecord *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbiRecord::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = str_2_Addr(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "constant" ) { constant = str_2_Bool(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "encoding" ) { encoding = str_2_Hash(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "index" ) { index = str_2_Uint(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "symbol" ) { symbol = fieldValue; return true; }
            if ( fieldName % "signature" ) { signature = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        case 'v':
            if ( fieldName % "variables" ) { variables = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAbiRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbiRecord::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> index;
    archive >> symbol;
    archive >> addr;
    archive >> encoding;
    archive >> type;
    archive >> constant;
    archive >> name;
    archive >> signature;
    archive >> variables;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbiRecord::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << index;
    archive << symbol;
    archive << addr;
    archive << encoding;
    archive << type;
    archive << constant;
    archive << name;
    archive << signature;
    archive << variables;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbiRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAbiRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAbiRecord::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAbiRecord, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbiRecord, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbiRecord, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbiRecord, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAbiRecord, "index", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbiRecord, "symbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CAbiRecord, "addr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAbiRecord, "encoding", T_HASH, ++fieldNum);
    ADD_FIELD(CAbiRecord, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CAbiRecord, "constant", T_BOOL, ++fieldNum);
    ADD_FIELD(CAbiRecord, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAbiRecord, "signature", T_TEXT, ++fieldNum);
    ADD_FIELD(CAbiRecord, "variables", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbiRecord, "schema");
    HIDE_FIELD(CAbiRecord, "deleted");
    HIDE_FIELD(CAbiRecord, "showing");
    HIDE_FIELD(CAbiRecord, "cname");

    builtIns.push_back(_biCAbiRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAbirecordChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAbiRecord *abi = reinterpret_cast<const CAbiRecord *>(dataPtr);
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, abi);
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
bool CAbiRecord::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAbiRecord::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAbirecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) return addr_2_Str(addr);
            break;
        case 'c':
            if ( fieldName % "constant" ) return int_2_Str(constant);
            break;
        case 'e':
            if ( fieldName % "encoding" ) return hash_2_Str(encoding);
            break;
        case 'i':
            if ( fieldName % "index" ) return uint_2_Str(index);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            break;
        case 's':
            if ( fieldName % "symbol" ) return symbol;
            if ( fieldName % "signature" ) return signature;
            break;
        case 't':
            if ( fieldName % "type" ) return type;
            break;
        case 'v':
            if ( fieldName % "variables" ) return variables;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbiRecord& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

