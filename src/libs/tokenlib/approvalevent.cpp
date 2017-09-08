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
#include "approvalevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QApprovalEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextApprovaleventChunk(const SFString& fieldIn, const void *data);
static SFString nextApprovaleventChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void QApprovalEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextApprovaleventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextApprovaleventChunk(const SFString& fieldIn, const void *data) {
    const QApprovalEvent *app = (const QApprovalEvent *)data;
    if (app) {
        // Give customized code a chance to override first
        SFString ret = nextApprovaleventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_owner" ) return fromAddress(app->_owner);
                if ( fieldIn % "_spender" ) return fromAddress(app->_spender);
                if ( fieldIn % "_value" ) return asStringBN(app->_value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, app);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QApprovalEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_owner" ) { _owner = toAddress(fieldValue); return true; }
            if ( fieldName % "_spender" ) { _spender = toAddress(fieldValue); return true; }
            if ( fieldName % "_value" ) { _value = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QApprovalEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QApprovalEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QApprovalEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> _owner;
    archive >> _spender;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QApprovalEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << _owner;
    archive << _spender;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
void QApprovalEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QApprovalEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QApprovalEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QApprovalEvent, "_owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QApprovalEvent, "_spender", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QApprovalEvent, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QApprovalEvent, "schema");
    HIDE_FIELD(QApprovalEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextApprovaleventChunk_custom(const SFString& fieldIn, const void *data) {
    const QApprovalEvent *app = (const QApprovalEvent *)data;
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
bool QApprovalEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QApprovalEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

