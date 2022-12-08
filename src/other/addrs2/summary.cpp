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
#include "summary.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSummary, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextSummaryChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextSummaryChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CSummary::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["summary_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextSummaryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSummaryChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CSummary*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CSummary::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextSummaryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "count") {
                return uint_2_Str(count);
            }
            break;
        case 'e':
            if (fieldName % "end") {
                return uint_2_Str(end);
            }
            break;
        case 's':
            if (fieldName % "start") {
                return uint_2_Str(start);
            }
            break;
        case 't':
            if (fieldName % "total") {
                return uint_2_Str(total);
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
bool CSummary::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "count") {
                count = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "end") {
                end = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "start") {
                start = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "total") {
                total = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CSummary::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CSummary::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> start;
    archive >> end;
    archive >> total;
    archive >> count;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSummary::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << start;
    archive << end;
    archive << total;
    archive << count;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSummary::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CSummary copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSummaryArray& array) {
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
CArchive& operator<<(CArchive& archive, const CSummaryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CSummary::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CSummary, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CSummary, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CSummary, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CSummary, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CSummary, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CSummary, "start", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CSummary, "end", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CSummary, "total", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CSummary, "count", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CSummary, "schema");
    HIDE_FIELD(CSummary, "deleted");
    HIDE_FIELD(CSummary, "showing");
    HIDE_FIELD(CSummary, "cname");

    builtIns.push_back(_biCSummary);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSummaryChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CSummary* sum = reinterpret_cast<const CSummary*>(dataPtr);
    if (sum) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, sum);
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
bool CSummary::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CSummary& sum) {
    sum.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSummary& sum) {
    sum.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CSummary& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_SUMMARY = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
CSummary& CSummary::operator+=(const CSummary& v1) {
    start = min(start, v1.start);
    end = max(end, v1.end);
    total = max(total, v1.total);
    count += v1.count;
    return *this;
}
// EXISTING_CODE
}  // namespace qblocks
