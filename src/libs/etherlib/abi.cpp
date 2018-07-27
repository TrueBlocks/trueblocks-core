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
#include "abi.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAbi::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAbi *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" ) {
                string_q str = fieldValue;
                CFunction item;
                while (item.parseJson3(str)) {
                    abiByName.push_back(item);
                    item = CFunction();  // reset
                }
                return true;
            }
            if ( fieldName % "abiByEncoding" ) {
                string_q str = fieldValue;
                CFunction item;
                while (item.parseJson3(str)) {
                    abiByEncoding.push_back(item);
                    item = CFunction();  // reset
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAbi::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> abiByName;
    archive >> abiByEncoding;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << abiByName;
    archive << abiByEncoding;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbiArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAbiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAbi, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "abiByName", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAbi, "abiByEncoding", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");
    HIDE_FIELD(CAbi, "showing");

    builtIns.push_back(_biCAbi);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAbi *abi = reinterpret_cast<const CAbi *>(dataPtr);
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
bool CAbi::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAbi::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" || fieldName % "abiByNameCnt" ) {
                size_t cnt = abiByName.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += abiByName[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if ( fieldName % "abiByEncoding" || fieldName % "abiByEncodingCnt" ) {
                size_t cnt = abiByEncoding.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += abiByEncoding[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbi& item) {
    // EXISTING_CODE
    if (sizeof(item) != 0) {  // do this to always go through here, but avoid a warning
        for (size_t i = 0 ; i < item.abiByName.size() ; i++) {
            os << item.abiByName[i].Format() << "\n";
        }
        for (size_t i = 0 ; i < item.abiByEncoding.size() ; i++) {
            os << item.abiByEncoding[i].Format() << "\n";
        }
        return os;
    }
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAbi::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "abiByName" && index < abiByName.size() )
        return &abiByName[index];
    if ( fieldName % "abiByEncoding" && index < abiByEncoding.size() )
        return &abiByEncoding[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CAbi::loadABIFromFile(const string_q& fileName) {

    string_q contents = asciiFileToString(fileName);
    CFunction func;
    while (func.parseJson3(contents)) {
        abiByName.push_back(func);
        abiByEncoding.push_back(func);
        func = CFunction();  // reset
    }
    sort(abiByName.begin(), abiByName.end(), sortByFuncName);
    sort(abiByEncoding.begin(), abiByEncoding.end());  // encoding is default sort
    return abiByName.size();
}
// EXISTING_CODE
}  // namespace qblocks

