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
#include "cacheentry.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCacheEntry, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextCacheentryChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCacheentryChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCacheEntry::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["cacheentry_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextCacheentryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCacheentryChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCacheEntry*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCacheEntry::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCacheentryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cached") {
                return bool_2_Str(cached);
            }
            break;
        case 'e':
            if (fieldName % "extra") {
                return extra;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                return path;
            }
            break;
        case 't':
            if (fieldName % "type") {
                return uint_2_Str(type);
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
bool CCacheEntry::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cached") {
                cached = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "extra") {
                extra = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                path = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCacheEntry::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCacheEntry::Serialize(CArchive& archive) {
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
    archive >> extra;
    archive >> cached;
    archive >> path;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCacheEntry::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << extra;
    archive << cached;
    archive << path;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCacheEntry::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CCacheEntry copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCacheEntryArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCacheEntryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCacheEntry::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCacheEntry, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CCacheEntry, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCacheEntry, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCacheEntry, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCacheEntry, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCacheEntry, "type", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CCacheEntry, "extra", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCacheEntry, "cached", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCacheEntry, "path", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCacheEntry, "schema");
    HIDE_FIELD(CCacheEntry, "deleted");
    HIDE_FIELD(CCacheEntry, "showing");
    HIDE_FIELD(CCacheEntry, "cname");

    builtIns.push_back(_biCCacheEntry);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCacheentryChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCacheEntry* cac = reinterpret_cast<const CCacheEntry*>(dataPtr);
    if (cac) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'b':
                if (fieldIn % "blockNumber")
                    return cac->extra;
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, cac);
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
bool CCacheEntry::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CCacheEntry& cac) {
    cac.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCacheEntry& cac) {
    cac.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCacheEntry& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CACHEENTRY = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
