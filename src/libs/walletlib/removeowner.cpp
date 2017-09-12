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
#include "removeowner.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRemoveOwner, CTransaction, dataSchema());

//---------------------------------------------------------------------------
static SFString nextRemoveownerChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextRemoveownerChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRemoveOwner::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRemoveownerChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRemoveownerChunk(const SFString& fieldIn, const void *dataPtr) {
    const QRemoveOwner *rem = (const QRemoveOwner *)dataPtr;
    if (rem) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = rem->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextRemoveownerChunk_custom(fieldIn, dataPtr);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_owner" ) return fromAddress(rem->_owner);
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, rem);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRemoveOwner::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
void QRemoveOwner::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRemoveOwner::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QRemoveOwner*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _owner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRemoveOwner::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _owner;

    return true;
}

//---------------------------------------------------------------------------
void QRemoveOwner::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QRemoveOwner, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRemoveOwner, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRemoveOwner, "_owner", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRemoveOwner, "schema");
    HIDE_FIELD(QRemoveOwner, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextRemoveownerChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QRemoveOwner *rem = (const QRemoveOwner *)dataPtr;
    if (rem) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rem);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QRemoveOwner::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QRemoveOwner::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QRemoveOwner::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextRemoveownerChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_owner" ) return fromAddress(_owner);
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

