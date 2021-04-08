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
#include "indexcacheitem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CIndexCacheItem, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextIndexcacheitemChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextIndexcacheitemChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CIndexCacheItem::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["indexcacheitem_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextIndexcacheitemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextIndexcacheitemChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CIndexCacheItem*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CIndexCacheItem::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextIndexcacheitemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomSizeBytes") {
                return uint_2_Str(bloomSizeBytes);
            }
            if (fieldName % "bloom_hash") {
                return bloom_hash;
            }
            break;
        case 'f':
            if (fieldName % "firstAppearance") {
                return uint_2_Str(firstAppearance);
            }
            if (fieldName % "firstTs") {
                return ts_2_Str(firstTs);
            }
            if (fieldName % "filename") {
                return filename;
            }
            if (fieldName % "fileDate") {
                return fileDate.Format(FMT_JSON);
            }
            break;
        case 'i':
            if (fieldName % "indexSizeBytes") {
                return uint_2_Str(indexSizeBytes);
            }
            if (fieldName % "index_hash") {
                return index_hash;
            }
            break;
        case 'l':
            if (fieldName % "latestAppearance") {
                return uint_2_Str(latestAppearance);
            }
            if (fieldName % "latestTs") {
                return ts_2_Str(latestTs);
            }
            break;
        case 'n':
            if (fieldName % "nAddresses") {
                return uint_2_Str(nAddresses);
            }
            if (fieldName % "nAppearances") {
                return uint_2_Str(nAppearances);
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
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
bool CIndexCacheItem::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomSizeBytes") {
                bloomSizeBytes = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "bloom_hash") {
                bloom_hash = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "firstAppearance") {
                firstAppearance = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "firstTs") {
                firstTs = str_2_Ts(fieldValue);
                return true;
            }
            if (fieldName % "filename") {
                filename = fieldValue;
                return true;
            }
            if (fieldName % "fileDate") {
                fileDate = str_2_Date(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexSizeBytes") {
                indexSizeBytes = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "index_hash") {
                index_hash = fieldValue;
                return true;
            }
            break;
        case 'l':
            if (fieldName % "latestAppearance") {
                latestAppearance = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "latestTs") {
                latestTs = str_2_Ts(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nAddresses") {
                nAddresses = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nAppearances") {
                nAppearances = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CIndexCacheItem::finishParse() {
    // EXISTING_CODE
    fileDate = fileLastModifyDate(getIndexPath("finalized/" + filename));
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CIndexCacheItem::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> type;
    archive >> nAddresses;
    archive >> nAppearances;
    archive >> firstAppearance;
    archive >> latestAppearance;
    archive >> firstTs;
    archive >> latestTs;
    archive >> filename;
    archive >> fileDate;
    archive >> indexSizeBytes;
    archive >> index_hash;
    archive >> bloomSizeBytes;
    archive >> bloom_hash;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIndexCacheItem::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << nAddresses;
    archive << nAppearances;
    archive << firstAppearance;
    archive << latestAppearance;
    archive << firstTs;
    archive << latestTs;
    archive << filename;
    archive << fileDate;
    archive << indexSizeBytes;
    archive << index_hash;
    archive << bloomSizeBytes;
    archive << bloom_hash;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIndexCacheItemArray& array) {
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
CArchive& operator<<(CArchive& archive, const CIndexCacheItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CIndexCacheItem::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CIndexCacheItem, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CIndexCacheItem, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "nAddresses", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "nAppearances", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "firstAppearance", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "latestAppearance", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "firstTs", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "latestTs", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "filename", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "fileDate", T_DATE, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "indexSizeBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "index_hash", T_IPFSHASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "bloomSizeBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "bloom_hash", T_IPFSHASH | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CIndexCacheItem, "schema");
    HIDE_FIELD(CIndexCacheItem, "deleted");
    HIDE_FIELD(CIndexCacheItem, "showing");
    HIDE_FIELD(CIndexCacheItem, "cname");

    builtIns.push_back(_biCIndexCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextIndexcacheitemChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CIndexCacheItem* ind = reinterpret_cast<const CIndexCacheItem*>(dataPtr);
    if (ind) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, ind);
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
bool CIndexCacheItem::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 8, 4)) {
        archive >> type;
        archive >> nAddresses;
        archive >> nAppearances;
        archive >> firstAppearance;
        archive >> latestAppearance;
        archive >> firstTs;
        archive >> latestTs;
        archive >> filename;
        fileDate = fileLastModifyDate(getIndexPath("finalized/" + filename));
        archive >> indexSizeBytes;
        archive >> index_hash;
        archive >> bloomSizeBytes;
        archive >> bloom_hash;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CIndexCacheItem& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_INDEXCACHEITEM = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
