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
#include "trace.h"
#include "transaction.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTrace, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTraceChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTraceChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTrace::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["trace_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextTraceChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTraceChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTrace*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTrace::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTraceChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "action") {
                if (action == CTraceAction())
                    return "{}";
                return action.Format();
            }
            if (fieldName % "articulatedTrace") {
                if (articulatedTrace == CFunction())
                    return "{}";
                return articulatedTrace.Format();
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                return hash_2_Str(blockHash);
            }
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            break;
        case 'c':
            if (fieldName % "compressedTrace") {
                return compressedTrace;
            }
            break;
        case 'e':
            if (fieldName % "error") {
                return error;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                if (result == CTraceResult())
                    return "{}";
                return result.Format();
            }
            break;
        case 's':
            if (fieldName % "subtraces") {
                return uint_2_Str(subtraces);
            }
            break;
        case 't':
            if (fieldName % "traceAddress" || fieldName % "traceAddressCnt") {
                size_t cnt = traceAddress.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + traceAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            if (fieldName % "transactionHash") {
                return hash_2_Str(transactionHash);
            }
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            if (fieldName % "type") {
                return type;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // Weird note here -- the above case for traceAddress is not used because it's overridden further up.
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("action")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = action.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("result")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = result.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("articulatedTrace")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = articulatedTrace.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTrace::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (pTrans)
        if (((CTransaction*)pTrans)->setValueByName(fieldName, fieldValue))  // NOLINT
            return true;
    if (fieldName % "transactionPosition")  // order matters
        fieldName = "transactionIndex";
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "action") {
                return action.parseJson3(fieldValue);
            }
            if (fieldName % "articulatedTrace") {
                return articulatedTrace.parseJson3(fieldValue);
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                blockHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "compressedTrace") {
                compressedTrace = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "error") {
                error = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                return result.parseJson3(fieldValue);
            }
            break;
        case 's':
            if (fieldName % "subtraces") {
                subtraces = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "traceAddress") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    traceAddress.push_back(nextTokenClear(str, ','));
                }
                return true;
            }
            if (fieldName % "transactionHash") {
                transactionHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTrace::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTrace::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockHash;
    archive >> blockNumber;
    archive >> subtraces;
    archive >> traceAddress;
    archive >> transactionHash;
    archive >> transactionIndex;
    archive >> type;
    archive >> error;
    archive >> action;
    archive >> result;
    archive >> articulatedTrace;
    // archive >> compressedTrace;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTrace::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockHash;
    archive << blockNumber;
    archive << subtraces;
    archive << traceAddress;
    archive << transactionHash;
    archive << transactionIndex;
    archive << type;
    archive << error;
    archive << action;
    archive << result;
    archive << articulatedTrace;
    // archive << compressedTrace;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTraceArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTrace::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTrace, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTrace, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTrace, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTrace, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrace, "blockHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTrace, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTrace, "subtraces", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "traceAddress", T_TEXT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTrace, "transactionHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTrace, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTrace, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTrace, "error", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(CTrace, "action", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CTraceAction));
    ADD_OBJECT(CTrace, "result", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CTraceResult));
    ADD_OBJECT(CTrace, "articulatedTrace", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CFunction));
    ADD_FIELD(CTrace, "compressedTrace", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CTrace, "compressedTrace");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTrace, "schema");
    HIDE_FIELD(CTrace, "deleted");
    HIDE_FIELD(CTrace, "showing");
    HIDE_FIELD(CTrace, "cname");

    builtIns.push_back(_biCTrace);

    // EXISTING_CODE
    ADD_FIELD(CTrace, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CTrace, "date");
    HIDE_FIELD(CTrace, "articulatedTrace");
    SET_TYPE(CTrace, "traceAddress", T_TEXT);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTraceChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTrace* tra = reinterpret_cast<const CTrace*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "articulatedTrace") {
                    if (tra->articulatedTrace == CFunction())
                        return "{}";
                    string_q str = tra->articulatedTrace.Format();
                    return (str.empty() ? "{}" : str);
                }
                break;
            case 'c':
                if (fieldIn % "compressedTrace")
                    return stripWhitespace(tra->articulatedTrace.compressed());
                break;
            case 'd':
                if (tra->pTrans) {
                    if (fieldIn % "date" || fieldIn % "datesh")
                        return nextTransactionChunk(fieldIn, tra->pTrans);
                }
                break;
            case 'f':
                if (tra->pTrans) {
                    if (fieldIn % "function") {
                        string_q ret = tra->Format("[{ARTICULATEDTRACE}]");
                        if (ret.empty())
                            return "";
                        CFunction func;
                        func.parseJson3(ret);
                        return func.name;
                    }
                }
                break;
            case 'r':
                if (fieldIn % "result") {
                    if (isTurboGeth() && !tra->error.empty()) {
                        return "{}";
                    }
                }
            case 't':
                // Normally, we don't have to do this, but traceAddress is a weird case. It's an array, but we don't
                // really want to present it that way.
                if (fieldIn % "traceAddress" || fieldIn % "traceAddressCnt") {
                    size_t cnt = tra->traceAddress.size();
                    if (endsWith(toLower(fieldIn), "cnt"))
                        return uint_2_Str(max((size_t)1, cnt));
                    string_q retS;
                    for (size_t i = 0; i < cnt; i++) {
                        if (!retS.empty())
                            retS += "-";
                        retS += tra->traceAddress[i];
                    }
                    if (retS.empty())
                        retS = "null";
                    return retS;
                }
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
bool CTrace::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTrace& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CTrace::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "action")
        return &action;

    if (fieldName % "result")
        return &result;

    if (fieldName % "articulatedTrace")
        return &articulatedTrace;

    return NULL;
}

//---------------------------------------------------------------------------
const string_q CTrace::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "traceAddress" && i < traceAddress.size())
        return (traceAddress[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRACE =
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{TRACEADDRESS}]\t"
    "[{ACTION::CALLTYPE}]\t"
    "[{ERROR}]\t"
    "[{ACTION::FROM}]\t"
    "[{ACTION::TO}]\t"
    "[{ACTION::VALUE}]\t"
    "[{ACTION::ETHER}]\t"
    "[{ACTION::GAS}]\t"
    "[{RESULT::GASUSED}]\t"
    "[{ACTION::INPUT}]\t"
    "[{COMPRESSEDTRACE}]\t"
    "[{RESULT::OUTPUT}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CTrace::isError(void) const {
    return !error.empty();
}
// EXISTING_CODE
}  // namespace qblocks
