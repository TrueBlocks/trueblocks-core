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
#include "collection.h"
#include "options_base.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCollection, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextCollectionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCollectionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCollection::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["collection_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextCollectionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCollectionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCollection*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCollection::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCollectionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addresses" || fieldName % "addressesCnt") {
                size_t cnt = addresses.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + addresses[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            if (fieldName % "addressList") {
                return addressList;
            }
            break;
        case 'c':
            if (fieldName % "cid") {
                return cid;
            }
            if (fieldName % "client") {
                return client;
            }
            break;
        case 'd':
            if (fieldName % "deleted") {
                return bool_2_Str(deleted);
            }
            break;
        case 'm':
            if (fieldName % "monitored") {
                return bool_2_Str(monitored);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                return uint_2_Str(sizeInBytes);
            }
            break;
        case 't':
            if (fieldName % "tags") {
                return tags;
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
bool CCollection::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addresses") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    addresses.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            if (fieldName % "addressList") {
                addressList = fieldValue;
                return true;
            }
            break;
        case 'c':
            if (fieldName % "cid") {
                cid = fieldValue;
                return true;
            }
            if (fieldName % "client") {
                client = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "deleted") {
                deleted = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'm':
            if (fieldName % "monitored") {
                monitored = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                sizeInBytes = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "tags") {
                tags = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCollection::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCollection::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> cid;
    archive >> tags;
    archive >> name;
    archive >> client;
    archive >> monitored;
    archive >> deleted;
    archive >> sizeInBytes;
    archive >> addresses;
    // archive >> addressList;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCollection::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << cid;
    archive << tags;
    archive << name;
    archive << client;
    archive << monitored;
    archive << deleted;
    archive << sizeInBytes;
    archive << addresses;
    // archive << addressList;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCollectionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCollectionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCollection::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCollection, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CCollection, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCollection, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCollection, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCollection, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCollection, "cid", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "tags", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "client", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "monitored", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "deleted", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "sizeInBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CCollection, "addresses", T_ADDRESS | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCollection, "addressList", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CCollection, "addressList");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCollection, "schema");
    HIDE_FIELD(CCollection, "deleted");
    HIDE_FIELD(CCollection, "showing");
    HIDE_FIELD(CCollection, "cname");

    builtIns.push_back(_biCCollection);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCollectionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCollection* col = reinterpret_cast<const CCollection*>(dataPtr);
    if (col) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'i':
                if (fieldIn % "id" && col->cid.empty()) {
                    string_q seed = col->name;
                    for (auto addr : col->addresses)
                        seed += addr;
                    return keccak256(seed).substr(0, 12);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, col);
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
bool CCollection::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCollection& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CCollection::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "addresses" && i < addresses.size())
        return (addresses[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_COLLECTION =
    "[{CID}]\t"
    "[{TAGS}]\t"
    "[{NAME}]\t"
    "[{CLIENT}]\t"
    "[{MONITORED}]\t"
    "[{DELETED}]\t"
    "[{SIZEINBYTES}]\t"
    "[{ADDRESSES}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
