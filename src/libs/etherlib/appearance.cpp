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
#include "appearance.h"
#include "indexedaddress.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAppearance, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextAppearanceChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAppearanceChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAppearance::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["appearance_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAppearanceChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAppearanceChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAppearance*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAppearance::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAppearanceChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    if (fieldName % "traceIndex") {
        if (traceIndex < 10)
            return (expContext().exportFmt == JSON1 || expContext().exportFmt == JSON1 ? "\"\"" : "");
        return uint_2_Str(traceIndex - 10);
    }
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            break;
        case 'd':
            if (fieldName % "date") {
                return date;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'r':
            if (fieldName % "reason") {
                return reason;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            if (fieldName % "traceIndex") {
                return uint_2_Str(traceIndex);
            }
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
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
bool CAppearance::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "date") {
                date = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reason") {
                reason = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "traceIndex") {
                traceIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAppearance::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> blockNumber;
    archive >> transactionIndex;
    archive >> traceIndex;
    archive >> reason;
    archive >> name;
    archive >> timestamp;
    archive >> date;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << blockNumber;
    archive << transactionIndex;
    archive << traceIndex;
    archive << reason;
    archive << name;
    archive << timestamp;
    archive << date;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAppearance copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAppearanceArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAppearance::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAppearance, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAppearance, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAppearance, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearance, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearance, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAppearance, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAppearance, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "traceIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "reason", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAppearance, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAppearance, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CAppearance, "date", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAppearance, "schema");
    HIDE_FIELD(CAppearance, "deleted");
    HIDE_FIELD(CAppearance, "showing");
    HIDE_FIELD(CAppearance, "cname");

    builtIns.push_back(_biCAppearance);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAppearanceChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAppearance* app = reinterpret_cast<const CAppearance*>(dataPtr);
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date") {
                    timestamp_t ts = bn_2_Timestamp(app->blockNumber);
                    return ts_2_Date(ts).Format(FMT_JSON);
                }
                break;
            case 't':
                if (fieldIn % "timestamp")
                    return ts_2_Str(bn_2_Timestamp(app->blockNumber));
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
bool CAppearance::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAppearance& app) {
    app.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearance& app) {
    app.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAppearance& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APPEARANCE =
    "[{ADDRESS}]\t"
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{TRACEINDEX}]\t"
    "[{REASON}]\t"
    "[{NAME}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
int findAddresses(const void* v1, const void* v2) {
    const CIndexedAddress* at1 = (CIndexedAddress*)v1;  // NOLINT
    const CIndexedAddress* at2 = (CIndexedAddress*)v2;  // NOLINT
    for (size_t i = 0; i < 20; i++) {
        int ret = at1->bytes[i] - at2->bytes[i];
        if (ret)
            return ret;
    }
    return 0;
}
// EXISTING_CODE
}  // namespace qblocks
