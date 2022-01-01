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
#include "status.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CStatus, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextStatusChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextStatusChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CStatus::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["status_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextStatusChunk, this);
}

//---------------------------------------------------------------------------
string_q nextStatusChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CStatus*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CStatus::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextStatusChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "clientVersion") {
                return clientVersion;
            }
            if (fieldName % "clientIds") {
                return clientIds;
            }
            if (fieldName % "configPath") {
                return configPath;
            }
            if (fieldName % "cachePath") {
                return cachePath;
            }
            if (fieldName % "caches" || fieldName % "cachesCnt") {
                size_t cnt = caches.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += caches[i]->Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'h':
            if (fieldName % "host") {
                return host;
            }
            if (fieldName % "hasEskey") {
                return bool_2_Str(hasEskey);
            }
            if (fieldName % "hasPinkey") {
                return bool_2_Str(hasPinkey);
            }
            break;
        case 'i':
            if (fieldName % "indexPath") {
                return indexPath;
            }
            if (fieldName % "isTesting") {
                return bool_2_Str(isTesting);
            }
            if (fieldName % "isApi") {
                return bool_2_Str(isApi);
            }
            if (fieldName % "isDocker") {
                return bool_2_Str(isDocker);
            }
            if (fieldName % "isScraping") {
                return bool_2_Str(isScraping);
            }
            if (fieldName % "isArchive") {
                return bool_2_Str(isArchive);
            }
            if (fieldName % "isTracing") {
                return bool_2_Str(isTracing);
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                return rpcProvider;
            }
            break;
        case 't':
            if (fieldName % "trueblocksVersion") {
                return trueblocksVersion;
            }
            if (fieldName % "ts") {
                return ts_2_Str(ts);
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
bool CStatus::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "clientVersion") {
                clientVersion = fieldValue;
                return true;
            }
            if (fieldName % "clientIds") {
                clientIds = fieldValue;
                return true;
            }
            if (fieldName % "configPath") {
                configPath = fieldValue;
                return true;
            }
            if (fieldName % "cachePath") {
                cachePath = fieldValue;
                return true;
            }
            if (fieldName % "caches") {
                // This drops memory, so we comment it out for now
                // clear();
                // caches = new CCachePtrArray;
                // if (caches) {
                //     string_q str = fieldValue;
                //     return caches->parseJson3(str);
                // }
                return false;
            }
            break;
        case 'h':
            if (fieldName % "host") {
                host = fieldValue;
                return true;
            }
            if (fieldName % "hasEskey") {
                hasEskey = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "hasPinkey") {
                hasPinkey = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexPath") {
                indexPath = fieldValue;
                return true;
            }
            if (fieldName % "isTesting") {
                isTesting = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isApi") {
                isApi = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isDocker") {
                isDocker = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isScraping") {
                isScraping = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isArchive") {
                isArchive = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isTracing") {
                isTracing = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                rpcProvider = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "trueblocksVersion") {
                trueblocksVersion = fieldValue;
                return true;
            }
            if (fieldName % "ts") {
                ts = str_2_Ts(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CStatus::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CStatus::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> clientVersion;
    archive >> clientIds;
    archive >> trueblocksVersion;
    archive >> rpcProvider;
    // archive >> configPath;
    archive >> cachePath;
    archive >> indexPath;
    archive >> host;
    archive >> isTesting;
    archive >> isApi;
    archive >> isDocker;
    archive >> isScraping;
    archive >> isArchive;
    archive >> isTracing;
    archive >> hasEskey;
    archive >> hasPinkey;
    // archive >> ts;
    // archive >> caches
    // EXISTING_CODE
    uint64_t nCaches = 0;
    archive >> nCaches;
    if (nCaches) {
        for (size_t i = 0; i < nCaches; i++) {
            string_q cacheType;
            archive >> cacheType;
            CCache* cache = reinterpret_cast<CCache*>(createObjectOfType(cacheType));  // NOLINT
            if (cache) {
                cache->Serialize(archive);
                caches.push_back(cache);
            }
        }
    }
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStatus::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << clientVersion;
    archive << clientIds;
    archive << trueblocksVersion;
    archive << rpcProvider;
    // archive << configPath;
    archive << cachePath;
    archive << indexPath;
    archive << host;
    archive << isTesting;
    archive << isApi;
    archive << isDocker;
    archive << isScraping;
    archive << isArchive;
    archive << isTracing;
    archive << hasEskey;
    archive << hasPinkey;
    // archive << ts;
    // archive << cache;
    // EXISTING_CODE
    archive << (uint64_t)caches.size();
    for (auto cache : caches) {
        archive << cache->getRuntimeClass()->getClassNamePtr();
        cache->SerializeC(archive);
    }
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStatus::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CStatus copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStatusArray& array) {
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
CArchive& operator<<(CArchive& archive, const CStatusArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CStatus::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CStatus, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CStatus, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CStatus, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CStatus, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CStatus, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CStatus, "clientVersion", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "clientIds", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "trueblocksVersion", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "rpcProvider", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "configPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CStatus, "configPath");
    ADD_FIELD(CStatus, "cachePath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "indexPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "host", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isTesting", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isApi", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isDocker", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isScraping", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isArchive", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "isTracing", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "hasEskey", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "hasPinkey", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatus, "ts", T_TIMESTAMP, ++fieldNum);
    HIDE_FIELD(CStatus, "ts");
    ADD_FIELD(CStatus, "caches", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CStatus, "schema");
    HIDE_FIELD(CStatus, "deleted");
    HIDE_FIELD(CStatus, "showing");
    HIDE_FIELD(CStatus, "cname");

    builtIns.push_back(_biCStatus);

    // EXISTING_CODE
    SHOW_FIELD(CStatus, "caches");
    HIDE_FIELD(CStatus, "ts");
    ADD_FIELD(CStatus, "date", T_DATE, ++fieldNum);
    SHOW_FIELD(CStatus, "date");
    SHOW_FIELD(CStatus, "configPath");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextStatusChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CStatus* sta = reinterpret_cast<const CStatus*>(dataPtr);
    if (sta) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date")
                    return isTestMode() ? "--date--" : ts_2_Date(sta->ts).Format(FMT_JSON);
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, sta);
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
bool CStatus::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CStatus& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CStatus::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "caches") {
        if (index == NOPOS) {
            CCache* empty = nullptr;
            ((CStatus*)this)->caches.push_back(empty);  // NOLINT
            index = caches.size() - 1;
        }
        if (index < caches.size())
            return caches[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_STATUS = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
