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
#include "rpcresult.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRPCResult, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextRpcresultChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextRpcresultChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CRPCResult::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["rpcresult_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextRpcresultChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRpcresultChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CRPCResult*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CRPCResult::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextRpcresultChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'i':
            if (fieldName % "id") {
                return id;
            }
            break;
        case 'j':
            if (fieldName % "jsonrpc") {
                return jsonrpc;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                return result;
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
bool CRPCResult::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // Note: This class doubles as a name/value pair for json parsing
    if (fieldName == "name")
        fieldName = "jsonrpc";
    else if (fieldName == "value")
        fieldName = "result";
    else if (fieldName == "error")
        fieldName = "result";
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'i':
            if (fieldName % "id") {
                id = fieldValue;
                return true;
            }
            break;
        case 'j':
            if (fieldName % "jsonrpc") {
                jsonrpc = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                result = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CRPCResult::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CRPCResult::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> jsonrpc;
    archive >> result;
    archive >> id;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRPCResult::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << jsonrpc;
    archive << result;
    archive << id;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRPCResult::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CRPCResult copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRPCResultArray& array) {
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
CArchive& operator<<(CArchive& archive, const CRPCResultArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CRPCResult::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CRPCResult, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CRPCResult, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CRPCResult, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CRPCResult, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CRPCResult, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CRPCResult, "jsonrpc", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRPCResult, "result", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRPCResult, "id", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRPCResult, "schema");
    HIDE_FIELD(CRPCResult, "deleted");
    HIDE_FIELD(CRPCResult, "showing");
    HIDE_FIELD(CRPCResult, "cname");

    builtIns.push_back(_biCRPCResult);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRpcresultChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CRPCResult* rpc = reinterpret_cast<const CRPCResult*>(dataPtr);
    if (rpc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, rpc);
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
bool CRPCResult::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CRPCResult& rpc) {
    rpc.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRPCResult& rpc) {
    rpc.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRPCResult& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_RPCRESULT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//--------------------------------------------------------------------
string_q extractRPCError(const string_q& resultIn) {
    string_q result = resultIn;
    CRPCResult generic;
    generic.parseJson3(result);  // pull out the result
    replace(generic.result, "code:", "name:");
    replace(generic.result, "message:", "value:");
    generic.parseJson3(generic.result);
    extern void restoreSpaces(string_q & str);
    restoreSpaces(generic.result);
    return "RPCerror(" + generic.id + "): " + generic.result;
}
// EXISTING_CODE
}  // namespace qblocks
