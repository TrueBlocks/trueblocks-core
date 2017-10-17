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
#include "accountwatch.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountWatch, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextAccountwatchChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAccountwatchChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccountWatch::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAccountwatchChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CAccountWatch *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "qbis") {
        char *p = (char *)fieldValue.c_str();
        uint32_t nFields = 0;
        qbis.parseJson(p, nFields);
        return true;
    }
    if (fieldName % "balance") {
        qbis.endBal = qbis.begBal = toWei(fieldValue);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = toAddress(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "color" ) { color = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "deepScan" ) { deepScan = toBool(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) { firstBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "index" ) { index = toUnsigned(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) { lastBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "nodeBal" ) { nodeBal = toWei(fieldValue); return true; }
            break;
        case 'q':
            if ( fieldName % "qbis" ) { /* qbis = fieldValue; */ return false; }
            break;
        case 's':
            if ( fieldName % "status" ) { status = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountWatch::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CAccountWatch*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> index;
    archive >> address;
    archive >> name;
    archive >> color;
    archive >> firstBlock;
    archive >> lastBlock;
    archive >> status;
    archive >> deepScan;
    archive >> qbis;
    archive >> nodeBal;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
    archive << index;
    archive << address;
    archive << name;
    archive << color;
    archive << firstBlock;
    archive << lastBlock;
    archive << status;
    archive << deepScan;
    archive << qbis;
    archive << nodeBal;

    return true;
}

//---------------------------------------------------------------------------
void CAccountWatch::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAccountWatch, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "index", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAccountWatch, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "color", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "firstBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "lastBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "status", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deepScan", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "qbis", T_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "nodeBal", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountWatch, "schema");
    HIDE_FIELD(CAccountWatch, "deleted");
    HIDE_FIELD(CAccountWatch, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CAccountWatch *acc = (const CAccountWatch *)dataPtr;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAccountWatch::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccountWatch::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAccountWatch::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAccountwatchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return fromAddress(address);
            break;
        case 'c':
            if ( fieldName % "color" ) return color;
            break;
        case 'd':
            if ( fieldName % "deepScan" ) return asString(deepScan);
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) return asStringU(firstBlock);
            break;
        case 'i':
            if ( fieldName % "index" ) return asStringU(index);
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) return asStringU(lastBlock);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            if ( fieldName % "nodeBal" ) return fromWei(nodeBal);
            break;
        case 'q':
            if ( fieldName % "qbis" ) { expContext().noFrst=true; return qbis.Format(); }
            break;
        case 's':
            if ( fieldName % "status" ) return status;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    SFString s;
    s = toUpper(SFString("qbis")) + "::";
    if (fieldName.Contains(s)) {
        SFString f = fieldName;
        f.ReplaceAll(s,"");
        f = qbis.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountWatch& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAccountWatch::getObjectAt(const SFString& name, uint32_t i) const {
    if ( name % "qbis" )
        return &qbis;
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CAccountWatch::getWatch(const CToml& toml, uint32_t n) {
    index = n;
    address = fixAddress(toLower(toml.getConfigStr("watches", "address_"+asString(n), "")));
    if (!isAddress(address))
        return false;
    name = toml.getConfigStr("watches", "name_"+asString(n), "");
    if (name.empty())
        return false;
    color = convertColor(toml.getConfigStr("watches", "color_"+asString(n), ""));
    if (cBlack != "" && color.empty())
        return false;
    firstBlock = toml.getConfigInt("watches", "firstBlock_"+asString(n), 0);
    lastBlock = toml.getConfigInt("watches", "lastBlock_"+asString(n), UINT_MAX);
    status = toml.getConfigStr("watches", "status_"+asString(n), "");
    qbis.begBal = toml.getConfigBigInt("watches", "start_bal_"+asString(n), 0);
    qbis.endBal = qbis.begBal;
    nodeBal = getBalance(address, firstBlock-1, false);
    return true;
}

//---------------------------------------------------------------------------
SFString CAccountWatch::displayName(bool terse, uint32_t w1, uint32_t w2) const {
    if (address == "others") {
        return padRight(name, w1 + w2 + 1);
    }

    if (terse) {
        uint64_t len = name.length();
        uint64_t need = 42 - len - 6; // " (" and "...)"
        return color + name.Left(42-6) + " (" + address.Left(need) + "...)" + cOff;
    }

    return padRight(name.Left(w1),w1) + " " + address.Left(w2) + " ";
}

//---------------------------------------------------------------------------
bool CAccountWatch::isTransactionOfInterest(CTransaction *trans, uint64_t nSigs, SFString sigs[]) const {
    // Assume it's not an internal transaction
    trans->isInternalTx = false;

    // First, check to see if the transaction is directly 'to' or 'from'
    if (trans->to.ContainsI(address) || trans->from.ContainsI(address))
        return true;

    // If this is a contract and this is its birth block, that's a hit
    if (trans->receipt.contractAddress == address) {
        trans->isInternalTx = true;  // TODO(tjayrush) - handle contract creation correctly (change to data)
        return true;
    }

    // Next, we check the receipt logs to see if the address appears either in
    // the log's 'address' field or in one of the data items
    //
    // TODO(tjayrush): We should do a 'deep trace' here (or when the block is first read)
    // to see if there was a 'call,' to our address.
    for (uint32_t i = 0 ; i < trans->receipt.logs.getCount() ; i++) {
        SFString acc = address;
        CLogEntry *l = reinterpret_cast<CLogEntry *>(&trans->receipt.logs[i]);
        if (l->address.Contains(acc)) {
            // If we find a receipt log with an 'address' of interest, this is an
            // internal transaction that caused our contract to emit an event.
            trans->isInternalTx = true;
            return true;

        } else {
            // Next, left pad the address with '0' to a width of 32 bytes. If
            // it matches either an indexed topic or one of the 32-byte aligned
            // data items, we have found a potential match. We cannot be sure this
            // is a hit, but it most likely is. This may be a false positive.
            acc = padLeft(acc.Substitute("0x",""), 64).Substitute(' ', '0');
            if (l->data.ContainsI(acc)) {
                // Do this first to avoid spinning through event sigs if we
                // don't have to.
                trans->isInternalTx = true;
                return true;

            } else {
                // If the topic[0] is an event of interest...
                for (uint64_t q = 0 ; q < nSigs ; q++) {
                    SFHash tHash = fromTopic(l->topics[0]);
                    if (tHash % sigs[q]) {
                        trans->isInternalTx = true;
                        return true;
                    }
                }

                // ...or the address is in the indexed topics or data
                for (uint32_t j = 1 ; j < l->topics.getCount() ; j++) {
                    SFHash tHash = fromTopic(l->topics[j]);
                    if (tHash % acc) {
                        trans->isInternalTx = true;
                        return true;
                    }
                }
            }
        }
    }

    if (deepScan) {
        CTraceArray traces;
        getTraces(traces, trans->hash);
        for (uint32_t i = 0 ; i < traces.getCount() ; i++) {
            CTraceAction *action = (CTraceAction*)&(traces[i].action);
            if (action->to % address || action->from % address || action->address % address || action->refundAddress % address) {
                trans->isInternalTx = true;
                return true;
            }
        }
    }
    return false;
}
// EXISTING_CODE
}  // namespace qblocks

