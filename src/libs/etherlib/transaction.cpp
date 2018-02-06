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
#include "transaction.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTransaction, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextTransactionChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTransactionChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTransaction::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTransactionChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTransactionChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTransaction *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName == "to" && fieldValue == "null")
        *((SFString*)&fieldValue) = "0x";

    if ( fieldName % "input" )
    {
        input = fieldValue;
        function = Format("[{FUNCTION}]");
        return true;

    } else if ( fieldName % "value" )
    {
        value = canonicalWei(fieldValue);
        ether = (double)strtold((const char*)Format("[{ETHER}]"),NULL);
        return true;

    } else if ( fieldName % "contractAddress" )
    {
        receipt.contractAddress = toAddress(fieldValue);
        return true;

    } else if ( fieldName % "gasUsed" )
    {
        receipt.gasUsed = toUnsigned(fieldValue);
        return true;
    } else if ( fieldName % "receipt" )
    {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        receipt.parseJson(p,nFields);
        return true;
    }
    if (pBlock)
        if (((CBlock*)pBlock)->setValueByName(fieldName, fieldValue))
            return true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockHash" ) { blockHash = toHash(fieldValue); return true; }
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = toGas(fieldValue); return true; }
            if ( fieldName % "gasPrice" ) { gasPrice = toGas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            if ( fieldName % "isError" ) { isError = toUnsigned(fieldValue); return true; }
            if ( fieldName % "isInternal" ) { isInternal = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = toUnsigned(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "receipt" ) { /* receipt = fieldValue; */ return false; }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) { transactionIndex = toUnsigned(fieldValue); return true; }
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            if ( fieldName % "to" ) { to = toAddress(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toWei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTransaction::finishParse() {
    // EXISTING_CODE
    if (pParent && pParent->isKindOf(GETRUNTIME_CLASS(CAccount)))
        funcPtr = ((CAccount*)pParent)->abi.findFunctionByEncoding(input.substr(2,8));
    function = Format("[{FUNCTION}]");
    ether = (double)strtold((const char*)Format("[{ETHER}]"),NULL);
    receipt.pTrans = this;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CTransaction*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> hash;
    archive >> blockHash;
    archive >> blockNumber;
    archive >> transactionIndex;
    archive >> nonce;
    archive >> timestamp;
    archive >> from;
    archive >> to;
    archive >> value;
    archive >> gas;
    archive >> gasPrice;
    archive >> input;
    archive >> isError;
    archive >> isInternal;
    archive >> receipt;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << hash;
    archive << blockHash;
    archive << blockNumber;
    archive << transactionIndex;
    archive << nonce;
    archive << timestamp;
    archive << from;
    archive << to;
    archive << value;
    archive << gas;
    archive << gasPrice;
    archive << input;
    archive << isError;
    archive << isInternal;
    archive << receipt;

    return true;
}

//---------------------------------------------------------------------------
void CTransaction::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTransaction, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "transactionIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "nonce", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CTransaction, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "value", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "gasPrice", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "isError", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "isInternal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "receipt", T_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransaction, "schema");
    HIDE_FIELD(CTransaction, "deleted");
    HIDE_FIELD(CTransaction, "showing");

    // EXISTING_CODE
    // These are 'phony' fields needed because etherscan.io sends them
    // as part of a transaction from its API; however, these fields
    // don't actually exist in a transaction. We add them (so Format
    // works, but hide them since they are never used.
    ADD_FIELD(CTransaction, "confirmations", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CTransaction, "confirmations");

    // Add custom fields
    ADD_FIELD(CTransaction, "gasCost", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "function", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "date", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "ether", T_ETHER, ++fieldNum);
    ADD_FIELD(CTransaction, "encoding", T_TEXT, ++fieldNum);

    // Hide fields we don't want to show by default
    HIDE_FIELD(CTransaction, "function");
    HIDE_FIELD(CTransaction, "encoding");
    HIDE_FIELD(CTransaction, "gasCost");
    HIDE_FIELD(CTransaction, "isError");
    HIDE_FIELD(CTransaction, "isInternal");
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "ether");
    //    HIDE_FIELD(CTransaction, "receipt");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTransactionChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTransaction *tra = (const CTransaction *)dataPtr;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'c':
                if ( fieldIn % "contractAddress" ) return fromAddress(tra->receipt.contractAddress);
                break;
            case 'd':
                if (fieldIn % "date")
                {
                    timestamp_t ts = (timestamp_t)tra->timestamp;
                    if (tra->pBlock)
                        ts = tra->pBlock->timestamp;
                    return dateFromTimeStamp(ts).Format(FMT_JSON);
                }
                break;
            case 'e':
                if ( fieldIn % "ether" ) return wei2Ether(asStringBN(tra->value));
                if ( fieldIn % "encoding" ) {
                    return tra->input.substr(0,10);
                }
                break;
            case 'f':
                if ( fieldIn % "function" )
                    return tra->inputToFunction();
                break;
            case 'g':
                if ( fieldIn % "gasUsed" ) return asStringU(tra->receipt.gasUsed);
                if ( fieldIn % "gasCost" ) {
                    SFUintBN used = tra->receipt.gasUsed;
                    SFUintBN price = tra->gasPrice;
                    return wei2Ether(to_string(used * price).c_str());
                }
                break;
            case 't':
                if ( fieldIn % "timestamp" && tra->pBlock)
                    return asString(tra->pBlock->timestamp);
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTransaction::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTransaction::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0,4,0)) {
        SFWei removed; // used to be cumulativeGasUsed
        archive >> hash;
        archive >> blockHash;
        archive >> blockNumber;
        archive >> transactionIndex;
        archive >> nonce;
        archive >> timestamp;
        archive >> from;
        archive >> to;
        archive >> value;
        archive >> gas;
        archive >> gasPrice;
        archive >> removed;
        archive >> input;
        archive >> isError;
        archive >> isInternal;
        archive >> receipt;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CTransaction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTransaction& tra) {
    tra.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CTransaction::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTransactionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockHash" ) return fromHash(blockHash);
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'f':
            if ( fieldName % "from" ) return fromAddress(from);
            break;
        case 'g':
            if ( fieldName % "gas" ) return fromGas(gas);
            if ( fieldName % "gasPrice" ) return fromGas(gasPrice);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'i':
            if ( fieldName % "input" ) return input;
            if ( fieldName % "isError" ) return asStringU(isError);
            if ( fieldName % "isInternal" ) return asStringU(isInternal);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return asStringU(nonce);
            break;
        case 'r':
            if ( fieldName % "receipt" ) { expContext().noFrst=true; return receipt.Format(); }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) return asStringU(transactionIndex);
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            if ( fieldName % "to" ) return fromAddress(to);
            break;
        case 'v':
            if ( fieldName % "value" ) return fromWei(value);
            break;
    }

    // EXISTING_CODE
    // See if this field belongs to the item's container
    ret = nextBlockChunk(fieldName, pBlock);
    if (ret.Contains("Field not found"))
        ret = EMPTY;
    if (!ret.empty())
        return ret;
    // EXISTING_CODE

    SFString s;
    s = toUpper(SFString("receipt")) + "::";
    if (fieldName.Contains(s)) {
        SFString f = fieldName;
        f.ReplaceAll(s,"");
        f = receipt.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTransaction& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CTransaction::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "receipt" )
        return &receipt;
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
int sortTransactionsForWrite(const void *rr1, const void *rr2)
{
    CTransaction *tr1 = (CTransaction*)rr1;
    CTransaction *tr2 = (CTransaction*)rr2;

    if (tr1->timestamp > tr2->timestamp)
        return 1;
    else if (tr1->timestamp < tr2->timestamp)
        return -1;
    if (tr1->from.compare(tr2->from))
        return tr1->from.compare(tr2->from);
    if (tr1->transactionIndex > tr2->transactionIndex)
        return 1;
    else if (tr1->transactionIndex < tr2->transactionIndex)
        return -1;
    return tr1->hash.compare(tr2->hash);
}

//---------------------------------------------------------------------------
bool CTransaction::isFunction(const SFString& func) const
{
    if (func=="none")
    {
        SFString ret = inputToFunction();
        if (ret.ContainsAny("acghrstuv"))
            return false;
        return (ret==" ");
    }

    return (funcPtr ? funcPtr->name == func : false);
}

//--------------------------------------------------------------------
inline SFString asStringULL(uint64_t i) {
    ostringstream os;
    os << i;
    return os.str().c_str();
}

//------------------------------------------------------------------------------
#define toBigNum2(a,b)      SFString(to_string(canonicalWei("0x"+grabPart(a,b))).c_str())
#define grabPart(a,b)       StripLeading((a).substr(64*(b),64),'0')
#define grabBigNum(a,b)     strtoull((const char*)grabPart(a,b),NULL,16)
#define toAddr(a,b)         "0x"+padLeft(grabPart(a,b),40,'0')
#define toAddrOld(a,b)      "0x"+grabPart(a,b)
#define toAscString(a,b)    hex2String("0x"+grabPart(a,b))
#define toBigNum(a,b)       asStringULL(grabBigNum(a,b))
#define toBigNum3(a,b)      padNum3(grabBigNum(a,b))
#define theRest(a,b)        (a).substr(64*(b),(a).length());
#define toVote(a,b)         (grabBigNum(a,b)?"Yea":"Nay")
#define toBoolean(a,b)      (grabBigNum(a,b)?"true":"false")
#define toBytes(a,b)        ((a).substr(64*(b),64))
SFString parse(const SFString& params, int nItems, SFString *types)
{
    SFString ret;
    for (size_t item = 0 ; item < (size_t)nItems ; item++)
    {
        SFString t = types[item];
        bool isDynamic = (t=="string" || t=="bytes" || t.Contains("[]"));
        SFString val;

        if ( t == "address"                    )   val =          toAddr      (params,item);
        else if ( t == "bool"                       )   val =          toBoolean   (params,item);
        else if ( t == "vote"                       )   val =          toVote      (params,item);
        else if ( t == "uint3"                      )   val =          toBigNum3   (params,item);
        else if ( t == "bytes256"                   )   val =          toAscString (params,item);
        else if ( t.Contains("int") &&   !isDynamic )   val =          toBigNum2   (params,item);
        else if ( t.Contains("bytes") && !isDynamic )   val =          toBytes     (params,item);
        else if ( isDynamic                         )   val = "off:" + toBigNum2   (params,item);
        else                                            val = "unknown type: " + t;

        if (val.Contains("off:"))
        {
            size_t start = toLong32u(val.Substitute("off:","")) / (size_t)32;
            size_t len   = grabBigNum(params,start);
            if (len == NOPOS)
                len = params.length()-start;
            if (t == "string")
                val = hex2String(params.substr((start+1)*64,len*2)).Substitute("\n","\\n").Substitute("\r","");
            else
                val = "0x"+params.substr((start+1)*64,len*2);
        }

        ret += ("|" + val);
    }
    return ret;
}

//---------------------------------------------------------------------------
SFString CTransaction::inputToFunction(void) const
{
    if (input.length()<10)
        return " ";

    if (funcPtr)
    {
        SFString items[256];
        int nItems = 0;
        for (uint32_t i = 0 ; i < funcPtr->inputs.getCount() ; i++)
            items[nItems++] = funcPtr->inputs[i].type;
        return funcPtr->name + parse(input.substr(10), nItems, items);
    }

    return " ";
}
// EXISTING_CODE
}  // namespace qblocks

