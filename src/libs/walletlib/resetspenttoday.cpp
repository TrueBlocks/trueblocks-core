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
#include "resetspenttoday.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QResetSpentToday, CTransaction);

//---------------------------------------------------------------------------
static SFString nextResetspenttodayChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextResetspenttodayChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QResetSpentToday::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextResetspenttodayChunk, this);
}

//---------------------------------------------------------------------------
SFString nextResetspenttodayChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QResetSpentToday *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QResetSpentToday::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QResetSpentToday::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QResetSpentToday::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QResetSpentToday*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QResetSpentToday::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    return true;
}

//---------------------------------------------------------------------------
void QResetSpentToday::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QResetSpentToday, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QResetSpentToday, "deleted", T_BOOL,  ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QResetSpentToday, "schema");
    HIDE_FIELD(QResetSpentToday, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextResetspenttodayChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QResetSpentToday *res = (const QResetSpentToday *)dataPtr;
    if (res) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, res);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QResetSpentToday::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QResetSpentToday::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QResetSpentToday::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextResetspenttodayChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // No fields

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QResetSpentToday& item) {
    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

