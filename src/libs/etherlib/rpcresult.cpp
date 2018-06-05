/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "rpcresult.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRPCResult, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextRpcresultChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextRpcresultChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CRPCResult::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextRpcresultChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRpcresultChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CRPCResult *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CRPCResult::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "id" ) { id = fieldValue; return true; }
            break;
        case 'j':
            if ( fieldName % "jsonrpc" ) { jsonrpc = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "result" ) { result = fieldValue; return true; }
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
bool CRPCResult::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CRPCResult*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> jsonrpc;
    archive >> result;
    archive >> id;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRPCResult::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << jsonrpc;
    archive << result;
    archive << id;

    return true;
}

//---------------------------------------------------------------------------
void CRPCResult::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CRPCResult, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CRPCResult, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CRPCResult, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CRPCResult, "jsonrpc", T_TEXT, ++fieldNum);
    ADD_FIELD(CRPCResult, "result", T_TEXT, ++fieldNum);
    ADD_FIELD(CRPCResult, "id", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRPCResult, "schema");
    HIDE_FIELD(CRPCResult, "deleted");
    HIDE_FIELD(CRPCResult, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextRpcresultChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CRPCResult *rpc = (const CRPCResult *)dataPtr;
    if (rpc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rpc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CRPCResult::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CRPCResult::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CRPCResult::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextRpcresultChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "id" ) return id;
            break;
        case 'j':
            if ( fieldName % "jsonrpc" ) return jsonrpc;
            break;
        case 'r':
            if ( fieldName % "result" ) return result;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRPCResult& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

