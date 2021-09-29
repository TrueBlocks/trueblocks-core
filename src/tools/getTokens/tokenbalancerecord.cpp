/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "tokenbalancerecord.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTokenBalanceRecord, CMonitor);

//---------------------------------------------------------------------------
extern string_q nextTokenbalancerecordChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTokenbalancerecordChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTokenBalanceRecord::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["tokenbalancerecord_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTokenbalancerecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTokenbalancerecordChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTokenBalanceRecord*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTokenBalanceRecord::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTokenbalancerecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            if (fieldName % "balance") {
                return wei_2_Str(balance);
            }
            break;
        case 'd':
            if (fieldName % "date") {
                return date;
            }
            if (fieldName % "diff") {
                return bni_2_Str(diff);
            }
            break;
        case 'h':
            if (fieldName % "holder") {
                return addr_2_Str(holder);
            }
            break;
        case 'p':
            if (fieldName % "priorBalance") {
                return wei_2_Str(priorBalance);
            }
            break;
        case 't':
            if (fieldName % "totalSupply") {
                return wei_2_Str(totalSupply);
            }
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CMonitor::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTokenBalanceRecord::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "balance") {
        balance = str_2_Wei(fieldValue);
        return true;
    }
    // EXISTING_CODE

    if (CMonitor::setValueByName(fieldName, fieldValue))
        return true;
    switch (tolower(fieldName[0])) {
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
            if (fieldName % "date") {
                date = fieldValue;
                return true;
            }
            if (fieldName % "diff") {
                diff = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'h':
            if (fieldName % "holder") {
                holder = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "priorBalance") {
                priorBalance = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "totalSupply") {
                totalSupply = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTokenBalanceRecord::finishParse() {
    // EXISTING_CODE
    if (!decimals)
        decimals = 18;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTokenBalanceRecord::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CMonitor::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> date;
    archive >> totalSupply;
    archive >> transactionIndex;
    // archive >> holder;
    archive >> priorBalance;
    archive >> balance;
    archive >> diff;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTokenBalanceRecord::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CMonitor::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << date;
    archive << totalSupply;
    archive << transactionIndex;
    // archive << holder;
    archive << priorBalance;
    archive << balance;
    archive << diff;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTokenBalanceRecord::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CTokenBalanceRecord copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTokenBalanceRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTokenBalanceRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTokenBalanceRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTokenBalanceRecord, "schema"))
        return;

    CMonitor::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CTokenBalanceRecord, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "date", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "totalSupply", T_WEI, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "holder", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CTokenBalanceRecord, "holder");
    ADD_FIELD(CTokenBalanceRecord, "priorBalance", T_WEI, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "balance", T_WEI, ++fieldNum);
    ADD_FIELD(CTokenBalanceRecord, "diff", T_INT256, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTokenBalanceRecord, "schema");
    HIDE_FIELD(CTokenBalanceRecord, "deleted");
    HIDE_FIELD(CTokenBalanceRecord, "showing");
    HIDE_FIELD(CTokenBalanceRecord, "cname");

    builtIns.push_back(_biCTokenBalanceRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTokenbalancerecordChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTokenBalanceRecord* tok = reinterpret_cast<const CTokenBalanceRecord*>(dataPtr);
    if (tok) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'b':
                if (fieldIn % "balance") {
                    CExportContext save = expContext();
                    expContext().asEther = true;
                    string_q ret = wei_2_Export(tok->blockNumber, tok->balance, tok->decimals ? tok->decimals : 18);
                    expContext() = save;
                    return ret;
                }
                break;
            case 't':
                if (fieldIn % "totalSupply") {
                    CExportContext save = expContext();
                    expContext().asEther = true;
                    string_q ret = wei_2_Export(tok->blockNumber, tok->totalSupply, tok->decimals ? tok->decimals : 18);
                    expContext() = save;
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tok);
                // EXISTING_CODE
                if (fieldIn % "priorBalance") {
                    CExportContext save = expContext();
                    expContext().asEther = true;
                    string_q ret =
                        wei_2_Export(tok->blockNumber, tok->priorBalance, tok->decimals ? tok->decimals : 18);
                    expContext() = save;
                    return ret;
                }
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CTokenBalanceRecord::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTokenBalanceRecord& tok) {
    tok.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTokenBalanceRecord& tok) {
    tok.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTokenBalanceRecord& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TOKENBALANCERECORD =
    "[{BLOCKNUMBER}]\t"
    "[{HOLDER}]\t"
    "[{ADDRESS}]\t"
    "[{NAME}]\t"
    "[{SYMBOL}]\t"
    "[{DECIMALS}]\t"
    "[{TOTALSUPPLY}]\t"
    "[{BALANCE}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
