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
#include "displayapp.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CDisplayApp, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextDisplayappChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextDisplayappChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CDisplayApp::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["displayapp_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextDisplayappChunk, this);
}

//---------------------------------------------------------------------------
string_q nextDisplayappChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CDisplayApp *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CDisplayApp::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = str_2_Uint(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) { transactionIndex = str_2_Uint(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CDisplayApp::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CDisplayApp::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> blockNumber;
    archive >> transactionIndex;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CDisplayApp::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << blockNumber;
    archive << transactionIndex;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CDisplayAppArray& array) {
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
CArchive& operator<<(CArchive& archive, const CDisplayAppArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CDisplayApp::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CDisplayApp, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CDisplayApp, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CDisplayApp, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDisplayApp, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDisplayApp, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CDisplayApp, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CDisplayApp, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDisplayApp, "transactionIndex", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CDisplayApp, "schema");
    HIDE_FIELD(CDisplayApp, "deleted");
    HIDE_FIELD(CDisplayApp, "showing");
    HIDE_FIELD(CDisplayApp, "cname");

    builtIns.push_back(_biCDisplayApp);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextDisplayappChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CDisplayApp *dis = reinterpret_cast<const CDisplayApp *>(dataPtr);
    if (dis) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, dis);
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
bool CDisplayApp::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CDisplayApp::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextDisplayappChunk_custom(fieldName, this);
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
        case 't':
            if ( fieldName % "transactionIndex" ) return uint_2_Str(transactionIndex);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CDisplayApp& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_DISPLAYAPP =
"[{ADDRESS}]\t"
"[{BLOCKNUMBER}]\t"
"[{TRANSACTIONINDEX}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

