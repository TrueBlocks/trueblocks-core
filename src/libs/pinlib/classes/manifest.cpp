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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "manifest.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinManifest, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinmanifestChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinmanifestChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinManifest::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinmanifest_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPinmanifestChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinmanifestChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinManifest*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinManifest::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinmanifestChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomFormat") {
                return bloomFormat;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                return fileName;
            }
            if (fieldName % "firstPin") {
                return uint_2_Str(firstPin);
            }
            break;
        case 'i':
            if (fieldName % "indexFormat") {
                return indexFormat;
            }
            break;
        case 'l':
            if (fieldName % "lastPin") {
                return uint_2_Str(lastPin);
            }
            break;
        case 'p':
            if (fieldName % "pins" || fieldName % "pinsCnt") {
                size_t cnt = pins.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += pins[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
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
bool CPinManifest::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomFormat") {
                bloomFormat = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                fileName = fieldValue;
                return true;
            }
            if (fieldName % "firstPin") {
                firstPin = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexFormat") {
                indexFormat = fieldValue;
                return true;
            }
            break;
        case 'l':
            if (fieldName % "lastPin") {
                lastPin = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "pins") {
                CPinnedChunk obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    pins.push_back(obj);
                    obj = CPinnedChunk();  // reset
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
void CPinManifest::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinManifest::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> fileName;
    archive >> indexFormat;
    archive >> bloomFormat;
    archive >> firstPin;
    archive >> lastPin;
    archive >> pins;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinManifest::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fileName;
    archive << indexFormat;
    archive << bloomFormat;
    archive << firstPin;
    archive << lastPin;
    archive << pins;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinManifestArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinManifestArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinManifest::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinManifest, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinManifest, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinManifest, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinManifest, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinManifest, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinManifest, "fileName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinManifest, "indexFormat", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinManifest, "bloomFormat", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinManifest, "firstPin", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CPinManifest, "lastPin", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CPinManifest, "pins", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinManifest, "schema");
    HIDE_FIELD(CPinManifest, "deleted");
    HIDE_FIELD(CPinManifest, "showing");
    HIDE_FIELD(CPinManifest, "cname");

    builtIns.push_back(_biCPinManifest);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinmanifestChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinManifest* pin = reinterpret_cast<const CPinManifest*>(dataPtr);
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
bool CPinManifest::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinManifest& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinManifest& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinManifest& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPinManifest::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "pins") {
        if (index == NOPOS) {
            CPinnedChunk empty;
            ((CPinManifest*)this)->pins.push_back(empty);  // NOLINT
            index = pins.size() - 1;
        }
        if (index < pins.size())
            return &pins[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINMANIFEST = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
