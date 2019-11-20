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
#include "balancedelta.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBalanceDelta, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextBalancedeltaChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextBalancedeltaChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CBalanceDelta::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["balancedelta_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBalancedeltaChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBalancedeltaChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBalanceDelta*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CBalanceDelta::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextBalancedeltaChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            if (fieldName % "balance") {
                return wei_2_Str(balance);
            }
            break;
        case 'd':
            if (fieldName % "diff") {
                return bni_2_Str(diff);
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
bool CBalanceDelta::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "balance") {
                balance = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "diff") {
                diff = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBalanceDelta::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBalanceDelta::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    // archive >> address;
    archive >> balance;
    archive >> diff;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBalanceDelta::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    // archive << address;
    archive << balance;
    archive << diff;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBalanceDeltaArray& array) {
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
CArchive& operator<<(CArchive& archive, const CBalanceDeltaArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBalanceDelta::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CBalanceDelta, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CBalanceDelta, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "address", T_ADDRESS, ++fieldNum);
    HIDE_FIELD(CBalanceDelta, "address");
    ADD_FIELD(CBalanceDelta, "balance", T_WEI, ++fieldNum);
    ADD_FIELD(CBalanceDelta, "diff", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBalanceDelta, "schema");
    HIDE_FIELD(CBalanceDelta, "deleted");
    HIDE_FIELD(CBalanceDelta, "showing");
    HIDE_FIELD(CBalanceDelta, "cname");

    builtIns.push_back(_biCBalanceDelta);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBalancedeltaChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CBalanceDelta* bal = reinterpret_cast<const CBalanceDelta*>(dataPtr);
    if (bal) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'e':
                if (fieldIn % "ether")
                    return wei_2_Ether(bnu_2_Str(bal->balance));
                if (fieldIn % "etherDiff") {
                    string_q res = bal->getValueByName("diff");
                    bool neg = contains(res, "-");
                    res = substitute(res, "-", "");
                    res = wei_2_Ether(res);
                    return ((neg ? "-" : "+") + res);
                }
                break;
            case 'd':
                if (fieldIn % "diff")
                    return bni_2_Str(bal->diff);
                if (fieldIn % "dollars")
                    return getDispBal(bal->blockNumber, bal->balance);
                if (fieldIn % "dollarsDiff") {
                    bigint_t d = bal->diff;
                    if (d < 0)
                        d *= -1;
                    biguint_t u = str_2_BigUint(bni_2_Str(d));
                    if (bal->diff >= 0)
                        return "+" + getDispBal(bal->blockNumber, u);
                    return "-" + getDispBal(bal->blockNumber, u);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, bal);
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
bool CBalanceDelta::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CBalanceDelta& bal) {
    bal.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBalanceDelta& bal) {
    bal.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBalanceDelta& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_BALANCEDELTA =
    "[{BLOCKNUMBER}]\t"
    "[{ADDRESS}]\t"
    "[{BALANCE}]\t"
    "[{DIFF}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
