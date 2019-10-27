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
#include "newblock.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewBlock, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextNewblockChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextNewblockChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNewBlock::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["newblock_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextNewblockChunk, this);
}

//---------------------------------------------------------------------------
string_q nextNewblockChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CNewBlock *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewBlock::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "number") {
        *(string_q*)&fieldName = "blockNumber";  // NOLINT

    } else if (fieldName % "transactions") {
        // Transactions come to us either as a JSON objects or lists
        // of hashes (i.e. a string array). JSON objects have 'from'
        // We handle those as normal below
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
            if ( fieldName % "blockNumber" ) { blockNumber = str_2_Uint(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "difficulty" ) { difficulty = str_2_Uint(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "finalized" ) { finalized = str_2_Bool(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) { gasLimit = str_2_Gas(fieldValue); return true; }
            if ( fieldName % "gasUsed" ) { gasUsed = str_2_Gas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = str_2_Hash(fieldValue); return true; }
            break;
        case 'm':
            if ( fieldName % "miner" ) { miner = str_2_Addr(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = str_2_Hash(fieldValue); return true; }
            if ( fieldName % "price" ) { price = str_2_Double(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = str_2_Ts(fieldValue); return true; }
            if ( fieldName % "transactions" ) {
                CTransaction item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    transactions.push_back(item);
                    item = CTransaction();  // reset
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
void CNewBlock::finishParse() {
    // EXISTING_CODE
    for (size_t i = 0 ; i < transactions.size() ; i++)
        transactions.at(i).pBlock = reinterpret_cast<CBlock *>(this);  // .at cannot access past the end of vector
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewBlock::Serialize(CArchive& archive) {

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
    archive >> price;
    archive >> finalized;
    archive >> timestamp;
    archive >> transactions;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewBlock::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    ((CNewBlock*)this)->m_schema = getVersionNum();  // NOLINT
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
CArchive& operator>>(CArchive& archive, CNewBlockArray& array) {
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
CArchive& operator<<(CArchive& archive, const CNewBlockArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CNewBlock::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CNewBlock, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CNewBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewBlock, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewBlock, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CNewBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CNewBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CNewBlock, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CNewBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "parentHash", T_HASH, ++fieldNum);
    ADD_FIELD(CNewBlock, "miner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewBlock, "difficulty", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "price", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CNewBlock, "finalized", T_BOOL, ++fieldNum);
    ADD_FIELD(CNewBlock, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CNewBlock, "transactions", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewBlock, "schema");
    HIDE_FIELD(CNewBlock, "deleted");
    HIDE_FIELD(CNewBlock, "showing");
    HIDE_FIELD(CNewBlock, "cname");

    builtIns.push_back(_biCNewBlock);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextNewblockChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CNewBlock *newp = reinterpret_cast<const CNewBlock *>(dataPtr);
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if ( fieldIn % "number" ) return uint_2_Str(newp->blockNumber);
                break;
            case 't':
                if ( expContext().hashesOnly && fieldIn % "transactions" ) {
                    size_t cnt = newp->transactions.size();
                    if (!cnt) return "";
                    string_q ret;
                    for (size_t i = 0 ; i < cnt ; i++) {
                        ret += newp->transactions[i].hash;
                        ret += ((i < cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, newp);
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
bool CNewBlock::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    bloom_t removed;
    if (m_schema < 502) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> removed;  // used to be logsBloom
        archive >> blockNumber;
        archive >> parentHash;
        archive >> timestamp;
        archive >> transactions;
        // NOTE: See the real code in CBlock file for note about upgrading
        CBlock upgrade;
        string_q prev = setDataSource("local");
        queryBlock(upgrade, uint_2_Str(blockNumber), false);
        setDataSource(prev);
        miner = upgrade.miner;
        difficulty = upgrade.difficulty;
        price = 0.0;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CNewBlock& newp) {
    newp.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CNewBlock& newp) {
    newp.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CNewBlock::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextNewblockChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) return uint_2_Str(blockNumber);
            break;
        case 'd':
            if ( fieldName % "difficulty" ) return uint_2_Str(difficulty);
            break;
        case 'f':
            if ( fieldName % "finalized" ) return bool_2_Str(finalized);
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) return gas_2_Str(gasLimit);
            if ( fieldName % "gasUsed" ) return gas_2_Str(gasUsed);
            break;
        case 'h':
            if ( fieldName % "hash" ) return hash_2_Str(hash);
            break;
        case 'm':
            if ( fieldName % "miner" ) return addr_2_Str(miner);
            break;
        case 'p':
            if ( fieldName % "parentHash" ) return hash_2_Str(parentHash);
            if ( fieldName % "price" ) return double_2_Str(price);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return ts_2_Str(timestamp);
            if ( fieldName % "transactions" || fieldName % "transactionsCnt" ) {
                size_t cnt = transactions.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
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
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewBlock& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CNewBlock::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "transactions" && index < transactions.size() )
        return &transactions[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_NEWBLOCK = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
CNewBlock::CNewBlock(const CBlock& block) {
    gasLimit = block.gasLimit;
    gasUsed = block.gasUsed;
    hash = block.hash;
//    logsBloom = block.logsBloom;
    blockNumber = block.blockNumber;
    parentHash = block.parentHash;
    timestamp = block.timestamp;
    transactions = block.transactions;
    miner = "0x0";
    difficulty = 0;
}

//-----------------------------------------------------------------------
bool readOneNewBlock_fromBinary(CNewBlock& block, const string_q& fileName) {
    block = CNewBlock();  // reset
    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        block.Serialize(archive);
        archive.Close();
        return block.blockNumber;
    }
    return false;
}

//-----------------------------------------------------------------------
bool readOneNewBlock_fromJson(CNewBlock& block, const string_q& fileName) {
    block = CNewBlock();  // reset
    string_q contents;
    asciiFileToString(fileName, contents);
    if (contains(contents, "null")) {
        replaceAll(contents, "null", "\"0x\"");
        stringToAsciiFile(fileName, contents);
    }
    if (!endsWith(contents, '\n'))
        stringToAsciiFile(fileName, contents + "\n");

    return block.parseJson3(contents);
}
// EXISTING_CODE

