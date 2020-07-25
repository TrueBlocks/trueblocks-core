/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "pinatametadata.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinataMetadata, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinatametadataChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinatametadataChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinataMetadata::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinatametadata_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPinatametadataChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinatametadataChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinataMetadata*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinataMetadata::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinatametadataChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "keyvalues" || fieldName % "keyvaluesCnt") {
                size_t cnt = keyvalues.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += keyvalues[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CPinataMetadata::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "keyvalues") {
                CKeyValue item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    keyvalues.push_back(item);
                    item = CKeyValue();  // reset
                }
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinataMetadata::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinataMetadata::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> keyvalues;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinataMetadata::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << keyvalues;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataMetadataArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinataMetadataArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinataMetadata::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinataMetadata, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinataMetadata, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinataMetadata, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataMetadata, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataMetadata, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinataMetadata, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinataMetadata, "keyvalues", T_OBJECT | TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinataMetadata, "schema");
    HIDE_FIELD(CPinataMetadata, "deleted");
    HIDE_FIELD(CPinataMetadata, "showing");
    HIDE_FIELD(CPinataMetadata, "cname");

    builtIns.push_back(_biCPinataMetadata);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinatametadataChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinataMetadata* pin = reinterpret_cast<const CPinataMetadata*>(dataPtr);
    if (pin) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pin);
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
bool CPinataMetadata::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinataMetadata& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPinataMetadata::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "keyvalues" && index < keyvalues.size())
        return &keyvalues[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINATAMETADATA = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
