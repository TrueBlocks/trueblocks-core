/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "block.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlock, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextBlockChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextBlockChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBlock::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBlockChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBlockChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CBlock *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBlock::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "number") {
        *(string_q*)&fieldName = "blockNumber";  // NOLINT

    } else if (fieldName % "author") {
        *(string_q*)&fieldName = "miner";  // NOLINT

    } else if (isTestMode() && fieldName % "blockHash") {
        *(string_q*)&fieldName = "hash";  // NOLINT

    } else if (fieldName % "transactions") {
        // Transactions come to us either as a JSON objects or lists of hashes (i.e. a string array). JSON objects have
        // (among other things) a 'from' field
        if (!contains(fieldValue, "from")) {
            string_q str = fieldValue;
            while (!str.empty()) {
                CTransaction trans;
                trans.hash = toAddress(nextTokenClear(str, ','));
                transactions.push_back(trans);
            }
            return true;
        }
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "difficulty" ) { difficulty = toUnsigned(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "finalized" ) { finalized = str2Bool(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) { gasLimit = toGas(fieldValue); return true; }
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'm':
            if ( fieldName % "miner" ) { miner = toAddress(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = toHash(fieldValue); return true; }
            if ( fieldName % "price" ) { price = str2Double(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            if ( fieldName % "transactions" ) {
                char *p = (char *)fieldValue.c_str();  // NOLINT
                while (p && *p) {
                    CTransaction item;
                    size_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        transactions.push_back(item);
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
    for (size_t i = 0 ; i < transactions.size() ; i++) {
        CTransaction *trans = &transactions.at(i);  // taking a non-const reference
        trans->pBlock = this;
        if (blockNumber >= byzantiumBlock && trans->receipt.status == NO_STATUS) {
            // If we have NO_STATUS in a receipt after the byzantium block, we have to pick it up.
            CReceipt rec;
            getReceipt(rec, trans->hash);
            trans->receipt.status = rec.status;
        }
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBlock::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
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
    archive >> price;
    archive >> finalized;
    archive >> timestamp;
    archive >> transactions;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBlock::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
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
    archive << price;
    archive << finalized;
    archive << timestamp;
    archive << transactions;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CBlockArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CBlockArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "parentHash", T_HASH, ++fieldNum);
    ADD_FIELD(CBlock, "miner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CBlock, "difficulty", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "price", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CBlock, "finalized", T_BOOL, ++fieldNum);
    ADD_FIELD(CBlock, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CBlock, "transactions", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlock, "schema");
    HIDE_FIELD(CBlock, "deleted");
    HIDE_FIELD(CBlock, "showing");

    // EXISTING_CODE
    ADD_FIELD(CBlock, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CBlock, "date");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBlockChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CBlock *blo = (const CBlock *)dataPtr;  // NOLINT
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date") {
                    timestamp_t ts = (timestamp_t)blo->timestamp;
                    return dateFromTimeStamp(ts).Format(FMT_JSON);
                }
                break;
            case 'n':
                if ( fieldIn % "number" ) return asStringU(blo->blockNumber);
                break;
            case 't':
                if ( expContext().hashesOnly && fieldIn % "transactions" ) {
                    size_t cnt = blo->transactions.size();
                    if (!cnt) return "";
                    string_q ret;
                    for (size_t i = 0 ; i < cnt ; i++) {
                        ret += blo->transactions[i].hash;
                        ret += ((i < cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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

//---------------------------------------------------------------------------
bool CBlock::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    SFBloom removed;
    if (m_schema <= getVersionNum(0, 3, 0)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> removed;  // used to be logsBloom
        archive >> blockNumber;
        archive >> parentHash;
        archive >> timestamp;
        archive >> transactions;
        // TODO(tjayrush) -- technically we should re-read these values from the node
        string_q save = getCurlContext()->provider;
        getCurlContext()->provider = "local";
        CBlock upgrade;
        size_t unused;
        queryBlock(upgrade, asStringU(blockNumber), false, false, unused);
        getCurlContext()->provider = save;
        miner = upgrade.miner;
        difficulty = upgrade.difficulty;
        price = 0.0;
        finalized = false;
        finishParse();
        done = true;
    } else if (m_schema <= getVersionNum(0, 4, 0)) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> blockNumber;
        archive >> parentHash;
        archive >> miner;
        archive >> difficulty;
        archive >> price;
        archive >> timestamp;
        archive >> transactions;
        finalized = false;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CBlock& blo) {
    blo.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CBlock& blo) {
    blo.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CBlock::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextBlockChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'd':
            if ( fieldName % "difficulty" ) return asStringU(difficulty);
            break;
        case 'f':
            if ( fieldName % "finalized" ) return asString(finalized);
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) return fromGas(gasLimit);
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'm':
            if ( fieldName % "miner" ) return fromAddress(miner);
            break;
        case 'p':
            if ( fieldName % "parentHash" ) return fromHash(parentHash);
            if ( fieldName % "price" ) return double2Str(price);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            if ( fieldName % "transactions" || fieldName % "transactionsCnt" ) {
                size_t cnt = transactions.size();
                if (endsWith(fieldName, "Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += transactions[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    if ( isTestMode() && fieldName % "blockHash" )
        return fromHash(hash);
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBlock& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CBlock::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "transactions" && index < transactions.size() )
        return &transactions[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAddressItem& item) {
    os << item.bn << "\t";
    os << (item.tx == NOPOS ? "" : asStringU(item.tx)) << "\t";
    os << (item.tc < 10 ? "" : asStringU(item.tc - 10)) << "\t";
    os << item.addr << "\t";
    os << item.reason;
    return os;
}

//---------------------------------------------------------------------------
uint64_t insertUnique(CAddressItemMap *addrMap, const CAddressItem& _value) {
    CAddressItemMap::iterator it = addrMap->find(_value);
    if (it == addrMap->end())  // not found
        it = addrMap->insert(make_pair(_value, true)).first;
    return it->second;
}

//---------------------------------------------------------------------------
bool accumulateAddresses(const CAddressItem& item, void *data) {
    if (zeroAddr(item.addr))
        return true;
    CAddressItem search(item.bn, item.tx, item.tc, item.addr, item.reason);
    insertUnique((CAddressItemMap*)data, search);  // NOLINT
    return true;
}

//---------------------------------------------------------------------------
bool CBlock::forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC filterFunc, void *data) {
    if (!func)
        return false;

    CAddressItemMap addrMap;
    forEveryAddress(accumulateAddresses, filterFunc, &addrMap);
    for (CAddressItemMap::iterator it = addrMap.begin(); it != addrMap.end(); ++it)
        (*func)(it->first, data);

    return true;
}

//---------------------------------------------------------------------------
bool isPotentialAddr(SFUintBN test, SFAddress& addrOut) {

    addrOut = "";

    static const SFUintBN small = hex2BN(  "0x00000000000000ffffffffffffffffffffffffff");  // smallest address we find
    static const SFUintBN large = hex2BN("0x010000000000000000000000000000000000000000");  // largest address we find
    if (test <= small || test >= large)
        return false;

    addrOut = to_hex(test).c_str();
    // Totally a heuristic that can't really be supported, but a good probability that this isn't an address
    if (endsWith(addrOut, "00000000"))
        return false;

    if (addrOut.length() < 40)
        addrOut = padLeft(addrOut, 40, '0');
    addrOut = extract(addrOut, addrOut.length() - 40, 40);
    addrOut = toLower("0x" + addrOut);

    return true;
}

//---------------------------------------------------------------------------
void potentialAddr(ADDRESSFUNC func, void *data, const CAddressItem& item, const string_q& potList) {

    if (!func)
        return;

    // Pull out 32-byte chunks and check to see if they are addresses
    SFAddress addr;
    for (size_t s = 0 ; s < potList.length() / 64 ; s++) {
        SFUintBN test = hex2BN("0x" + extract(potList, s*64, 64));
        if (isPotentialAddr(test, addr)) {
            CAddressItem it(item);
            it.addr = addr;
            (*func)(it, data);
        }
    }
}

//---------------------------------------------------------------------------
void foundOne(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, const string_q& reason) {
    CAddressItem item(bn, tx, tc, addr, reason);
    (*func)(item, data);
}

//---------------------------------------------------------------------------
void foundPot(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList, const string_q& reason) {
    CAddressItem item(bn, tx, tc, "", reason);
    potentialAddr(func, data, item, potList);
}

//---------------------------------------------------------------------------
bool CBlock::forEveryAddress(ADDRESSFUNC func, TRANSFUNC filterFunc, void *data) {

    if (!func)
        return false;

    foundOne(func, data, blockNumber, NOPOS, 0, miner, "miner");
    for (size_t tr = 0 ; tr < transactions.size() ; tr++) {
        const CTransaction *trans   = &transactions[tr];
        const CReceipt     *receipt = &trans->receipt;
        foundOne(func, data, blockNumber, tr, 0, trans->from, "from");
        foundOne(func, data, blockNumber, tr, 0, trans->to, "to");
        foundOne(func, data, blockNumber, tr, 0, receipt->contractAddress, "creation");
        foundPot(func, data, blockNumber, tr, 0, extract(trans->input, 10), "input");
        for (size_t l = 0 ; l < receipt->logs.size() ; l++) {
            const CLogEntry *log = &receipt->logs[l];
            string_q logId = "log" + asStringU(l) + "_";
            foundOne(func, data, blockNumber, tr, 0, log->address, logId + "generator");
            for (size_t t = 0 ; t < log->topics.size() ; t++) {
                SFAddress addr;
                string_q topId = asStringU(t);
                if (isPotentialAddr(log->topics[t], addr)) {
                    foundOne(func, data, blockNumber, tr, 0, addr, logId + "topic_" + topId);
                }
            }
            foundPot(func, data, blockNumber, tr, 0, extract(log->data, 2), logId + "data");
        }

        // If we're not filtering, or the filter passes, proceed. Note the filter depends on the
        // transaction only, not on any address.
        if (!filterFunc || !filterFunc(trans, data)) {  // may look at DDos range and nTraces for example
            CTraceArray traces;
            getTraces(traces, trans->hash);
            for (size_t t = 0 ; t < traces.size() ; t++) {
                const CTrace *trace = &traces[t];  // taking a non-const reference
                string_q trID = "trace" + asStringU(t) + "_";
                foundOne(func, data, blockNumber, tr, t+10, trace->action.from, trID + "from");
                foundOne(func, data, blockNumber, tr, t+10, trace->action.to, trID + "to");
                foundOne(func, data, blockNumber, tr, t+10, trace->action.refundAddress, trID + "refundAddr");
                foundOne(func, data, blockNumber, tr, t+10, trace->action.address, trID + "creation");
                foundOne(func, data, blockNumber, tr, t+10, trace->result.address, trID + "self-destruct");
                string_q input = extract(trace->action.input, 10);
                if (!input.empty())
                    foundPot(func, data, blockNumber, tr, t+10, input, trID + "input");
            }
        }
    }
    return true;
}
// EXISTING_CODE
}  // namespace qblocks

