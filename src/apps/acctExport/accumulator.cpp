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
#include "accumulator.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccumulator, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAccumulatorChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAccumulatorChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAccumulator::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["accumulator_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAccumulatorChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccumulatorChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccumulator*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAccumulator::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAccumulatorChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "endOfPeriod") {
                return endOfPeriod.Format(FMT_JSON);
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
bool CAccumulator::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "endOfPeriod") {
                endOfPeriod = str_2_Date(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccumulator::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccumulator::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> endOfPeriod;
    // EXISTING_CODE
    // archive >> sum_type;
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccumulator::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << endOfPeriod;
    // EXISTING_CODE
    // archive << sum_type;
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccumulatorArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccumulatorArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccumulator::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAccumulator, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAccumulator, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccumulator, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccumulator, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccumulator, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccumulator, "endOfPeriod", T_DATE, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccumulator, "schema");
    HIDE_FIELD(CAccumulator, "deleted");
    HIDE_FIELD(CAccumulator, "showing");
    HIDE_FIELD(CAccumulator, "cname");

    builtIns.push_back(_biCAccumulator);

    // EXISTING_CODE
    ADD_FIELD(CAccumulator, "sum_type", T_NUMBER, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccumulatorChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAccumulator* acc = reinterpret_cast<const CAccumulator*>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 's':
                if (fieldIn % "sum_type") {
                    return uint_2_Str(acc->sum_type);
                }
                break;
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
bool CAccumulator::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccumulator& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ACCUMULATOR = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
period_t getSummaryType(const string_q& type) {
    if (type == "hourly")
        return BY_HOUR;
    if (type == "daily")
        return BY_DAY;
    if (type == "weekly")
        return BY_WEEK;
    if (type == "monthly")
        return BY_MONTH;
    if (type == "quarterly")
        return BY_QUARTER;
    if (type == "annually")
        return BY_YEAR;
    return BY_NOTHING;
}
// EXISTING_CODE
}  // namespace qblocks
