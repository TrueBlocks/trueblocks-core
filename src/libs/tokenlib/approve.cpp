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
IMPLEMENT_NODE(QApprove, CTransaction, dataVersion);

//---------------------------------------------------------------------------
static SFString nextApproveChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextApproveChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QApprove::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextApproveChunk, this);
}

//---------------------------------------------------------------------------
SFString nextApproveChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QApprove *app = (const QApprove *)data;
    if (app) {
        // Give customized code a chance to override first
        SFString ret = nextApproveChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_spender" ) return fromAddress(app->_spender);
                if ( fieldIn % "_value" ) return asStringBN(app->_value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, app);
        if (!ret.empty())
            return ret;
    }

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
            if ( fieldName % "_value" ) { _value = toUnsigned(fieldValue); return true; }
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

    CTransaction::Serialize(archive);

    archive >> _spender;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QApprove::SerializeC(SFArchive& archive) const {
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
    ADD_FIELD(QApprove, "_spender", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QApprove, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QApprove, "schema");
    HIDE_FIELD(QApprove, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextApproveChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QApprove *app = (const QApprove *)data;
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, app);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QApprove::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QApprove::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

