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
#include "traceTest.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceTest, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTracetestChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTracetestChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTraceTest::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["tracetest_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextTracetestChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTracetestChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTraceTest*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTraceTest::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTracetestChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "enabled") {
                return enabled;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                return fileName;
            }
            break;
        case 'm':
            if (fieldName % "method") {
                return method;
            }
            break;
        case 'n':
            if (fieldName % "number") {
                return number;
            }
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'p':
            if (fieldName % "params") {
                return params;
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
bool CTraceTest::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "enabled") {
                enabled = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                fileName = fieldValue;
                return true;
            }
            break;
        case 'm':
            if (fieldName % "method") {
                method = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "number") {
                number = fieldValue;
                return true;
            }
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "params") {
                params = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceTest::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceTest::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> enabled;
    archive >> number;
    archive >> method;
    archive >> name;
    archive >> params;
    archive >> fileName;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceTest::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << enabled;
    archive << number;
    archive << method;
    archive << name;
    archive << params;
    archive << fileName;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceTestArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTraceTestArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTraceTest::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTraceTest, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTraceTest, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceTest, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceTest, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceTest, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceTest, "enabled", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceTest, "number", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceTest, "method", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceTest, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceTest, "params", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceTest, "fileName", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceTest, "schema");
    HIDE_FIELD(CTraceTest, "deleted");
    HIDE_FIELD(CTraceTest, "showing");
    HIDE_FIELD(CTraceTest, "cname");

    builtIns.push_back(_biCTraceTest);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTracetestChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTraceTest* tra = reinterpret_cast<const CTraceTest*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tra);
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
bool CTraceTest::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceTest& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRACETEST =
    "[{ENABLED}]\t"
    "[{NUMBER}]\t"
    "[{METHOD}]\t"
    "[{NAME}]\t"
    "[{PARAMS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
