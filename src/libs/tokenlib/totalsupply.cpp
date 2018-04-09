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
#include "totalsupply.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QTotalSupply, CTransaction);

//---------------------------------------------------------------------------
static SFString nextTotalsupplyChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTotalsupplyChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QTotalSupply::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTotalsupplyChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTotalsupplyChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QTotalSupply *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QTotalSupply::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
void QTotalSupply::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QTotalSupply::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QTotalSupply*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QTotalSupply::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE

    return true;
}

//---------------------------------------------------------------------------
void QTotalSupply::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QTotalSupply, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QTotalSupply, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QTotalSupply, "showing", T_BOOL,  ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QTotalSupply, "schema");
    HIDE_FIELD(QTotalSupply, "deleted");
    HIDE_FIELD(QTotalSupply, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTotalsupplyChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QTotalSupply *tot = (const QTotalSupply *)dataPtr;
    if (tot) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tot);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QTotalSupply::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QTotalSupply::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QTotalSupply::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTotalsupplyChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // No fields

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QTotalSupply& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

