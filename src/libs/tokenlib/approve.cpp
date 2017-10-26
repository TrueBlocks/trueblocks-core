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
#include "approve.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QApprove, CTransaction);

//---------------------------------------------------------------------------
static SFString nextApproveChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextApproveChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QApprove::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextApproveChunk, this);
}

//---------------------------------------------------------------------------
SFString nextApproveChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QApprove *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QApprove::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_spender" ) { _spender = toAddress(fieldValue); return true; }
            if ( fieldName % "_value" ) { _value = toWei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QApprove::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QApprove::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QApprove*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _spender;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QApprove::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _spender;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
void QApprove::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QApprove, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QApprove, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QApprove, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QApprove, "_spender", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QApprove, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QApprove, "schema");
    HIDE_FIELD(QApprove, "deleted");
    HIDE_FIELD(QApprove, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextApproveChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QApprove *app = (const QApprove *)dataPtr;
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, app);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QApprove::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QApprove::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QApprove::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextApproveChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_spender" ) return fromAddress(_spender);
            if ( fieldName % "_value" ) return asStringBN(_value);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QApprove& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

