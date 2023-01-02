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
#include "block.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlock, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextBlockChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextBlockChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CBlock::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["block_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBlockChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBlockChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBlock*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CBlock::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextBlockChunk_custom(fieldName, this);
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
            if (fieldName % "baseFeePerGas") {
                return wei_2_Str(baseFeePerGas);
            }
            break;
        case 'd':
            if (fieldName % "difficulty") {
                return uint_2_Str(difficulty);
            }
            break;
        case 'f':
            if (fieldName % "finalized") {
                return bool_2_Str(finalized);
            }
            break;
        case 'g':
            if (fieldName % "gasLimit") {
                return gas_2_Str(gasLimit);
            }
            if (fieldName % "gasUsed") {
                return gas_2_Str(gasUsed);
            }
            break;
        case 'h':
            if (fieldName % "hash") {
                return hash_2_Str(hash);
            }
            break;
        case 'm':
            if (fieldName % "miner") {
                return addr_2_Str(miner);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'p':
            if (fieldName % "parentHash") {
                return hash_2_Str(parentHash);
            }
            break;
        case 't':
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
            }
            if (fieldName % "transactions" || fieldName % "transactionsCnt") {
                size_t cnt = transactions.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += transactions[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if (fieldName % "tx_hashes" || fieldName % "tx_hashesCnt") {
                size_t cnt = tx_hashes.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + tx_hashes[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            break;
        case 'u':
            if (fieldName % "unclesCnt") {
                return uint_2_Str(unclesCnt);
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
bool CBlock::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "baseFeePerGas") {
                baseFeePerGas = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "difficulty") {
                difficulty = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "finalized") {
                finalized = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gasLimit") {
                gasLimit = str_2_Gas(fieldValue);
                return true;
            }
            if (fieldName % "gasUsed") {
                gasUsed = str_2_Gas(fieldValue);
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hash") {
                hash = str_2_Hash(fieldValue);
                return true;
            }
            break;
        case 'm':
            if (fieldName % "miner") {
                miner = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "parentHash") {
                parentHash = str_2_Hash(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            if (fieldName % "transactions") {
                CTransaction obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    transactions.push_back(obj);
                    obj = CTransaction();  // reset
                }
                return true;
            }
            if (fieldName % "tx_hashes") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    tx_hashes.push_back(nextTokenClear(str, ','));
                }
                return true;
            }
            break;
        case 'u':
            if (fieldName % "unclesCnt") {
                unclesCnt = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBlock::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBlock::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> gasLimit;
    archive >> gasUsed;
    archive >> hash;
    archive >> blockNumber;
    archive >> parentHash;
    archive >> miner;
    archive >> difficulty;
    archive >> finalized;
    archive >> timestamp;
    archive >> baseFeePerGas;
    archive >> transactions;
    // archive >> tx_hashes;
    // archive >> name;
    // archive >> unclesCnt;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBlock::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << gasLimit;
    archive << gasUsed;
    archive << hash;
    archive << blockNumber;
    archive << parentHash;
    archive << miner;
    archive << difficulty;
    archive << finalized;
    archive << timestamp;
    archive << baseFeePerGas;
    archive << transactions;
    // archive << tx_hashes;
    // archive << name;
    // archive << unclesCnt;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBlock::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CBlock copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBlockArray& array) {
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
CArchive& operator<<(CArchive& archive, const CBlockArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CBlock, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CBlock, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CBlock, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CBlock, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "hash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CBlock, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CBlock, "parentHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CBlock, "miner", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CBlock, "difficulty", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "finalized", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CBlock, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CBlock, "baseFeePerGas", T_WEI, ++fieldNum);
    ADD_FIELD(CBlock, "transactions", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CBlock, "tx_hashes", T_TEXT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CBlock, "tx_hashes");
    ADD_FIELD(CBlock, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CBlock, "name");
    ADD_FIELD(CBlock, "unclesCnt", T_UNUMBER, ++fieldNum);
    HIDE_FIELD(CBlock, "unclesCnt");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlock, "schema");
    HIDE_FIELD(CBlock, "deleted");
    HIDE_FIELD(CBlock, "showing");
    HIDE_FIELD(CBlock, "cname");

    builtIns.push_back(_biCBlock);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBlockChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CBlock* blo = reinterpret_cast<const CBlock*>(dataPtr);
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, blo);
                // EXISTING_CODE
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
bool CBlock::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CBlock& blo) {
    blo.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBlock& blo) {
    blo.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBlock& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CBlock::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "transactions") {
        if (index == NOPOS) {
            CTransaction empty;
            ((CBlock*)this)->transactions.push_back(empty);  // NOLINT
            index = transactions.size() - 1;
        }
        if (index < transactions.size())
            return &transactions[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const string_q CBlock::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "tx_hashes" && i < tx_hashes.size())
        return (tx_hashes[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_BLOCK =
    "[{BLOCKNUMBER}]\t"
    "[{TIMESTAMP}]\t"
    "[{DIFFICULTY}]\t"
    "[{GASUSED}]\t"
    "[{GASLIMIT}]\t"
    "[{BASEFEEPERGAS}]\t"
    "[{MINER}]\t"
    "[{HASH}]\t"
    "[{PARENTHASH}]\t"
    "[{TRANSACTIONSCNT}]\t"
    "[{FINALIZED}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
