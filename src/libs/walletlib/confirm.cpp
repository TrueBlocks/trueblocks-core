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
#include "confirm.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QConfirm, CTransaction, dataSchema());

//---------------------------------------------------------------------------
static SFString nextConfirmChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextConfirmChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QConfirm::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextConfirmChunk, this);
}

//---------------------------------------------------------------------------
SFString nextConfirmChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QConfirm *con = (const QConfirm *)data;
    if (con) {
        // Give customized code a chance to override first
        SFString ret = nextConfirmChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_h" ) return con->_h;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, con);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_h" ) { _h = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QConfirm::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QConfirm*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _h;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _h;

    return true;
}

//---------------------------------------------------------------------------
void QConfirm::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QConfirm, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QConfirm, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QConfirm, "_h", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QConfirm, "schema");
    HIDE_FIELD(QConfirm, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextConfirmChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QConfirm *con = (const QConfirm *)data;
    if (con) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, con);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QConfirm::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QConfirm::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

