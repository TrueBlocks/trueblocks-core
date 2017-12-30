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
#include "block.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlock, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextBlockChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextBlockChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBlock::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextBlockChunk, this);
}

//---------------------------------------------------------------------------
SFString nextBlockChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CBlock *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBlock::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "number") {
        *(SFString*)&fieldName = "blockNumber";

    } else if (isTestMode() && fieldName % "blockHash") {
        *(SFString*)&fieldName = "hash";

    } else if (fieldName % "transactions") {
        // Transactions come to us either as a JSON objects or lists
        // of hashes (i.e. a string array). JSON objects have 'from'
        
        if (!fieldValue.Contains("from")) {
            SFString str = fieldValue;
            while (!str.empty()) {
                CTransaction trans;
                trans.hash = toAddress(nextTokenClear(str, ','));
                transactions[transactions.getCount()] = trans;
            }
            return true;
        }
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) { gasLimit = toGas(fieldValue); return true; }
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "logsBloom" ) { logsBloom = toBloom(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = toHash(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            if ( fieldName % "transactions" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CTransaction item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        transactions[transactions.getCount()] = item;
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
    for (uint32_t i=0;i<transactions.getCount();i++) {
        CTransaction *trans = &transactions[i];
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
        return ((const CBlock*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> gasLimit;
    archive >> gasUsed;
    archive >> hash;
    archive >> logsBloom;
    archive >> blockNumber;
    archive >> parentHash;
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
    archive << logsBloom;
    archive << blockNumber;
    archive << parentHash;
    archive << timestamp;
    archive << transactions;

    return true;
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CBlock, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CBlock, "logsBloom", T_BLOOM, ++fieldNum);
    ADD_FIELD(CBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "parentHash", T_HASH, ++fieldNum);
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
SFString nextBlockChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CBlock *blo = (const CBlock *)dataPtr;
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "date")
                {
                    timestamp_t ts = (timestamp_t)blo->timestamp;
                    return dateFromTimeStamp(ts).Format(FMT_JSON);
                }
                break;
            case 'n':
                if ( fieldIn % "number" ) return asStringU(blo->blockNumber);
                break;
            case 't':
                if ( expContext().hashesOnly && fieldIn % "transactions" ) {
                    uint32_t cnt = blo->transactions.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
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
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBlock::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CBlock::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
#ifdef UPGRADING
    if (m_schema < 514) {
        CBlock_513 old;
        archive >> old.gasLimit;
        archive >> old.gasUsed;
        archive >> old.hash;
        archive >> old.logsBloom;
        archive >> old.blockNumber;
        archive >> old.parentHash;
        archive >> old.timestamp;
        archive >> old.transactions;
        old.finishParse();
        *this = old;
        finishParse();
        done = true;
    }
#endif
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
SFString CBlock::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextBlockChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) return fromGas(gasLimit);
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'l':
            if ( fieldName % "logsBloom" ) return fromBloom(logsBloom);
            break;
        case 'p':
            if ( fieldName % "parentHash" ) return fromHash(parentHash);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            if ( fieldName % "transactions" || fieldName % "transactionsCnt" ) {
                uint32_t cnt = transactions.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
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

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CBlock::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "transactions" && index < transactions.getCount() )
        return &transactions[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
#ifdef UPGRADING
CBlock& CBlock::operator=(const CBlock_513& old) {

    // Pick up the missing stuff...
    SFString results;
    queryRawBlock(results, asStringU(blockNumber), false, false);
    CRPCResult generic;
    char *p = cleanUpJson((char*)(const char*)results);
    generic.parseJson(p);
    p = cleanUpJson((char *)generic.result.c_str());
    parseJson(p);

    // The old stuff should be identical, but we assign it anyway
    gasLimit = old.gasLimit;
    gasUsed = old.gasUsed;
    hash = old.hash;
    logsBloom = old.logsBloom;
    blockNumber = old.blockNumber;
    parentHash = old.parentHash;
    timestamp = old.timestamp;
    transactions.Clear();
    for (uint32_t i = 0 ; i < old.transactions.getCount(); i++) {
        transactions[transactions.getCount()] = old.transactions[i];
    }
    return *this;
}
#endif
// EXISTING_CODE
}  // namespace qblocks

