/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include <algorithm>
#include "incomestatement.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CIncomeStatement, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextIncomestatementChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextIncomestatementChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CIncomeStatement::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["incomestatement_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextIncomestatementChunk, this);
}

//---------------------------------------------------------------------------
string_q nextIncomestatementChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CIncomeStatement*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CIncomeStatement::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextIncomestatementChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "begBal") {
                return bni_2_Str(begBal);
            }
            if (fieldName % "blockNum") {
                return uint_2_Str(blockNum);
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                return bni_2_Str(endBal);
            }
            break;
        case 'g':
            if (fieldName % "gasCostInWei") {
                return bni_2_Str(gasCostInWei);
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                return bni_2_Str(inflow);
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                return bni_2_Str(outflow);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "begBal") {
                begBal = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "blockNum") {
                blockNum = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                endBal = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gasCostInWei") {
                gasCostInWei = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                inflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                outflow = str_2_Wei(fieldValue);
                return true;
            }
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
bool CIncomeStatement::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> begBal;
    archive >> inflow;
    archive >> outflow;
    archive >> gasCostInWei;
    archive >> endBal;
    archive >> blockNum;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << begBal;
    archive << inflow;
    archive << outflow;
    archive << gasCostInWei;
    archive << endBal;
    archive << blockNum;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIncomeStatementArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CIncomeStatementArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CIncomeStatement::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CIncomeStatement, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CIncomeStatement, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "begBal", T_INT256, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "inflow", T_INT256, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "outflow", T_INT256, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "gasCostInWei", T_INT256, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBal", T_INT256, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "blockNum", T_BLOCKNUM, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CIncomeStatement, "schema");
    HIDE_FIELD(CIncomeStatement, "deleted");
    HIDE_FIELD(CIncomeStatement, "showing");
    HIDE_FIELD(CIncomeStatement, "cname");

    builtIns.push_back(_biCIncomeStatement);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextIncomestatementChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CIncomeStatement* inc = reinterpret_cast<const CIncomeStatement*>(dataPtr);
    if (inc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, inc);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CIncomeStatement::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CIncomeStatement& inc) {
    inc.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CIncomeStatement& inc) {
    inc.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CIncomeStatement& item) {
    // EXISTING_CODE
    if (sizeof(item) != 0) {  // do this to always go through here, but avoid a warning
        uint64_t width = 22;
        if (item.begBal == item.endBal && item.begBal == -1) {
            os << padCenter("begBal", width) << "   " << padCenter("inFlow", width) << "   "
               << padCenter("outFlow", width) << "   " << padCenter("gasCost", width) << "   "
               << padCenter("endBal", width);
        } else {
            os << (item.begBal > 0 ? cGreen : bBlack) << padLeft(wei_2_Ether(bni_2_Str(item.begBal)), width) << bBlack
               << "   ";
            os << (item.inflow > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.inflow)), width) << bBlack
               << "   ";
            os << (item.outflow > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.outflow)), width) << bBlack
               << "   ";
            os << (item.gasCostInWei > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.gasCostInWei)), width)
               << cOff << "   ";
            os << (item.endBal > 0 ? cGreen : bBlack) << padLeft(wei_2_Ether(bni_2_Str(item.endBal)), width);
        }
        { return os; }
    }
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_INCOMESTATEMENT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
// EXISTING_CODE
}  // namespace qblocks
