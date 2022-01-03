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
#include "indexcacheitem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CIndexCacheItem, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextIndexcacheitemChunk(const string_q& fieldIn, const void* dataPtr);
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
            if (fieldName % "bloomHash") {
                return bloomHash;
            }
            break;
        case 'f':
            if (fieldName % "firstApp") {
                return uint_2_Str(firstApp);
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
            if (fieldName % "indexHash") {
                return indexHash;
            }
            break;
        case 'l':
            if (fieldName % "latestApp") {
                return uint_2_Str(latestApp);
            }
            if (fieldName % "latestTs") {
                return ts_2_Str(latestTs);
            }
            break;
        case 'n':
            if (fieldName % "nAddrs") {
                return uint_2_Str(nAddrs);
            }
            if (fieldName % "nApps") {
                return uint_2_Str(nApps);
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
            if (fieldName % "bloomHash") {
                bloomHash = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "firstApp") {
                firstApp = str_2_Uint(fieldValue);
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
            if (fieldName % "indexHash") {
                indexHash = fieldValue;
                return true;
            }
            break;
        case 'l':
            if (fieldName % "latestApp") {
                latestApp = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "latestTs") {
                latestTs = str_2_Ts(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nAddrs") {
                nAddrs = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nApps") {
                nApps = (uint32_t)str_2_Uint(fieldValue);
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
    archive >> nAddrs;
    archive >> nApps;
    archive >> firstApp;
    archive >> latestApp;
    archive >> firstTs;
    archive >> latestTs;
    archive >> filename;
    archive >> fileDate;
    archive >> indexSizeBytes;
    archive >> indexHash;
    archive >> bloomSizeBytes;
    archive >> bloomHash;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIndexCacheItem::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << nAddrs;
    archive << nApps;
    archive << firstApp;
    archive << latestApp;
    archive << firstTs;
    archive << latestTs;
    archive << filename;
    archive << fileDate;
    archive << indexSizeBytes;
    archive << indexHash;
    archive << bloomSizeBytes;
    archive << bloomHash;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIndexCacheItem::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CIndexCacheItem copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
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
    ADD_FIELD(CIndexCacheItem, "nAddrs", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "nApps", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "firstApp", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "latestApp", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "firstTs", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "latestTs", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "filename", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "fileDate", T_DATE, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "indexSizeBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "indexHash", T_IPFSHASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "bloomSizeBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CIndexCacheItem, "bloomHash", T_IPFSHASH | TS_OMITEMPTY, ++fieldNum);

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
            case 'f':
                if (fieldIn % "fileDate") {
                    return fileLastModifyDate(getPathToIndex("finalized/" + ind->filename)).Format(FMT_JSON);
                }
                break;
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CIndexCacheItem::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 8, 4)) {
        archive >> type;
        archive >> nAddrs;
        archive >> nApps;
        archive >> firstApp;
        archive >> latestApp;
        archive >> firstTs;
        archive >> latestTs;
        archive >> filename;
        fileDate = fileLastModifyDate(getPathToIndex("finalized/" + filename));
        archive >> indexSizeBytes;
        archive >> indexHash;
        archive >> bloomSizeBytes;
        archive >> bloomHash;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CIndexCacheItem& ind) {
    ind.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIndexCacheItem& ind) {
    ind.Serialize(archive);
    return archive;
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
