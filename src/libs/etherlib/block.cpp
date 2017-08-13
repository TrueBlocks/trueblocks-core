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
IMPLEMENT_NODE(CBlock, CBaseNode, curVersion);

//---------------------------------------------------------------------------
extern SFString nextBlockChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextBlockChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CBlock::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextBlockChunk, this);
}

//---------------------------------------------------------------------------
SFString nextBlockChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CBlock *blo = (const CBlock *)data;
    if (blo) {
        // Give customized code a chance to override first
        SFString ret = nextBlockChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'b':
                if ( fieldIn % "blockNumber" ) return asStringU(blo->blockNumber);
                break;
            case 'g':
                if ( fieldIn % "gasLimit" ) return asStringU(blo->gasLimit);
                if ( fieldIn % "gasUsed" ) return asStringU(blo->gasUsed);
                break;
            case 'h':
                if ( fieldIn % "hash" ) return fromHash(blo->hash);
                break;
            case 'l':
                if ( fieldIn % "logsBloom" ) return fromBloom(blo->logsBloom);
                break;
            case 'p':
                if ( fieldIn % "parentHash" ) return fromHash(blo->parentHash);
                break;
            case 't':
                if ( fieldIn % "timestamp" ) return asStringU(blo->timestamp);
                if ( fieldIn % "transactions" ) {
                    uint32_t cnt = blo->transactions.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += blo->transactions[i].Format();
                        ret += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, blo);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBlock::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
        case 'p':
            if ( fieldName % "parentHash" ) { parentHash = toHash(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toUnsigned(fieldValue); return true; }
            if ( fieldName % "transactions" ) return true;
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
bool CBlock::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CBlock*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

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

    return true;
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CBlock, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CBlock, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CBlock, "gasLimit", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "gasUsed", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "hash", T_TEXT, ++fieldNum);
    ADD_FIELD(CBlock, "logsBloom", T_BLOOM, ++fieldNum);
    ADD_FIELD(CBlock, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "parentHash", T_TEXT, ++fieldNum);
    ADD_FIELD(CBlock, "timestamp", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlock, "transactions", T_TEXT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlock, "schema");
    HIDE_FIELD(CBlock, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextBlockChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CBlock *blo = (const CBlock *)data;
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
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
                    return nextBasenodeChunk(fieldIn, force, blo);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBlock::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CBlock::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
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
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

