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
IMPLEMENT_NODE(CTransaction, CBaseNode, dataSchema());

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
        case 'c':
            if ( fieldName % "cumulativeGasUsed" ) { cumulativeGasUsed = toWei(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = toUnsigned(fieldValue); return true; }
            if ( fieldName % "gasPrice" ) { gasPrice = toUnsigned(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            if ( fieldName % "isError" ) { isError = toUnsigned(fieldValue); return true; }
            if ( fieldName % "isInternalTx" ) { isInternalTx = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = toUnsigned(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "receipt" ) { /* receipt = fieldValue; */ return false; }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) { transactionIndex = toUnsigned(fieldValue); return true; }
            if ( fieldName % "timestamp" ) { timestamp = toUnsigned(fieldValue); return true; }
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
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CTransaction*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

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
    archive >> cumulativeGasUsed;
    archive >> input;
    archive >> isError;
    archive >> isInternalTx;
    archive >> receipt;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

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
    archive << cumulativeGasUsed;
    archive << input;
    archive << isError;
    archive << isInternalTx;
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
    ADD_FIELD(CTransaction, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "transactionIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "nonce", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "timestamp", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "value", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "gasPrice", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "cumulativeGasUsed", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "isError", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransaction, "isInternalTx", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransaction, "receipt", T_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransaction, "schema");
    HIDE_FIELD(CTransaction, "deleted");

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

    // Hide fields we don't want to show by default
    HIDE_FIELD(CTransaction, "function");
    HIDE_FIELD(CTransaction, "gasCost");
    HIDE_FIELD(CTransaction, "isError");
    HIDE_FIELD(CTransaction, "isInternalTx");
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
                    return asStringBN(used * price);
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
    bool done = false;
    // EXISTING_CODE
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

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockHash" ) return fromHash(blockHash);
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'c':
            if ( fieldName % "cumulativeGasUsed" ) return fromWei(cumulativeGasUsed);
            break;
        case 'f':
            if ( fieldName % "from" ) return fromAddress(from);
            break;
        case 'g':
            if ( fieldName % "gas" ) return asStringU(gas);
            if ( fieldName % "gasPrice" ) return asStringU(gasPrice);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'i':
            if ( fieldName % "input" ) return input;
            if ( fieldName % "isError" ) return asStringU(isError);
            if ( fieldName % "isInternalTx" ) return asStringU(isInternalTx);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return asStringU(nonce);
            break;
        case 'r':
            if ( fieldName % "receipt" ) { expContext().noFrst=true; return receipt.Format(); }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) return asStringU(transactionIndex);
            if ( fieldName % "timestamp" ) return asStringU(timestamp);
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
SFString parseParams(const CTransaction* trans, const SFString& which, const SFString& params)
{
    if (which=="approve")
    {
        //function approve(address _spender, uint256 _amount) returns (bool success) {  discuss
        SFString items[] = { "address", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="transferWithoutReward")
    {
        //function transferWithoutReward(address _to, uint256 _value)
        SFString items[] = { "address", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="retrieveDAOReward")
    {
        // function retrieveDAOReward(bool _toMembers)
        SFString items[] = { "bool", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="splitDAO")
    {
        //function splitDAO(uint _proposalID, address _newCurator)
        SFString items[] = { "uint3", "address", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="createTokenProxy")
    {
        //function createTokenProxy(address _tokenHolder)
        SFString items[] = { "address", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="getMyReward")
    {
        //function getMyReward()
        SFString items[] = { };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="payOut")
    {
        //function payOut(address _recipient, uint _amount)
        SFString items[] = { "address", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="receiveEther")
    {
        //function receiveEther()
        SFString items[] = { };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);
        return which;

    } else if (which=="changeDomain")
    {
        //function changeDomain( uint domain, uint expires, uint price, address transfer )
        SFString items[] = { "bytes256", "uint256", "uint256", "address", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="changeId")
    {
        //function changeId( uint domain, uint name, uint value )
        SFString items[] = { "bytes256", "bytes256", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="getDomain")
    {
        //function getDomain( uint domain )
        SFString items[] = { "bytes256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="getId")
    {
        //function getId( uint domain, uint id )
        SFString items[] = { "bytes256", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="balanceOf")
    {
        //function balanceOf(addr _owner);
        SFString items[] = { "address", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="vote")
    {
        //function vote(uint _proposalID, bool _supportsProposal)
        SFString items[] = { "uint3", "vote", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="executeProposal")
    {
        //function executeProposal(uint _proposalID, bytes _transactionData)
        SFString items[] = { "uint3", "bytes", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="transferFrom")
    {
        //function transferFrom(address _from, address _to, uint256 _amount)
        SFString items[] = { "address", "address", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="transfer")
    {
        //function transfer(address _to, uint256 _amount)
        SFString items[] = { "address", "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="newProposal")
    {
        //function newProposal(address _recipient,uint _amount,string _description,bytes _transactionData,uint _debatingPeriod,bool _newCurator)
        SFString items[] = { "address", "uint256", "string", "bytes", "uint256", "bool", };
        int nItems = sizeof(items) / sizeof(SFString);

        SFString type = (trans->value > 0 ? " (non-split)" : " (split)");
        return which + type + parse(params, nItems, items);

    } else if (which=="SendEmail")
    {
        SFString items[] = { "string","string", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="mint")
    {
        //function mint(address _to, string _identity)
        SFString items[] = { "address","string", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="removeOwner")
    {
        //function removeOwner(address)
        SFString items[] = { "address" };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="isOwner")
    {
        //function isOwner(address)
        SFString items[] = { "address" };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="resetSpentToday")
    {
        //function resetSpentToday()
        SFString items[] = { };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="addOwner")
    {
        //function addOwner(address)
        SFString items[] = { "address" };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="confirm")
    {
        //function confirm(bytes32)
        SFString items[] = { "bytes32", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="setDailyLimit")
    {
        //function setDailyLimit(uint256)
        SFString items[] = { "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="execute")
    {
        //function execute(address,uint256,bytes)
        SFString items[] = { "address","uint256","bytes" };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="revoke")
    {
        //function revoke(bytes32)
        SFString items[] = { "bytes32", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="changeRequirement")
    {
        //function changeRequirement(uint256)
        SFString items[] = { "uint256", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="kill")
    {
        //function kill(address)
        SFString items[] = { "address" };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    } else if (which=="changeOwner")
    {
        //function changeOwner(address,address)
        SFString items[] = { "address","address", };
        int nItems = sizeof(items) / sizeof(SFString);
        return which + parse(params, nItems, items);

    }

    return which;
}

//---------------------------------------------------------------------------
SFString CTransaction::inputToFunction(void) const
{
    if (funcPtr)
    {
        SFString items[256];
        int nItems = 0;
        for (uint32_t i = 0 ; i < funcPtr->inputs.getCount() ; i++)
            items[nItems++] = funcPtr->inputs[i].type;
        return funcPtr->name + parse(input.substr(10), nItems, items);
        //        return parseParams(this, funcPtr->name, input.substr(10));
    }

    if (input.length()<10)
        return " ";
#if 1
    /* from Mist wallet
     */
    // This is needed here because we only check the abi of the asked for account. We should load and check both from and to addresses' abi
    char ch = input[2];
    if (!input.startsWith("0x"))
        ch = input[0];
    switch (ch)
    {
        case '0':
            if      (input.startsWith( "0x095ea7b3" )) return parseParams(this, "approve",                   input.substr(10));
            else if (input.startsWith( "0x0221038a" )) return parseParams(this, "payOut",                    input.substr(10));
            break;

        case '1':
            if      (input.startsWith( "0x1a7a98e2" )) return parseParams(this, "getDomain",                 input.substr(10));
            else if (input.startsWith( "0x173825d9" )) return parseParams(this, "removeOwner",               input.substr(10));
            break;

        case '2':
            if      (input.startsWith( "0x237e9492" )) return parseParams(this, "executeProposal",           input.substr(10));
            else if (input.startsWith( "0x24fc65ed" )) return parseParams(this, "getId",                     input.substr(10));
            else if (input.startsWith( "0x23b872dd" )) return parseParams(this, "transferFrom",              input.substr(10));
            else if (input.startsWith( "0x2632bf20" )) return parseParams(this, "unblockMe",                 input.substr(10));
            else if (input.startsWith( "0x2f54bf6e" )) return parseParams(this, "isOwner",                   input.substr(10));
            break;

        case '3':
            break;

        case '4':
            if      (input.startsWith( "0x4e10c3ee" )) return parseParams(this, "transferWithoutReward",     input.substr(10));
            break;

        case '5':
            if      (input.startsWith( "0x590e1ae3" )) return parseParams(this, "refund",                    input.substr(10));
            else if (input.startsWith( "0x5c52c2f5" )) return parseParams(this, "resetSpentToday",           input.substr(10));
            break;

        case '6':
            if      (input.startsWith( "0x612e45a3" )) return parseParams(this, "newProposal",               input.substr(10));
            break;

        case '7':
            if      (input.startsWith( "0x78524b2e" )) return parseParams(this, "halveMinQuorum",            input.substr(10));
            else if (input.startsWith( "0x75090ebf" )) return parseParams(this, "changeDomain",              input.substr(10));
            else if (input.startsWith( "0x70a08231" )) return parseParams(this, "balanceOf",                 input.substr(10));
            else if (input.startsWith( "0x7065cb48" )) return parseParams(this, "addOwner",                  input.substr(10));
            else if (input.startsWith( "0x797af627" )) return parseParams(this, "confirm",                   input.substr(10));
            break;

        case '8':
            if      (input.startsWith( "0x82661dc4" )) return parseParams(this, "splitDAO",                  input.substr(10));
            break;

        case '9':
            break;

        case 'a':
            if      (input.startsWith( "0xa1da2fb9" )) return parseParams(this, "retrieveDAOReward",         input.substr(10));
            else if (input.startsWith( "0xa3912ec8" )) return parseParams(this, "receiveEther",              input.substr(10));
            else if (input.startsWith( "0xa9059cbb" )) return parseParams(this, "transfer",                  input.substr(10));
            break;

        case 'b':
            if      (input.startsWith( "0xbaac5300" )) return parseParams(this, "createTokenProxy",          input.substr(10));
            else if (input.startsWith( "0xb20d30a9" )) return parseParams(this, "setDailyLimit",             input.substr(10));
            else if (input.startsWith( "0xb61d27f6" )) return parseParams(this, "execute",                   input.substr(10));
            else if (input.startsWith( "0xb75c7dc6" )) return parseParams(this, "revoke",                    input.substr(10));
            else if (input.startsWith( "0xba51a6df" )) return parseParams(this, "changeRequirement",         input.substr(10));
            break;

        case 'c':
            if      (input.startsWith( "0xc9d27afe" )) return parseParams(this, "vote",                      input.substr(10));
            else if (input.startsWith( "0xcc9ae3f6" )) return parseParams(this, "getMyReward",               input.substr(10));
            else if (input.startsWith( "0xcbf0b0c0" )) return parseParams(this, "kill",                      input.substr(10));
            break;

        case 'd':
            if      (input.startsWith( "0xdbde1988" )) return parseParams(this, "transferFromWithoutReward", input.substr(10));
            break;

        case 'e':
            if      (input.startsWith( "0xeceb2945" )) return parseParams(this, "checkProposalCode",         input.substr(10));
            else if (input.startsWith( "0xeb1ff845" )) return parseParams(this, "changeId",                  input.substr(10));
            break;

        case 'f':
            if      (input.startsWith( "0xf00d4b5d" )) return parseParams(this, "changeOwner",               input.substr(10));
            break;
    }

    if (pParent && pParent->isKindOf(GETRUNTIME_CLASS(CAccount)))
    {
        SFString test = input.substr(2,8);
        CAccount *p = (CAccount*)pParent;
        for (uint32_t i=0;i<p->abi.abiByName.getCount();i++)
        {
            CFunction *f = &p->abi.abiByName[i];
            if (f->encoding.ContainsI(test))
            {
                SFString str = f->Format("[{NAME}]");
                return str;
            }
        }
    }
#endif

    return " ";
}
#if 0
archive >> creates;
archive >> confirmations;
archive >> contractAddress;
archive >> cumulativeGasUsed;
#endif
// EXISTING_CODE
}  // namespace qblocks

