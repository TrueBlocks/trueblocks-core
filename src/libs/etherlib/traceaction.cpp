/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "traceaction.h"
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceAction, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextTraceactionChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTraceactionChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTraceAction::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTraceactionChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceactionChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTraceAction *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = toAddress(fieldValue); return true; }
            break;
        case 'b':
            if ( fieldName % "balance" ) { balance = toWei(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "callType" ) { callType = fieldValue; return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = toGas(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "refundAddress" ) { refundAddress = toAddress(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "to" ) { to = toAddress(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toWei(fieldValue); return true; }
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
bool CTraceAction::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CTraceAction*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> callType;
    archive >> from;
    archive >> gas;
    archive >> input;
    archive >> to;
    archive >> value;
    archive >> address;
    archive >> balance;
    archive >> refundAddress;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
    archive << callType;
    archive << from;
    archive << gas;
    archive << input;
    archive << to;
    archive << value;
    archive << address;
    archive << balance;
    archive << refundAddress;

    return true;
}

//---------------------------------------------------------------------------
void CTraceAction::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTraceAction, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceAction, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTraceAction, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTraceAction, "callType", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTraceAction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "value", T_WEI, ++fieldNum);
    ADD_FIELD(CTraceAction, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceAction, "balance", T_WEI, ++fieldNum);
    ADD_FIELD(CTraceAction, "refundAddress", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceAction, "schema");
    HIDE_FIELD(CTraceAction, "deleted");
    HIDE_FIELD(CTraceAction, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceactionChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTraceAction *tra = (const CTraceAction *)dataPtr;
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
bool CTraceAction::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTraceAction::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CTraceAction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTraceAction& tra) {
    tra.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CTraceAction::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTraceactionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return fromAddress(address);
            break;
        case 'b':
            if ( fieldName % "balance" ) return fromWei(balance);
            break;
        case 'c':
            if ( fieldName % "callType" ) return callType;
            break;
        case 'f':
            if ( fieldName % "from" ) return fromAddress(from);
            break;
        case 'g':
            if ( fieldName % "gas" ) return fromGas(gas);
            break;
        case 'i':
            if ( fieldName % "input" ) return input;
            break;
        case 'r':
            if ( fieldName % "refundAddress" ) return fromAddress(refundAddress);
            break;
        case 't':
            if ( fieldName % "to" ) return fromAddress(to);
            break;
        case 'v':
            if ( fieldName % "value" ) return fromWei(value);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceAction& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

