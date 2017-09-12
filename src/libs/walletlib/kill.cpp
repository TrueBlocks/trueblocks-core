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
#include "kill.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QKill, CTransaction, dataSchema());

//---------------------------------------------------------------------------
static SFString nextKillChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextKillChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QKill::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextKillChunk, this);
}

//---------------------------------------------------------------------------
SFString nextKillChunk(const SFString& fieldIn, const void *dataPtr) {
    const QKill *kil = (const QKill *)dataPtr;
    if (kil) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = kil->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextKillChunk_custom(fieldIn, dataPtr);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_to" ) return fromAddress(kil->_to);
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, kil);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QKill::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_to" ) { _to = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QKill::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QKill::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QKill*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _to;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QKill::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _to;

    return true;
}

//---------------------------------------------------------------------------
void QKill::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QKill, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QKill, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QKill, "_to", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QKill, "schema");
    HIDE_FIELD(QKill, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextKillChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QKill *kil = (const QKill *)dataPtr;
    if (kil) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, kil);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QKill::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QKill::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QKill::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextKillChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_to" ) return fromAddress(_to);
            break;
    }
    return "";
#else
    return Format("[{"+toUpper(fieldName)+"}]");
#endif
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

