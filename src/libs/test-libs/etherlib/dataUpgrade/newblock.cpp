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
#include "newblock.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewBlock, CBaseNode, curVersion);

//---------------------------------------------------------------------------
extern SFString nextNewblockChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextNewblockChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CNewBlock::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextNewblockChunk, this);
}

//---------------------------------------------------------------------------
SFString nextNewblockChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CNewBlock *newp = (const CNewBlock *)data;
    if (newp) {
        // Give customized code a chance to override first
        SFString ret = nextNewblockChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'b':
                if ( fieldIn % "blockNumber" ) return asStringU(newp->blockNumber);
                break;
            case 'g':
                if ( fieldIn % "gasLimit" ) return asStringU(newp->gasLimit);
                if ( fieldIn % "gasUsed" ) return asStringU(newp->gasUsed);
                break;
            case 'h':
                if ( fieldIn % "hash" ) return fromHash(newp->hash);
                break;
            case 'l':
                if ( fieldIn % "logsBloom" ) return fromBloom(newp->logsBloom);
                break;
            case 'm':
                if ( fieldIn % "miner" ) return fromAddress(newp->miner);
                break;
            case 'p':
                if ( fieldIn % "parentHash" ) return fromHash(newp->parentHash);
                break;
            case 's':
                if ( fieldIn % "size" ) return asStringU(newp->size);
                break;
            case 't':
                if ( fieldIn % "timestamp" ) return asString(newp->timestamp);
                if ( fieldIn % "transactions" ) {
                    uint32_t cnt = newp->transactions.getCount();
                    if (!cnt) return "";
                    ret = "";
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += newp->transactions[i].Format();
                        ret += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, newp);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewBlock::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "number") {
        *(SFString*)&fieldName = "blockNumber";

    } else if (fieldName % "transactions") {
        // Transactions can come to us either as a JSON object (starts with '{') or a list
        // of hashes (i.e. a string array).
        if (fieldValue.Contains("{")) {
            char *p = (char *)fieldValue.c_str();
            while (p && *p) {
                CTransaction trans;
                uint32_t nFields = 0;
                p = trans.parseJson(p, nFields);
                if (nFields)
                    transactions[transactions.getCount()] = trans;
            }

        } else {
            SFString str = fieldValue;
            while (!str.empty()) {
                CTransaction trans;
                trans.hash = toAddress(nextTokenClear(str, ','));
                transactions[transactions.getCount()] = trans;
            }
        }
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasLimit" ) { gasLimit = toUnsigned(fieldValue); return true; }
            if ( fieldName % "gasUsed" ) { gasUsed = toUnsigned(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "logsBloom" ) { logsBloom = toBloom(fieldValue); return true; }
            break;
        case 'm':
            if ( fieldName % "miner" ) { miner = toAddress(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = toHash(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "size" ) { size = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toLong(fieldValue); return true; }
            if ( fieldName % "transactions" ) return true;
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewBlock::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

#define MAJOR 0
#define MINOR 2
#define BUILD 0
#define CURRENT_SCHEMA ((MAJOR * 1000000) + (MINOR * 1000) + (BUILD))
//---------------------------------------------------------------------------------------------------
bool CNewBlock::Serialize(SFArchive& archive) {
    m_schema = CURRENT_SCHEMA;
    if (!archive.isReading())
        return ((const CNewBlock*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    if (m_schema < CURRENT_SCHEMA)
        return readBackLevel(archive);

    archive >> gasLimit;
    archive >> gasUsed;
    archive >> hash;
    archive >> logsBloom;
    archive >> blockNumber;
    archive >> parentHash;
    archive >> timestamp;
    archive >> transactions;
    archive >> miner;
    archive >> size;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewBlock::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << gasLimit;
    archive << gasUsed;
    archive << hash;
    archive << logsBloom;
    archive << blockNumber;
    archive << parentHash;
    archive << timestamp;
    archive << transactions;
    archive << miner;
    archive << size;

    return true;
}

//---------------------------------------------------------------------------
void CNewBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CNewBlock, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewBlock, "gasLimit", T_GAS, ++fieldNum);
    ADD_FIELD(CNewBlock, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CNewBlock, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CNewBlock, "logsBloom", T_BLOOM, ++fieldNum);
    ADD_FIELD(CNewBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "parentHash", T_HASH, ++fieldNum);
    ADD_FIELD(CNewBlock, "timestamp", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewBlock, "transactions", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CNewBlock, "miner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewBlock, "size", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewBlock, "schema");
    HIDE_FIELD(CNewBlock, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextNewblockChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CNewBlock *newp = (const CNewBlock *)data;
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if ( fieldIn % "number" ) return asStringU(newp->blockNumber);
                break;
            case 't':
                if ( expContext().hashesOnly && fieldIn % "transactions" ) {
                    uint32_t cnt = newp->transactions.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
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
                    return nextBasenodeChunk(fieldIn, force, newp);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CNewBlock::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CNewBlock::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema == 501) {
        archive >> gasLimit;
        archive >> gasUsed;
        archive >> hash;
        archive >> logsBloom;
        archive >> blockNumber;
        archive >> parentHash;
        archive >> timestamp;
        archive >> transactions;
        miner = "NOTSET";
        size = 0xdeadbeef;
        finishParse();
        return true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CNewBlock& newp) {
    newp.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CNewBlock& newp) {
    newp.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CNewBlock::CNewBlock(const CBlock& block) {
    gasLimit = block.gasLimit;
    gasUsed = block.gasUsed;
    hash = block.hash;
    logsBloom = block.logsBloom;
    blockNumber = block.blockNumber;
    parentHash = block.parentHash;
    timestamp = block.timestamp;
    transactions = block.transactions;
    // miner;
    // size;
}

//-----------------------------------------------------------------------
bool readOneNewBlock_fromBinary(CNewBlock& block, const SFString& fileName) {
    block = CNewBlock(); // reset
    SFArchive archive(true, curVersion, true);
    if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT))
    {
        block.Serialize(archive);
        archive.Close();
        return block.blockNumber;
    }
    return false;
}

//-----------------------------------------------------------------------
bool readOneNewBlock_fromJson(CNewBlock& block, const SFString& fileName) {
    block = CNewBlock(); // reset
    SFString contents = asciiFileToString(fileName);
    if (contents.Contains("null")) {
        contents.ReplaceAll("null", "\"0x\"");
        stringToAsciiFile(fileName, contents);
    }

    if (!contents.endsWith('\n')) {
        stringToAsciiFile(fileName, contents+"\n");
    }

    char *p = cleanUpJson((char *)(const char*)contents);
    uint32_t nFields=0;
    block.parseJson(p,nFields);
    return nFields;
}
// EXISTING_CODE
}  // namespace qblocks

