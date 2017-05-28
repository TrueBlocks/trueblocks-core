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
#include "changerequirement.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QChangeRequirement, CTransaction, curVersion);

//---------------------------------------------------------------------------
static SFString nextChangerequirementChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextChangerequirementChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QChangeRequirement::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextChangerequirementChunk, this);
}

//---------------------------------------------------------------------------
SFString nextChangerequirementChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QChangeRequirement *cha = (const QChangeRequirement *)data;
    if (cha) {
        // Give customized code a chance to override first
        SFString ret = nextChangerequirementChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_newRequired" ) return asStringBN(cha->_newRequired);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, cha);
        if (!ret.empty())
            return ret;
    }

    return "Field not found: [{" + fieldIn + "}]\n";
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newRequired" ) { _newRequired = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QChangeRequirement::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const QChangeRequirement*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _newRequired;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _newRequired;

    return true;
}

//---------------------------------------------------------------------------
void QChangeRequirement::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QChangeRequirement, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(QChangeRequirement, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "_newRequired", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QChangeRequirement, "schema");
    HIDE_FIELD(QChangeRequirement, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextChangerequirementChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QChangeRequirement *cha = (const QChangeRequirement *)data;
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, cha);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool QChangeRequirement::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QChangeRequirement::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

