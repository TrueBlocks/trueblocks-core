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
#include "acctscrapestats.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAcctScrapeStats, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAcctscrapestatsChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAcctscrapestatsChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAcctScrapeStats::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["acctscrapestats_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAcctscrapestatsChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAcctscrapestatsChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAcctScrapeStats*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAcctScrapeStats::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAcctscrapestatsChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "nFiles") {
                return uint_2_Str(nFiles);
            }
            if (fieldName % "nSkipped") {
                return uint_2_Str(nSkipped);
            }
            if (fieldName % "nChecked") {
                return uint_2_Str(nChecked);
            }
            if (fieldName % "nBloomMisses") {
                return uint_2_Str(nBloomMisses);
            }
            if (fieldName % "nBloomHits") {
                return uint_2_Str(nBloomHits);
            }
            if (fieldName % "nFalsePositive") {
                return uint_2_Str(nFalsePositive);
            }
            if (fieldName % "nPositive") {
                return uint_2_Str(nPositive);
            }
            if (fieldName % "nRecords") {
                return uint_2_Str(nRecords);
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
bool CAcctScrapeStats::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "nFiles") {
                nFiles = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nSkipped") {
                nSkipped = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nChecked") {
                nChecked = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nBloomMisses") {
                nBloomMisses = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nBloomHits") {
                nBloomHits = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nFalsePositive") {
                nFalsePositive = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nPositive") {
                nPositive = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nRecords") {
                nRecords = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAcctScrapeStats::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAcctScrapeStats::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> nFiles;
    archive >> nSkipped;
    archive >> nChecked;
    archive >> nBloomMisses;
    archive >> nBloomHits;
    archive >> nFalsePositive;
    archive >> nPositive;
    archive >> nRecords;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAcctScrapeStats::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << nFiles;
    archive << nSkipped;
    archive << nChecked;
    archive << nBloomMisses;
    archive << nBloomHits;
    archive << nFalsePositive;
    archive << nPositive;
    archive << nRecords;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAcctScrapeStatsArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAcctScrapeStatsArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAcctScrapeStats::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAcctScrapeStats, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAcctScrapeStats, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nFiles", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nSkipped", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nChecked", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nBloomMisses", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nBloomHits", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nFalsePositive", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nPositive", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAcctScrapeStats, "nRecords", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAcctScrapeStats, "schema");
    HIDE_FIELD(CAcctScrapeStats, "deleted");
    HIDE_FIELD(CAcctScrapeStats, "showing");
    HIDE_FIELD(CAcctScrapeStats, "cname");

    builtIns.push_back(_biCAcctScrapeStats);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAcctscrapestatsChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAcctScrapeStats* acc = reinterpret_cast<const CAcctScrapeStats*>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAcctScrapeStats::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAcctScrapeStats& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ACCTSCRAPESTATS =
    "[{NFILES}]\t"
    "[{NSKIPPED}]\t"
    "[{NCHECKED}]\t"
    "[{NBLOOMMISSES}]\t"
    "[{NBLOOMHITS}]\t"
    "[{NFALSEPOSITIVE}]\t"
    "[{NPOSITIVE}]\t"
    "[{NRECORDS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
