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
#include "tableentry.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTableEntry, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTableentryChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTableentryChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTableEntry::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["tableentry_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTableentryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTableentryChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTableEntry*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTableEntry::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTableentryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "description") {
                return description;
            }
            break;
        case 'f':
            if (fieldName % "freelist") {
                return uint_2_Str(freelist);
            }
            break;
        case 'i':
            if (fieldName % "id") {
                return uint_2_Str(id);
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                return longName;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 's':
            if (fieldName % "silky") {
                return uint_2_Str(silky);
            }
            if (fieldName % "stat") {
                if (stat == CLmdbStat())
                    return "{}";
                return stat.Format();
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("stat")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = stat.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTableEntry::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "description") {
                description = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "freelist") {
                freelist = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "id") {
                id = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                longName = fieldValue;
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
            if (fieldName % "silky") {
                silky = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "stat") {
                return stat.parseJson3(fieldValue);
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTableEntry::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTableEntry::Serialize(CArchive& archive) {
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
    archive >> silky;
    archive >> name;
    archive >> longName;
    archive >> description;
    archive >> freelist;
    archive >> stat;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTableEntry::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << id;
    archive << silky;
    archive << name;
    archive << longName;
    archive << description;
    archive << freelist;
    archive << stat;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTableEntryArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTableEntryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTableEntry::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTableEntry, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTableEntry, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTableEntry, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTableEntry, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTableEntry, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTableEntry, "id", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTableEntry, "silky", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTableEntry, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTableEntry, "longName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTableEntry, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTableEntry, "freelist", T_UNUMBER, ++fieldNum);
    ADD_OBJECT(CTableEntry, "stat", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CLmdbStat));

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTableEntry, "schema");
    HIDE_FIELD(CTableEntry, "deleted");
    HIDE_FIELD(CTableEntry, "showing");
    HIDE_FIELD(CTableEntry, "cname");

    builtIns.push_back(_biCTableEntry);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTableentryChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTableEntry* tab = reinterpret_cast<const CTableEntry*>(dataPtr);
    if (tab) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tab);
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
bool CTableEntry::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTableEntry& tab) {
    tab.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTableEntry& tab) {
    tab.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTableEntry& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CTableEntry::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "stat")
        return &stat;

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TABLEENTRY =
    "[{ID}]\t"
    "[{SILKY}]\t"
    "[{NAME}]\t"
    "[{LONGNAME}]\t"
    "[{DESCRIPTION}]\t"
    "[{FREELIST}]\t"
    "[{STAT::PSIZE}]\t"
    "[{STAT::DEPTH}]\t"
    "[{STAT::ENTRIES}]\t"
    "[{STAT::BRANCH_PAGES}]\t"
    "[{STAT::BRANCH_BYTES}]\t"
    "[{STAT::BRANCH_PCT}]\t"
    "[{STAT::LEAF_PAGES}]\t"
    "[{STAT::LEAF_BYTES}]\t"
    "[{STAT::LEAF_PCT}]\t"
    "[{STAT::OVERFLOW_PAGES}]\t"
    "[{STAT::OVERFLOW_BYTES}]\t"
    "[{STAT::OVERFLOW_PCT}]\t"
    "[{STAT::TOTAL_PAGES}]\t"
    "[{STAT::TOTAL_BYTES}]\t"
    "[{STAT::TOTAL_PCT}]\t"
    "[{STAT::AVG}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
