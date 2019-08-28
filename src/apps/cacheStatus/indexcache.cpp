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
#include "indexcache.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CIndexCache, CCache);

//---------------------------------------------------------------------------
static string_q nextIndexcacheChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextIndexcacheChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CIndexCache::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["indexcache_fmt"] : fmtIn);
    if (fmt.empty()) {
        doExport(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextIndexcacheChunk, this);
}

//---------------------------------------------------------------------------
string_q nextIndexcacheChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CIndexCache *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CIndexCache::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CCache::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "items" ) {
                CIndexCacheItem item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    items.push_back(item);
                    item = CIndexCacheItem();  // reset
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
void CIndexCache::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CIndexCache::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CCache::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> items;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIndexCache::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CCache::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << items;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIndexCacheArray& array) {
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
CArchive& operator<<(CArchive& archive, const CIndexCacheArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CIndexCache::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CIndexCache, "schema")) return;

    CCache::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CIndexCache, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CIndexCache, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CIndexCache, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CIndexCache, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CIndexCache, "items", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CIndexCache, "schema");
    HIDE_FIELD(CIndexCache, "deleted");
    HIDE_FIELD(CIndexCache, "showing");
    HIDE_FIELD(CIndexCache, "cname");

    builtIns.push_back(_biCIndexCache);

    // EXISTING_CODE
    ADD_FIELD(CIndexCache, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CIndexCache, "path", T_TEXT, ++fieldNum);
    ADD_FIELD(CIndexCache, "nFiles", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIndexCache, "nFolders", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIndexCache, "sizeInBytes", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIndexCache, "valid_counts", T_BOOL, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextIndexcacheChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CIndexCache *ind = reinterpret_cast<const CIndexCache *>(dataPtr);
    if (ind) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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
bool CIndexCache::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CIndexCache& ind) {
    ind.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIndexCache& ind) {
    ind.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CIndexCache::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextIndexcacheChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "items" || fieldName % "itemsCnt" ) {
                size_t cnt = items.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += items[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CCache::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CIndexCache& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CIndexCache::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "items" && index < items.size() )
        return &items[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

