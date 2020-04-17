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
#include "traceaction.h"
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceAction, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTraceactionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTraceactionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTraceAction::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["traceaction_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTraceactionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTraceactionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTraceAction*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTraceAction::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTraceactionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'b':
            if (fieldName % "balance") {
                return wei_2_Str(balance);
            }
            break;
        case 'c':
            if (fieldName % "callType") {
                return callType;
            }
            break;
        case 'f':
            if (fieldName % "from") {
                return addr_2_Str(from);
            }
            break;
        case 'g':
            if (fieldName % "gas") {
                return gas_2_Str(gas);
            }
            break;
        case 'i':
            if (fieldName % "init") {
                return init;
            }
            if (fieldName % "input") {
                return input;
            }
            break;
        case 'r':
            if (fieldName % "refundAddress") {
                return addr_2_Str(refundAddress);
            }
            break;
        case 't':
            if (fieldName % "to") {
                return addr_2_Str(to);
            }
            break;
        case 'v':
            if (fieldName % "value") {
                return wei_2_Str(value);
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
bool CTraceAction::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
            if (fieldName % "balance") {
                balance = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "callType") {
                callType = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "from") {
                from = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gas") {
                gas = str_2_Gas(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "init") {
                init = fieldValue;
                return true;
            }
            if (fieldName % "input") {
                input = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "refundAddress") {
                refundAddress = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "to") {
                to = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                value = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceAction::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::Serialize(CArchive& archive) {
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
    archive >> balance;
    archive >> callType;
    archive >> from;
    archive >> gas;
    archive >> init;
    archive >> input;
    archive >> refundAddress;
    archive >> to;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << balance;
    archive << callType;
    archive << from;
    archive << gas;
    archive << init;
    archive << input;
    archive << refundAddress;
    archive << to;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceActionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTraceActionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTraceAction::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTraceAction, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTraceAction, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceAction, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceAction, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceAction, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "balance", T_WEI, ++fieldNum);
    ADD_FIELD(CTraceAction, "callType", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTraceAction, "init", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "refundAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "value", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceAction, "schema");
    HIDE_FIELD(CTraceAction, "deleted");
    HIDE_FIELD(CTraceAction, "showing");
    HIDE_FIELD(CTraceAction, "cname");

    builtIns.push_back(_biCTraceAction);

    // EXISTING_CODE
    ADD_FIELD(CTraceAction, "ether", T_ETHER, ++fieldNum)
    HIDE_FIELD(CTraceAction, "ether");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTraceactionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTraceAction* tra = reinterpret_cast<const CTraceAction*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'e':
                if (fieldIn % "ether")
                    return wei_2_Ether(bnu_2_Str(tra->value));
                break;
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
bool CTraceAction::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTraceAction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceAction& tra) {
    tra.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceAction& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRACEACTION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
