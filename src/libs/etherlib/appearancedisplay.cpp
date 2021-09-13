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
#include "appearancedisplay.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAppearanceDisplay, CAccountName);

//---------------------------------------------------------------------------
extern string_q nextAppearancedisplayChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAppearancedisplayChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAppearanceDisplay::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["appearancedisplay_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAppearancedisplayChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAppearancedisplayChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAppearanceDisplay*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAppearanceDisplay::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAppearancedisplayChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
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
bool CAppearanceDisplay::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAppearanceDisplay::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceDisplay::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountName::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> transactionIndex;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceDisplay::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << transactionIndex;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceDisplay::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAppearanceDisplay copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceDisplayArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAppearanceDisplayArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAppearanceDisplay::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAppearanceDisplay, "schema"))
        return;

    CAccountName::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CAppearanceDisplay, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "transactionIndex", T_BLOCKNUM, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAppearanceDisplay, "schema");
    HIDE_FIELD(CAppearanceDisplay, "deleted");
    HIDE_FIELD(CAppearanceDisplay, "showing");
    HIDE_FIELD(CAppearanceDisplay, "cname");

    builtIns.push_back(_biCAppearanceDisplay);

    // EXISTING_CODE
    ADD_FIELD(CAppearanceDisplay, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CAppearanceDisplay, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CAppearanceDisplay, "timestamp");
    HIDE_FIELD(CAppearanceDisplay, "date");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAppearancedisplayChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAppearanceDisplay* app = reinterpret_cast<const CAppearanceDisplay*>(dataPtr);
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date") {
                    timestamp_t ts = getTimestampAt(app->blockNumber);
                    return ts_2_Date(ts).Format(FMT_JSON);
                }
                break;
            case 't':
                if (fieldIn % "timestamp")
                    return ts_2_Str(getTimestampAt(app->blockNumber));
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, app);
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
bool CAppearanceDisplay::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAppearanceDisplay& app) {
    app.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceDisplay& app) {
    app.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAppearanceDisplay& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APPEARANCEDISPLAY =
    "[{ADDRESS}]\t"
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]\t"
    "[{NAME}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
