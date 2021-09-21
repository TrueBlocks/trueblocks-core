/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "pinatapin.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinataPin, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextPinatapinChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinatapinChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinataPin::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinatapin_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextPinatapinChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinatapinChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinataPin*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinataPin::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinatapinChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "date_pinned") {
                return date_pinned;
            }
            if (fieldName % "date_unpinned") {
                return date_unpinned;
            }
            break;
        case 'i':
            if (fieldName % "id") {
                return id;
            }
            if (fieldName % "ipfs_pin_hash") {
                return ipfs_pin_hash;
            }
            break;
        case 'm':
            if (fieldName % "metadata") {
                if (metadata == CPinataMetadata())
                    return "{}";
                return metadata.Format();
            }
            break;
        case 'r':
            if (fieldName % "regions" || fieldName % "regionsCnt") {
                size_t cnt = regions.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += regions[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 's':
            if (fieldName % "size") {
                return uint_2_Str(size);
            }
            break;
        case 'u':
            if (fieldName % "user_id") {
                return user_id;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // test for contained object field specifiers
    string_q objSpec;
    objSpec = toUpper("metadata") + "::";
    if (contains(fieldName, objSpec))
        return metadata.getValueByName(substitute(fieldName, objSpec, ""));

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CPinataPin::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "date_pinned") {
                date_pinned = fieldValue;
                return true;
            }
            if (fieldName % "date_unpinned") {
                date_unpinned = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "id") {
                id = fieldValue;
                return true;
            }
            if (fieldName % "ipfs_pin_hash") {
                ipfs_pin_hash = fieldValue;
                return true;
            }
            break;
        case 'm':
            if (fieldName % "metadata") {
                return metadata.parseJson3(fieldValue);
            }
            break;
        case 'r':
            if (fieldName % "regions") {
                CPinataRegion obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    regions.push_back(obj);
                    obj = CPinataRegion();  // reset
                }
                return true;
            }
            break;
        case 's':
            if (fieldName % "size") {
                size = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'u':
            if (fieldName % "user_id") {
                user_id = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinataPin::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinataPin::Serialize(CArchive& archive) {
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
    archive >> ipfs_pin_hash;
    archive >> size;
    archive >> user_id;
    archive >> date_pinned;
    archive >> date_unpinned;
    archive >> metadata;
    archive >> regions;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinataPin::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << id;
    archive << ipfs_pin_hash;
    archive << size;
    archive << user_id;
    archive << date_pinned;
    archive << date_unpinned;
    archive << metadata;
    archive << regions;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinataPin::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CPinataPin copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataPinArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinataPinArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinataPin::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinataPin, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinataPin, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinataPin, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataPin, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataPin, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinataPin, "id", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataPin, "ipfs_pin_hash", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataPin, "size", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CPinataPin, "user_id", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataPin, "date_pinned", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataPin, "date_unpinned", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(CPinataPin, "metadata", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CPinataMetadata));
    ADD_FIELD(CPinataPin, "regions", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinataPin, "schema");
    HIDE_FIELD(CPinataPin, "deleted");
    HIDE_FIELD(CPinataPin, "showing");
    HIDE_FIELD(CPinataPin, "cname");

    builtIns.push_back(_biCPinataPin);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinatapinChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinataPin* pin = reinterpret_cast<const CPinataPin*>(dataPtr);
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CPinataPin::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinataPin& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataPin& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinataPin& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPinataPin::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "metadata")
        return &metadata;
    if (fieldName % "regions") {
        if (index == NOPOS) {
            CPinataRegion empty;
            ((CPinataPin*)this)->regions.push_back(empty);  // NOLINT
            index = regions.size() - 1;
        }
        if (index < regions.size())
            return &regions[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINATAPIN = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
