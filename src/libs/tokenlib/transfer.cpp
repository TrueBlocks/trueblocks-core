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
#include "transfer.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QTransfer, CTransaction, curVersion);

//---------------------------------------------------------------------------
static SFString nextTransferChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextTransferChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QTransfer::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextTransferChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTransferChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QTransfer *tra = (const QTransfer *)data;
    if (tra) {
        // Give customized code a chance to override first
        SFString ret = nextTransferChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_to" ) return fromAddress(tra->_to);
                if ( fieldIn % "_value" ) return asStringBN(tra->_value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, tra);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QTransfer::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_to" ) { _to = toAddress(fieldValue); return true; }
            if ( fieldName % "_value" ) { _value = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QTransfer::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QTransfer::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const QTransfer*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _to;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QTransfer::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _to;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
void QTransfer::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QTransfer, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QTransfer, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QTransfer, "_to", T_TEXT, ++fieldNum);
    ADD_FIELD(QTransfer, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QTransfer, "schema");
    HIDE_FIELD(QTransfer, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTransferChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QTransfer *tra = (const QTransfer *)data;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, tra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QTransfer::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QTransfer::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

