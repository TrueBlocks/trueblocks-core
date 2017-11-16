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
IMPLEMENT_NODE(CIncomeStatement, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextIncomestatementChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextIncomestatementChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CIncomeStatement::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextIncomestatementChunk, this);
}

//---------------------------------------------------------------------------
SFString nextIncomestatementChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CIncomeStatement *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
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
        case 'g':
            if ( fieldName % "gasCost" ) { gasCost = toLong(fieldValue); return true; }
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

    if (archive.isWriting())
        return ((const CIncomeStatement*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> begBal;
    archive >> inflow;
    archive >> outflow;
    archive >> gasCost;
    archive >> endBal;
    archive >> blockNum;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
    archive << begBal;
    archive << inflow;
    archive << outflow;
    archive << gasCost;
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
    ADD_FIELD(CIncomeStatement, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CIncomeStatement, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CIncomeStatement, "begBal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "inflow", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "outflow", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "gasCost", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "blockNum", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CIncomeStatement, "schema");
    HIDE_FIELD(CIncomeStatement, "deleted");
    HIDE_FIELD(CIncomeStatement, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextIncomestatementChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CIncomeStatement *inc = (const CIncomeStatement *)dataPtr;
    if (inc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, inc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CIncomeStatement::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CIncomeStatement::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
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
SFString CIncomeStatement::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextIncomestatementChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "begBal" ) return asStringBN(begBal);
            if ( fieldName % "blockNum" ) return asStringU(blockNum);
            break;
        case 'e':
            if ( fieldName % "endBal" ) return asStringBN(endBal);
            break;
        case 'g':
            if ( fieldName % "gasCost" ) return asStringBN(gasCost);
            break;
        case 'i':
            if ( fieldName % "inflow" ) return asStringBN(inflow);
            break;
        case 'o':
            if ( fieldName % "outflow" ) return asStringBN(outflow);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CIncomeStatement& item) {
    // EXISTING_CODE
    uint32_t width = 22;
    if (item.begBal == item.endBal && item.begBal == -1) {
        os << padCenter("begBal", width) << "   "
        << padCenter("inFlow", width) << "   "
        << padCenter("outFlow", width) << "   "
        << padCenter("gasCost", width) << "   "
        << padCenter("endBal", width);
    } else {
        os << (item.begBal>0?cGreen:bBlack) << padLeft(wei2Ether(to_string(item.begBal).c_str()),width) << bBlack << "   ";
        os << (item.inflow>0?cYellow:"") << padLeft(wei2Ether(to_string(item.inflow).c_str()),width) << bBlack << "   ";
        os << (item.outflow>0?cYellow:"") << padLeft(wei2Ether(to_string(item.outflow).c_str()),width) << bBlack << "   ";
        os << (item.gasCost>0?cYellow:"") << padLeft(wei2Ether(to_string(item.gasCost).c_str()),width) << cOff << "   ";
        os << (item.endBal>0?cGreen:bBlack) << padLeft(wei2Ether(to_string(item.endBal).c_str()),width);
    }
    { return os; }
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CIncomeStatement::reconcile(const SFAddress& addr, blknum_t blockNum1) {
    nodeBal = getBalance(addr, blockNum1, false);
    return balanced();
}
// EXISTING_CODE
}  // namespace qblocks

