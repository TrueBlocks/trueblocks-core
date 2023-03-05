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
#include "appearance.h"

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
            break;
        default:
            break;
    }

    // EXISTING_CODE
    if (isTestMode() && fieldName % "blockHash")
        return hash_2_Str(hash);
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CBlock::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // LOG4("CBlock::setValueByName --> " + fieldName + "=" + fieldValue.substr(0,50));
    if (fieldName % "number") {
        fieldName = "blockNumber";  // NOLINT

    } else if (fieldName % "author") {
        fieldName = "miner";  // NOLINT

    } else if (fieldName % "blockHash") {
        fieldName = "hash";  // NOLINT

    } else if (fieldName % "baseFeePerGas") {
        fieldValue = substitute(fieldValue, "null", "0x0");

    } else if (fieldName % "transactions") {
        // Transactions come to us either as a JSON objects or lists of hashes (i.e. a string array).
        // JSON objects have (among other things) a 'from' field so we can identify the former by its absence
        if (!contains(fieldValue, "from")) {
            string_q str = fieldValue;
            while (!str.empty()) {
                CTransaction trans;
                trans.hash = str_2_Addr(nextTokenClear(str, ','));
                transactions.push_back(trans);
            }
            return true;
        }
    }
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
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBlock::finishParse() {
    // EXISTING_CODE
    for (size_t i = 0; i < transactions.size(); i++) {
        CTransaction* trans = &transactions.at(i);  // taking a non-const reference
        trans->pBlock = this;
    }
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

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlock, "schema");
    HIDE_FIELD(CBlock, "deleted");
    HIDE_FIELD(CBlock, "showing");
    HIDE_FIELD(CBlock, "cname");

    builtIns.push_back(_biCBlock);

    // EXISTING_CODE
    ADD_FIELD(CBlock, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CBlock, "date");
    ADD_FIELD(CBlock, "age", T_DATE, ++fieldNum);
    HIDE_FIELD(CBlock, "age");
    HIDE_FIELD(CBlock, "price");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBlockChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CBlock* blo = reinterpret_cast<const CBlock*>(dataPtr);
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "age") {
                    if (isTestMode())
                        return "100";
                    static CBlock latest;
                    if (latest.timestamp == 0)
                        getBlockHeader(latest, "latest");
                    timestamp_t myTs = (blo->timestamp);
                    timestamp_t blkTs = ((timestamp_t)latest.timestamp);
                    if (blkTs > myTs) {
                        return int_2_Str(blkTs - myTs);
                    }
                    return "0";
                }
                break;
            case 'd':
                if (fieldIn % "date") {
                    timestamp_t ts = (timestamp_t)blo->timestamp;
                    return ts_2_Date(ts).Format(FMT_JSON);
                }
                break;
            case 'n':
                if (fieldIn % "number")
                    return uint_2_Str(blo->blockNumber);
                break;
            case 't':
                if (expContext().hashesOnly && fieldIn % "transactions") {
                    size_t cnt = blo->transactions.size();
                    if (!cnt)
                        return "";
                    string_q ret;
                    for (size_t i = 0; i < cnt; i++) {
                        ret += blo->transactions[i].hash;
                        ret += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
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
    double removed1;
    biguint_t removed2;
    // TODO: Remove old upgrade code -- it's okay -- don't worry about it...
    if (m_schema < getVersionNum(0, 3, 1)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> removed2;  // used to be logsB loom
        archive >> blockNumber;
        archive >> parentHash;
        archive >> timestamp;
        archive >> transactions;
        // TODO(tjayrush): technically we should re-read these values from the node
        CBlock upgrade;
        string_q prev = setDataSource("local");
        queryBlock(upgrade, uint_2_Str(blockNumber), false);
        setDataSource(prev);
        miner = upgrade.miner;
        difficulty = upgrade.difficulty;
        finalized = false;
        finishParse();
        done = true;
    } else if (m_schema < getVersionNum(0, 4, 1)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> blockNumber;
        archive >> parentHash;
        archive >> miner;
        archive >> difficulty;
        archive >> removed1;  // price;
        archive >> timestamp;
        archive >> transactions;
        finalized = false;
        finishParse();
        done = true;
    } else if (m_schema < getVersionNum(0, 10, 4)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> blockNumber;
        archive >> parentHash;
        archive >> miner;
        archive >> difficulty;
        archive >> removed1;  // price;
        archive >> finalized;
        archive >> timestamp;
        archive >> transactions;
        if (m_schema == getVersionNum(0, 10, 3))
            archive >> baseFeePerGas;  // we need to read it, but we reset it anyway
        if (blockNumber < londonBlock()) {
            baseFeePerGas = 0;
        } else {
            CBlock upgrade;
            getBlockHeader(upgrade, uint_2_Hex(blockNumber));
            baseFeePerGas = upgrade.baseFeePerGas;
        }
        finalized = false;
        finishParse();
        done = true;
    }
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
    "[{FINALIZED}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
extern bool accumulateAddresses(const CAppearance& item, void* data);
extern bool foundOne(APPEARANCEFUNC func, void* data, blknum_t bn, blknum_t tx, blknum_t tc, const address_t& addr,
                     const string_q& reason);  // NOLINT
extern bool foundPot(APPEARANCEFUNC func, void* data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList,
                     const string_q& reason);  // NOLINT

//---------------------------------------------------------------------------
string_q toStringList(const CStringArray& array) {
    bool first = true;
    string_q ret;
    for (auto elem : array) {
        if (!first)
            ret += (elem + "_");
        first = false;
    }
    ret = "[" + trim(ret, '_') + "]";
    ret = substitute(ret, "[]", "");
    return ret.empty() ? "" : ret + "_";
}

//---------------------------------------------------------------------------
bool CBlock::forEveryAppearanceInBlock(APPEARANCEFUNC func, TRANSFUNC traceFilter, void* data) {
    if (!func)
        return false;

    foundOne(func, data, blockNumber, 99999, 0, miner, "miner");
    uint64_t nUncles = getUncleCount(blockNumber);
    for (size_t index = 0; index < nUncles; index++) {
        CBlock uncle;
        getUncle(uncle, blockNumber, index);
        foundOne(func, data, blockNumber, 99998, 0, uncle.miner, "uncle");
    }
    for (size_t tr = 0; tr < transactions.size(); tr++) {
        CTransaction* trans = &transactions[tr];
        if (!trans->forEveryAppearanceInTx(func, traceFilter, data))
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool getTracesAndVisit(const hash_t& hash, CAppearance& item, APPEARANCEFUNC funcy, void* data) {
    string_q str;
    queryRawTrace(str, hash);

    CRPCResult generic;
    generic.parseJson3(str);  // pull out the result

    blknum_t traceID = 0;
    CTrace trace;
    while (trace.parseJson3(generic.result)) {
        string_q trID = "trace_" + uint_2_Str(traceID) + "_" + toStringList(trace.traceAddress);
        if (!foundOne(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, trace.action.from,
                      trID + "from"))
            return false;
        if (!foundOne(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, trace.action.to, trID + "to"))
            return false;
        if (!foundOne(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, trace.action.refundAddress,
                      trID + "refundAddr"))
            return false;
        if (!foundOne(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, trace.action.selfDestructed,
                      trID + "self-destruct"))
            return false;
        if (!foundOne(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, trace.result.address,
                      trID + "self-destruct"))
            return false;
        string_q inpt = extract(trace.action.input, 10);
        if (!inpt.empty())
            if (!foundPot(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, inpt, trID + "input"))
                return false;
        string_q outpt = extract(trace.result.output, 2);
        if (!outpt.empty())
            if (!foundPot(funcy, data, item.blockNumber, item.transactionIndex, traceID + 10, outpt, trID + "output"))
                return false;
        traceID++;
        trace = CTrace();  // reset
    }
    return true;
}

//---------------------------------------------------------------------------
bool CTransaction::forEveryAppearanceInTx(APPEARANCEFUNC funcy, TRANSFUNC filt, void* data) {
    blknum_t tr = transactionIndex;
    const CReceipt* recPtr = &receipt;
    if (!foundOne(funcy, data, blockNumber, tr, 0, from, "from"))
        return false;
    if (!foundOne(funcy, data, blockNumber, tr, 0, to, "to"))
        return false;
    if (!foundOne(funcy, data, blockNumber, tr, 0, recPtr->contractAddress, "creation"))
        return false;
    if (!foundPot(funcy, data, blockNumber, tr, 0, extract(input, 10), "input"))
        return false;
    for (size_t l = 0; l < recPtr->logs.size(); l++) {
        const CLog* log = &recPtr->logs[l];
        string_q logId = "log_" + uint_2_Str(l) + "_";
        if (!foundOne(funcy, data, blockNumber, tr, 0, log->address, logId + "generator"))
            return false;
        for (size_t t = 0; t < log->topics.size(); t++) {
            address_t addr;
            string_q topId = uint_2_Str(t);
            if (isPotentialAddr(topic_2_BigUint(log->topics[t]), addr)) {
                if (!foundOne(funcy, data, blockNumber, tr, 0, addr, logId + "topic_" + topId))
                    return false;
            }
        }
        if (!foundPot(funcy, data, blockNumber, tr, 0, extract(log->data, 2), logId + "data"))
            return false;
    }

    // If we're not filtering, or the filter passes, proceed. Note the filter depends on the
    // transaction only, not on any address.
    if (!filt || !filt(this, data)) {  // may look at DDos range and nTraces for example
        CAppearance item(blockNumber, tr, NOPOS, "", "");
        getTracesAndVisit(hash, item, funcy, data);
    }
    return true;
}

//---------------------------------------------------------------------------
bool CTransaction::forEveryUniqueAppearanceInTx(APPEARANCEFUNC func, TRANSFUNC traceFilter, void* data) {
    if (!func)
        return false;
    CUniqueState state(func, data, false);
    return forEveryAppearanceInTx(accumulateAddresses, traceFilter, &state);
}

//---------------------------------------------------------------------------
bool CTransaction::forEveryUniqueAppearanceInTxPerTx(APPEARANCEFUNC func, TRANSFUNC traceFilter, void* data) {
    if (!func)
        return false;
    CUniqueState state(func, data, true);
    return forEveryAppearanceInTx(accumulateAddresses, traceFilter, &state);
}

//---------------------------------------------------------------------------
blknum_t path_2_Bn(const string_q& path, blknum_t& endOut, timestamp_t& ts) {
    // comes in of the form first_blk-last_blk.X or blknum.X
    CStringArray parts;
    explode(parts, path, '/');
    ts = 0;
    string_q e;
    if (contains(path, "_ts")) {
        e = nextTokenClear(parts[parts.size() - 1], '_');
        string_q t = substitute(substitute(parts[parts.size() - 1], "ts", ""), ".txt", "");
        ts = str_2_Ts(t);
    } else {
        e = nextTokenClear(parts[parts.size() - 1], '.');
    }
    string_q b = nextTokenClear(e, '-');
    endOut = (e.empty() || !isNumeral(e) ? NOPOS : str_2_Uint(e));
    return (b.empty() || !isNumeral(b) ? NOPOS : str_2_Uint(b));
}

//-------------------------------------------------------------------------
bool CBlock::forEveryTransaction(TRANSVISITFUNC func, void* data) const {
    if (!func)
        return false;

    for (auto trans : transactions) {
        if (!(*func)(trans, data))
            return false;
    }

    return true;
}

//-------------------------------------------------------------------------
bool CBlock::forEveryLog(LOGVISITFUNC func, void* data) const {
    if (!func)
        return false;

    for (auto trans : transactions) {
        if (!trans.forEveryLog(func, data))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool CBlock::forEveryTrace(TRACEVISITFUNC func, void* data) const {
    if (!func)
        return false;

    for (auto trans : transactions) {
        if (trans.timestamp == 0) {
            trans.timestamp = timestamp;
        }
        if (!trans.forEveryTrace(func, data))
            return false;
    }

    return true;
}
// EXISTING_CODE
}  // namespace qblocks
