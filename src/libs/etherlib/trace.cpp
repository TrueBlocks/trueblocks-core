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
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTrace, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextTraceChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTraceChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTrace::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTraceChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTrace *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTrace::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "action") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        action.parseJson(p,nFields);
        return true;

    } else if (fieldName % "result") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        result.parseJson(p,nFields);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "action" ) { /* action = fieldValue; */ return false; }
            break;
        case 'b':
            if ( fieldName % "blockHash" ) { blockHash = toHash(fieldValue); return true; }
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "error" ) { error = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "result" ) { /* result = fieldValue; */ return false; }
            break;
        case 's':
            if ( fieldName % "subtraces" ) { subtraces = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "traceAddress" ) {
                SFString str = fieldValue;
                while (!str.empty()) {
                    traceAddress[traceAddress.getCount()] = toAddress(nextTokenClear(str,','));
                }
                return true;
            }
            if ( fieldName % "transactionHash" ) { transactionHash = toHash(fieldValue); return true; }
            if ( fieldName % "transactionPosition" ) { transactionPosition = toUnsigned(fieldValue); return true; }
            if ( fieldName % "type" ) { type = fieldValue; return true; }
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
bool CTrace::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CTrace*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockHash;
    archive >> blockNumber;
    archive >> subtraces;
    archive >> traceAddress;
    archive >> transactionHash;
    archive >> transactionPosition;
    archive >> type;
    archive >> error;
    archive >> action;
    archive >> result;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTrace::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockHash;
    archive << blockNumber;
    archive << subtraces;
    archive << traceAddress;
    archive << transactionHash;
    archive << transactionPosition;
    archive << type;
    archive << error;
    archive << action;
    archive << result;

    return true;
}

//---------------------------------------------------------------------------
void CTrace::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTrace, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTrace, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTrace, "blockHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTrace, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "subtraces", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "traceAddress", T_ADDRESS|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CTrace, "transactionHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTrace, "transactionPosition", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrace, "error", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrace, "action", T_OBJECT, ++fieldNum);
    ADD_FIELD(CTrace, "result", T_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTrace, "schema");
    HIDE_FIELD(CTrace, "deleted");
    HIDE_FIELD(CTrace, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTrace *tra = (const CTrace *)dataPtr;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTrace::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTrace::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CTrace::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTraceChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "action" ) { expContext().noFrst=true; return action.Format(); }
            break;
        case 'b':
            if ( fieldName % "blockHash" ) return fromHash(blockHash);
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'e':
            if ( fieldName % "error" ) return error;
            break;
        case 'r':
            if ( fieldName % "result" ) { expContext().noFrst=true; return result.Format(); }
            break;
        case 's':
            if ( fieldName % "subtraces" ) return asStringU(subtraces);
            break;
        case 't':
            if ( fieldName % "traceAddress" || fieldName % "traceAddressCnt" ) {
                uint32_t cnt = traceAddress.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += ("\"" + traceAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
            }
            if ( fieldName % "transactionHash" ) return fromHash(transactionHash);
            if ( fieldName % "transactionPosition" ) return asStringU(transactionPosition);
            if ( fieldName % "type" ) return type;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    SFString s;
    s = toUpper(SFString("action")) + "::";
    if (fieldName.Contains(s)) {
        SFString f = fieldName;
        f.ReplaceAll(s,"");
        f = action.getValueByName(f);
        return f;
    }

    s = toUpper(SFString("result")) + "::";
    if (fieldName.Contains(s)) {
        SFString f = fieldName;
        f.ReplaceAll(s,"");
        f = result.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTrace& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CTrace::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "action" )
        return &action;
    if ( fieldName % "result" )
        return &result;
    return NULL;
}

//---------------------------------------------------------------------------
const SFString CTrace::getStringAt(const SFString& name, uint32_t i) const {
    if ( name % "traceAddress" && i < traceAddress.getCount() )
        return (traceAddress[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CTrace::isError(void) const {
    return !error.empty();
}
// EXISTING_CODE
}  // namespace qblocks

