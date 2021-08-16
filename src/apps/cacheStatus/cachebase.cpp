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
#include "cachebase.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCacheBase, CAccountName);

//---------------------------------------------------------------------------
static string_q nextCachebaseChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCachebaseChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCacheBase::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["cachebase_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextCachebaseChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCachebaseChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCacheBase*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCacheBase::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCachebaseChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'f':
            if (fieldName % "firstApp") {
                return uint_2_Str(firstApp);
            }
            break;
        case 'l':
            if (fieldName % "latestApp") {
                return uint_2_Str(latestApp);
            }
            break;
        case 'n':
            if (fieldName % "nApps") {
                return uint_2_Str(nApps);
            }
            break;
        case 'p':
            if (fieldName % "path") {
                return path;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                return sizeInBytes == 0 ? "" : uint_2_Str(sizeInBytes);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CAccountName::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CCacheBase::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'f':
            if (fieldName % "firstApp") {
                firstApp = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "latestApp") {
                latestApp = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nApps") {
                nApps = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                path = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                sizeInBytes = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCacheBase::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCacheBase::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountName::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    // archive >> nApps;
    // archive >> firstApp;
    // archive >> latestApp;
    // archive >> path;
    // archive >> sizeInBytes;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCacheBase::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    // archive << nApps;
    // archive << firstApp;
    // archive << latestApp;
    // archive << path;
    // archive << sizeInBytes;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCacheBase::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CCacheBase copy;
    // FUTURE_WORK
    // FUTURE_WORK
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCacheBaseArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCacheBaseArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCacheBase::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCacheBase, "schema"))
        return;

    CAccountName::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CCacheBase, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCacheBase, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCacheBase, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCacheBase, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCacheBase, "nApps", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CCacheBase, "nApps");
    ADD_FIELD(CCacheBase, "firstApp", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CCacheBase, "firstApp");
    ADD_FIELD(CCacheBase, "latestApp", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CCacheBase, "latestApp");
    ADD_FIELD(CCacheBase, "path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CCacheBase, "path");
    ADD_FIELD(CCacheBase, "sizeInBytes", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CCacheBase, "sizeInBytes");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCacheBase, "schema");
    HIDE_FIELD(CCacheBase, "deleted");
    HIDE_FIELD(CCacheBase, "showing");
    HIDE_FIELD(CCacheBase, "cname");

    builtIns.push_back(_biCCacheBase);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCachebaseChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCacheBase* cac = reinterpret_cast<const CCacheBase*>(dataPtr);
    if (cac) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
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

//---------------------------------------------------------------------------
bool CCacheBase::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCacheBase& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CACHEBASE =
    "[{DISPLAY_NAME}]\t"
    "[{FIRSTAPP}]\t"
    "[{LASTAPP}]\t"
    "[{APPEARANCERANGE}]\t"
    "[{NRECORDS}]\t"
    "[{APPEARANCEINTERVAL}]\t"
    "[{SIZEINBYTES}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
