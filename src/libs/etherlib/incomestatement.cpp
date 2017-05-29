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
#include "incomestatement.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CIncomeStatement, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextIncomestatementChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextIncomestatementChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CIncomeStatement::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextIncomestatementChunk, this);
}

//---------------------------------------------------------------------------
SFString nextIncomestatementChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CIncomeStatement *inc = (const CIncomeStatement *)data;
    if (inc) {
        // Give customized code a chance to override first
        SFString ret = nextIncomestatementChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'b':
                if ( fieldIn % "begBal" ) return asStringBN(inc->begBal);
                if ( fieldIn % "blockNum" ) return asStringU(inc->blockNum);
                break;
            case 'e':
                if ( fieldIn % "endBal" ) return asStringBN(inc->endBal);
                break;
            case 'i':
                if ( fieldIn % "inflow" ) return asStringBN(inc->inflow);
                break;
            case 'o':
                if ( fieldIn % "outflow" ) return asStringBN(inc->outflow);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, inc);
        if (!ret.empty())
            return ret;
    }

    return "Field not found: [{" + fieldIn + "}]\n";
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "begBal" ) { begBal = toLong(fieldValue); return true; }
            if ( fieldName % "blockNum" ) { blockNum = toUnsigned(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "endBal" ) { endBal = toLong(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "inflow" ) { inflow = toLong(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "outflow" ) { outflow = toLong(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CIncomeStatement::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CIncomeStatement*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> begBal;
    archive >> inflow;
    archive >> outflow;
    archive >> endBal;
    archive >> blockNum;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << begBal;
    archive << inflow;
    archive << outflow;
    archive << endBal;
    archive << blockNum;

    return true;
}

//---------------------------------------------------------------------------
void CIncomeStatement::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CIncomeStatement, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CIncomeStatement, "begBal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "inflow", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "outflow", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "blockNum", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CIncomeStatement, "schema");
    HIDE_FIELD(CIncomeStatement, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextIncomestatementChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CIncomeStatement *inc = (const CIncomeStatement *)data;
    if (inc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, inc);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CIncomeStatement::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CIncomeStatement::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CIncomeStatement& inc) {
    inc.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CIncomeStatement& inc) {
    inc.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
ostream& operator<<(ostream& os, const CIncomeStatement& is) {
    int width=22;
    if (is.begBal == is.endBal && is.begBal == -1) {
        os << padCenter("begBal",width) << "   "
        << padCenter("inFlow",width) << "   "
        << padCenter("outFlow",width) << "   "
        << padCenter("endBal",width);
    } else {
        os << padLeft(wei2Ether(to_string(is.begBal).c_str()),width) << "   "
        << padLeft(wei2Ether(to_string(is.inflow).c_str()),width) << "   "
        << padLeft(wei2Ether(to_string(is.outflow).c_str()),width) << "   "
        << padLeft(wei2Ether(to_string(is.endBal).c_str()),width);
    }
    return os;
}
bool CIncomeStatement::reconcile(const SFAddress& addr, blknum_t blockNum) {
    nodeBal = getBalance(addr, blockNum, false); return balanced();
}
// EXISTING_CODE
}  // namespace qblocks

