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
#include "etherlib.h"

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
        case 'a':
            if (fieldName % "asset") {
                return asset;
            }
            break;
        case 'b':
            if (fieldName % "blockNum") {
                return uint_2_Str(blockNum);
            }
            if (fieldName % "begBal") {
                return begBal;
            }
            if (fieldName % "begBalDiff") {
                return begBalDiff;
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                return endBal;
            }
            if (fieldName % "endBalCalc") {
                return endBalCalc;
            }
            if (fieldName % "endBalDiff") {
                return endBalDiff;
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                return inflow;
            }
            if (fieldName % "intInflow") {
                return intInflow;
            }
            if (fieldName % "intOutflow") {
                return intOutflow;
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                return outflow;
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                return reconciliationType;
            }
            if (fieldName % "reconciled") {
                return bool_2_Str(reconciled);
            }
            break;
        case 's':
            if (fieldName % "suicideInflow") {
                return suicideInflow;
            }
            if (fieldName % "suicideOutflow") {
                return suicideOutflow;
            }
            break;
        case 'w':
            if (fieldName % "weiGasCost") {
                return weiGasCost;
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
        case 'a':
            if (fieldName % "asset") {
                asset = fieldValue;
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNum") {
                blockNum = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "begBal") {
                begBal = fieldValue;
                return true;
            }
            if (fieldName % "begBalDiff") {
                begBalDiff = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                endBal = fieldValue;
                return true;
            }
            if (fieldName % "endBalCalc") {
                endBalCalc = fieldValue;
                return true;
            }
            if (fieldName % "endBalDiff") {
                endBalDiff = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                inflow = fieldValue;
                return true;
            }
            if (fieldName % "intInflow") {
                intInflow = fieldValue;
                return true;
            }
            if (fieldName % "intOutflow") {
                intOutflow = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                outflow = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                reconciliationType = fieldValue;
                return true;
            }
            if (fieldName % "reconciled") {
                reconciled = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "suicideInflow") {
                suicideInflow = fieldValue;
                return true;
            }
            if (fieldName % "suicideOutflow") {
                suicideOutflow = fieldValue;
                return true;
            }
            break;
        case 'w':
            if (fieldName % "weiGasCost") {
                weiGasCost = fieldValue;
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
    archive >> blockNum;
    archive >> asset;
    archive >> begBal;
    archive >> begBalDiff;
    archive >> inflow;
    archive >> outflow;
    archive >> intInflow;
    archive >> intOutflow;
    archive >> suicideInflow;
    archive >> suicideOutflow;
    archive >> weiGasCost;
    archive >> endBal;
    archive >> endBalCalc;
    archive >> endBalDiff;
    archive >> reconciliationType;
    archive >> reconciled;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CIncomeStatement::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNum;
    archive << asset;
    archive << begBal;
    archive << begBalDiff;
    archive << inflow;
    archive << outflow;
    archive << intInflow;
    archive << intOutflow;
    archive << suicideInflow;
    archive << suicideOutflow;
    archive << weiGasCost;
    archive << endBal;
    archive << endBalCalc;
    archive << endBalDiff;
    archive << reconciliationType;
    archive << reconciled;

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
    ADD_FIELD(CIncomeStatement, "blockNum", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "asset", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "begBal", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "begBalDiff", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "inflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "outflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "intInflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "intOutflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "suicideInflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "suicideOutflow", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "weiGasCost", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBal", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBalCalc", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "endBalDiff", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "reconciliationType", T_TEXT, ++fieldNum);
    ADD_FIELD(CIncomeStatement, "reconciled", T_BOOL, ++fieldNum);

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
    //    if (sizeof(item) != 0) {  // do this to always go through here, but avoid a warning
    //        uint64_t width = 22;
    //        if (item.begBal == item.endBal && item.begBal == -1) {
    //            os << padCenter("begBal", width) << "   " << padCenter("inFlow", width) << "   "
    //               << padCenter("outFlow", width) << "   " << padCenter("gasCost", width) << "   "
    //               << padCenter("endBal", width);
    //        } else {
    //            os << (item.begBal > 0 ? cGreen : bBlack) << padLeft(wei_2_Ether(bni_2_Str(item.begBal)), width) <<
    //            bBlack
    //               << "   ";
    //            os << (item.inflow > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.inflow)), width) <<
    //            bBlack
    //               << "   ";
    //            os << (item.outflow > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.outflow)), width) <<
    //            bBlack
    //               << "   ";
    //            os << (item.weiGasCost > 0 ? cYellow : "") << padLeft(wei_2_Ether(bni_2_Str(item.weiGasCost)), width)
    //               << cOff << "   ";
    //            os << (item.endBal > 0 ? cGreen : bBlack) << padLeft(wei_2_Ether(bni_2_Str(item.endBal)), width);
    //        }
    //        { return os; }
    //    }
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
inline string_q bni_2_Ether(const bigint_t& num) {
    if (num == 0)
        return "-";
    string_q ret = wei_2_Ether(str_2_Wei(bni_2_Str(num)));
    CStringArray parts;
    explode(parts, ret, '.');
    ret = parts[0] + ".";
    if (parts.size() == 1)
        return ret + "0000000";
    if (parts[1].length() >= 7)
        return ret + parts[1].substr(0, 7);
    return ret + parts[1] + string_q(7 - parts[1].length(), '0');
}

//---------------------------------------------------------------------------
CIncomeStatement::CIncomeStatement(const CReconciliationNumeric& nums) {
    blockNum = nums.blockNum;
    if (expContext().asEther) {
        begBal = bni_2_Ether(nums.begBal);
        begBalDiff = nums.begBalDiff >= 0 ? bni_2_Ether(nums.begBalDiff) : "-" + bni_2_Ether(nums.begBalDiff * -1);
        inflow = bni_2_Ether(nums.inflow);
        outflow = bni_2_Ether(nums.outflow);
        intInflow = bni_2_Ether(nums.intInflow);
        intOutflow = bni_2_Ether(nums.intOutflow);
        suicideInflow = bni_2_Ether(nums.suicideInflow);
        suicideOutflow = bni_2_Ether(nums.suicideOutflow);
        weiGasCost = bni_2_Ether(nums.weiGasCost);
        endBal = bni_2_Ether(nums.endBal);
        endBalCalc = bni_2_Ether(nums.endBalCalc);
        endBalDiff = nums.endBalDiff >= 0 ? bni_2_Ether(nums.endBalDiff) : "-" + bni_2_Ether(nums.endBalDiff * -1);
    } else {
        begBal = bni_2_Str(nums.begBal);
        begBalDiff = bni_2_Str(nums.begBalDiff);
        inflow = bni_2_Str(nums.inflow);
        outflow = bni_2_Str(nums.outflow);
        intInflow = bni_2_Str(nums.intInflow);
        intOutflow = bni_2_Str(nums.intOutflow);
        suicideInflow = bni_2_Str(nums.suicideInflow);
        suicideOutflow = bni_2_Str(nums.suicideOutflow);
        weiGasCost = bni_2_Str(nums.weiGasCost);
        endBal = bni_2_Str(nums.endBal);
        endBalCalc = bni_2_Str(nums.endBalCalc);
        endBalDiff = bni_2_Str(nums.endBalDiff);
    }
    asset = nums.asset;
    reconciliationType = nums.reconciliationType;
    reconciled = nums.reconciled;
}
// EXISTING_CODE
}  // namespace qblocks
