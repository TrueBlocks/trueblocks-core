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
#include "addowner.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QAddOwner, CTransaction, curVersion);

//---------------------------------------------------------------------------
static SFString nextAddownerChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextAddownerChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QAddOwner::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextAddownerChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAddownerChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QAddOwner *add = (const QAddOwner *)data;
    if (add) {
        // Give customized code a chance to override first
        SFString ret = nextAddownerChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_owner" ) return fromAddress(add->_owner);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, add);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QAddOwner::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_owner" ) { _owner = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QAddOwner::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QAddOwner::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const QAddOwner*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _owner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QAddOwner::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _owner;

    return true;
}

//---------------------------------------------------------------------------
void QAddOwner::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QAddOwner, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(QAddOwner, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(QAddOwner, "_owner", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QAddOwner, "schema");
    HIDE_FIELD(QAddOwner, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAddownerChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QAddOwner *add = (const QAddOwner *)data;
    if (add) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, add);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool QAddOwner::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QAddOwner::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

