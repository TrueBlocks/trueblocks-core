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
#include "accountname.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountName, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAccountnameChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccountName::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["accountname_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAccountnameChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccountName *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = fieldValue; return true; }
            break;
        case 'c':
            if ( fieldName % "custom" ) { custom = str_2_Bool(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "description" ) { description = fieldValue; return true; }
            break;
        case 'f':
            if ( fieldName % "fn" ) { fn = fieldValue; return true; }
            break;
        case 'g':
            if ( fieldName % "group" ) { group = fieldValue; return true; }
            break;
        case 'i':
            if ( fieldName % "isContract" ) { isContract = str_2_Bool(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "logo" ) { logo = fieldValue; return true; }
            if ( fieldName % "lb" ) { lb = str_2_Uint(fieldValue); return true; }
            if ( fieldName % "le" ) { le = str_2_Uint(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "nrecs" ) { nrecs = str_2_Uint(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "symbol" ) { symbol = fieldValue; return true; }
            if ( fieldName % "source" ) { source = fieldValue; return true; }
            if ( fieldName % "shared" ) { shared = fieldValue; return true; }
            if ( fieldName % "size" ) { size = str_2_Double(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountName::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> group;
    archive >> addr;
    archive >> symbol;
    archive >> name;
    archive >> source;
    archive >> description;
    archive >> logo;
    archive >> isContract;
    archive >> custom;
    archive >> shared;
//    archive >> fn;
//    archive >> size;
//    archive >> lb;
//    archive >> le;
//    archive >> nrecs;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << group;
    archive << addr;
    archive << symbol;
    archive << name;
    archive << source;
    archive << description;
    archive << logo;
    archive << isContract;
    archive << custom;
    archive << shared;
//    archive << fn;
//    archive << size;
//    archive << lb;
//    archive << le;
//    archive << nrecs;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountNameArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccountNameArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccountName::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAccountName, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAccountName, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountName, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountName, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountName, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAccountName, "group", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "addr", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "symbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "source", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "description", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "logo", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "isContract", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountName, "custom", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountName, "shared", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "fn", T_TEXT, ++fieldNum);
    HIDE_FIELD(CAccountName, "fn");
    ADD_FIELD(CAccountName, "size", T_DOUBLE, ++fieldNum);
    HIDE_FIELD(CAccountName, "size");
    ADD_FIELD(CAccountName, "lb", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CAccountName, "lb");
    ADD_FIELD(CAccountName, "le", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CAccountName, "le");
    ADD_FIELD(CAccountName, "nrecs", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CAccountName, "nrecs");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountName, "schema");
    HIDE_FIELD(CAccountName, "deleted");
    HIDE_FIELD(CAccountName, "showing");
    HIDE_FIELD(CAccountName, "cname");

    builtIns.push_back(_biCAccountName);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAccountName *acc = reinterpret_cast<const CAccountName *>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAccountName::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAccountName::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAccountnameChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) return addr;
            break;
        case 'c':
            if ( fieldName % "custom" ) return bool_2_Str(custom);
            break;
        case 'd':
            if ( fieldName % "description" ) return description;
            break;
        case 'f':
            if ( fieldName % "fn" ) return fn;
            break;
        case 'g':
            if ( fieldName % "group" ) return group;
            break;
        case 'i':
            if ( fieldName % "isContract" ) return bool_2_Str(isContract);
            break;
        case 'l':
            if ( fieldName % "logo" ) return logo;
            if ( fieldName % "lb" ) return uint_2_Str(lb);
            if ( fieldName % "le" ) return uint_2_Str(le);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            if ( fieldName % "nrecs" ) return uint_2_Str(nrecs);
            break;
        case 's':
            if ( fieldName % "symbol" ) return symbol;
            if ( fieldName % "source" ) return source;
            if ( fieldName % "shared" ) return shared;
            if ( fieldName % "size" ) return double_2_Str(size);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountName& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CAccountName::CAccountName(const string_q& strIn) {
    string str = substitute(substitute(trim(strIn, '\t'), "\n", ""), "\r", "");

    CStringArray parts;
    explode(parts, str, '\t');
    if (parts.size() > 0) { group = parts[0]; }
    if (parts.size() > 1) { addr = toLower(parts[1]); }
    if (parts.size() > 2) { name = parts[2]; }
    if (parts.size() > 3) { description = parts[3]; }
    if (parts.size() > 4) { symbol = parts[4]; }
    if (parts.size() > 5) { source = parts[5]; }
    if (parts.size() > 6) { logo = parts[6]; }
    if (parts.size() > 7) { isContract = str_2_Bool(parts[7]); }
    if (parts.size() > 8) { custom = str_2_Bool(parts[8]); }
    if (parts.size() > 9) { shared = parts[9]; }
}
// EXISTING_CODE
}  // namespace qblocks

