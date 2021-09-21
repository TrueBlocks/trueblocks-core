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
#include "scrapestatistics.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CScrapeStatistics, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextScrapestatisticsChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextScrapestatisticsChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CScrapeStatistics::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["scrapestatistics_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextScrapestatisticsChunk, this);
}

//---------------------------------------------------------------------------
string_q nextScrapestatisticsChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CScrapeStatistics*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CScrapeStatistics::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextScrapestatisticsChunk_custom(fieldName, this);
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
            if (fieldName % "nStageChecked") {
                return uint_2_Str(nStageChecked);
            }
            if (fieldName % "nStageHits") {
                return uint_2_Str(nStageHits);
            }
            if (fieldName % "nTotalHits") {
                return uint_2_Str(nTotalHits);
            }
            if (fieldName % "nFileRecords") {
                return uint_2_Str(nFileRecords);
            }
            if (fieldName % "nCacheWrites") {
                return uint_2_Str(nCacheWrites);
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
bool CScrapeStatistics::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
            if (fieldName % "nStageChecked") {
                nStageChecked = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nStageHits") {
                nStageHits = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nTotalHits") {
                nTotalHits = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nFileRecords") {
                nFileRecords = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nCacheWrites") {
                nCacheWrites = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CScrapeStatistics::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CScrapeStatistics::Serialize(CArchive& archive) {
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
    archive >> nStageChecked;
    archive >> nStageHits;
    archive >> nTotalHits;
    archive >> nFileRecords;
    archive >> nCacheWrites;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CScrapeStatistics::SerializeC(CArchive& archive) const {
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
    archive << nStageChecked;
    archive << nStageHits;
    archive << nTotalHits;
    archive << nFileRecords;
    archive << nCacheWrites;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CScrapeStatistics::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CScrapeStatistics copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CScrapeStatisticsArray& array) {
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
CArchive& operator<<(CArchive& archive, const CScrapeStatisticsArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CScrapeStatistics::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CScrapeStatistics, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CScrapeStatistics, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nFiles", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nSkipped", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nChecked", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nBloomMisses", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nBloomHits", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nFalsePositive", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nPositive", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nStageChecked", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nStageHits", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nTotalHits", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nFileRecords", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CScrapeStatistics, "nCacheWrites", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CScrapeStatistics, "schema");
    HIDE_FIELD(CScrapeStatistics, "deleted");
    HIDE_FIELD(CScrapeStatistics, "showing");
    HIDE_FIELD(CScrapeStatistics, "cname");

    builtIns.push_back(_biCScrapeStatistics);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextScrapestatisticsChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CScrapeStatistics* scr = reinterpret_cast<const CScrapeStatistics*>(dataPtr);
    if (scr) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, scr);
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
bool CScrapeStatistics::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CScrapeStatistics& scr) {
    scr.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CScrapeStatistics& scr) {
    scr.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CScrapeStatistics& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_SCRAPESTATISTICS =
    "[{NFILES}]\t"
    "[{NSKIPPED}]\t"
    "[{NCHECKED}]\t"
    "[{NBLOOMMISSES}]\t"
    "[{NBLOOMHITS}]\t"
    "[{NFALSEPOSITIVE}]\t"
    "[{NPOSITIVE}]\t"
    "[{NSTAGECHECKED}]\t"
    "[{NSTAGEHITS}]\t"
    "[{NTOTALHITS}]\t"
    "[{NFILERECORDS}]\t"
    "[{NCACHEWRITES}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
