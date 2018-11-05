/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "tokenlib.h"
#include "walletlib.h"
#include "parselib.h"

//-----------------------------------------------------------------------
void parselib_init(QUITHANDLER qh) {
    etherlib_init(qh);
    CConfirmationEvent::registerClass();
    CEscapeHatchBlackistedTokenEvent::registerClass();
    CEscapeHatchCalledEvent::registerClass();
    CExecutionEvent::registerClass();
    CExecutionFailureEvent::registerClass();
    CFundsSentEvent::registerClass();
    COwnerAdditionEvent::registerClass();
    COwnerRemovalEvent::registerClass();
    COwnershipRemovedEvent::registerClass();
    COwnershipRequestedEvent::registerClass();
    COwnershipTransferredEvent::registerClass();
    CRequirementChangeEvent::registerClass();
    CRevocationEvent::registerClass();
    CSubmissionEvent::registerClass();
    CAcceptOwnership::registerClass();
    CChangeHatchEscapeCaller::registerClass();
    CChangeOwner::registerClass();
    CChangeOwnership::registerClass();
    CConfirmTransaction::registerClass();
    CEscapeHatch::registerClass();
    CExecuteTransaction::registerClass();
    CFill::registerClass();
    CFinalizeFunding::registerClass();
    COnApprove::registerClass();
    COnTransfer::registerClass();
    CProposeOwnership::registerClass();
    CProxyPayment::registerClass();
    CRemoveOwnership::registerClass();
    CReplaceOwner::registerClass();
    CRevokeConfirmation::registerClass();
    CSeal::registerClass();
    CSetVault::registerClass();
    CSubmitTransaction::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_MAXOWNERCOUNT = "0xf7c56230";
const string_q func_acceptOwnership = "0x79ba5097";
const string_q func_beneficiary = "0x38af3eed";
const string_q func_changeHatchEscapeCaller = "0xd836fbe8";
const string_q func_changeOwner = "0xa6f9dae1";
const string_q func_changeOwnership = "0x2af4c31e";
const string_q func_confirmTransaction = "0xc01a8c84";
const string_q func_confirmations = "0x3411c81c";
const string_q func_endFundingTime = "0xe4693e98";
const string_q func_escapeHatch = "0xa142d608";
const string_q func_escapeHatchCaller = "0x1f6eb6e7";
const string_q func_escapeHatchDestination = "0xf5b61230";
const string_q func_executeTransaction = "0xee22610b";
const string_q func_fill = "0x884b5dc2";
const string_q func_finalizeFunding = "0xa19ed39d";
const string_q func_getConfirmationCount = "0x8b51d13f";
const string_q func_getConfirmations = "0xb5dc40c3";
const string_q func_getOwner = "0x893d20e8";
const string_q func_getOwners = "0xa0e67e2b";
const string_q func_getTransactionCount = "0x54741525";
const string_q func_getTransactionIds = "0xa8abe69a";
const string_q func_isConfirmed = "0x784547a7";
const string_q func_isTokenEscapable = "0x892db057";
const string_q func_maximumFunding = "0x99d64ab0";
const string_q func_newOwnerCandidate = "0xd091b550";
const string_q func_onApprove = "0xda682aeb";
const string_q func_onTransfer = "0x4a393149";
const string_q func_owner = "0x8da5cb5b";
const string_q func_owners = "0x025e7c27";
const string_q func_proposeOwnership = "0x710bf322";
const string_q func_proxyPayment = "0xf48c3054";
const string_q func_removeOwnership = "0xd617e0d3";
const string_q func_replaceOwner = "0xe20056e6";
const string_q func_required = "0xdc8452cd";
const string_q func_revokeConfirmation = "0x20ea8d86";
const string_q func_seal = "0x3fb27b85";
const string_q func_sealed = "0xe4b203ef";
const string_q func_setVault = "0x6817031b";
const string_q func_startFundingTime = "0xb75ece9c";
const string_q func_submitTransaction = "0xc6427474";
const string_q func_tokenContract = "0x55a373d6";
const string_q func_totalCollected = "0xe29eb836";
const string_q func_transactionCount = "0xb77bf600";
const string_q func_transactions = "0x9ace38c2";
const string_q func_vaultAddress = "0x430bf08a";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_acceptOwnership) {
            // function acceptOwnership()
            // 0x79ba5097
            CAcceptOwnership *a = new CAcceptOwnership;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("acceptOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_changeHatchEscapeCaller) {
            // function changeHatchEscapeCaller(address _newEscapeHatchCaller)
            // 0xd836fbe8
            CChangeHatchEscapeCaller *a = new CChangeHatchEscapeCaller;
            a->CTransaction::operator=(*p);
            a->_newEscapeHatchCaller = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeHatchEscapeCaller", params, nItems, items);
            return a;

        } else if (encoding == func_changeOwner) {
            // function changeOwner(address _newOwner)
            // 0xa6f9dae1
            CChangeOwner *a = new CChangeOwner;
            a->CTransaction::operator=(*p);
            a->_newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeOwner", params, nItems, items);
            return a;

        } else if (encoding == func_changeOwnership) {
            // function changeOwnership(address _newOwner)
            // 0x2af4c31e
            CChangeOwnership *a = new CChangeOwnership;
            a->CTransaction::operator=(*p);
            a->_newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_confirmTransaction) {
            // function confirmTransaction(uint256 transactionId)
            // 0xc01a8c84
            CConfirmTransaction *a = new CConfirmTransaction;
            a->CTransaction::operator=(*p);
            a->transactionId = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("confirmTransaction", params, nItems, items);
            return a;

        } else if (encoding == func_escapeHatch) {
            // function escapeHatch(address _token)
            // 0xa142d608
            CEscapeHatch *a = new CEscapeHatch;
            a->CTransaction::operator=(*p);
            a->_token = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("escapeHatch", params, nItems, items);
            return a;

        } else if (encoding == func_executeTransaction) {
            // function executeTransaction(uint256 transactionId)
            // 0xee22610b
            CExecuteTransaction *a = new CExecuteTransaction;
            a->CTransaction::operator=(*p);
            a->transactionId = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("executeTransaction", params, nItems, items);
            return a;

        } else if (encoding == func_fill) {
            // function fill(uint256[] data)
            // 0x884b5dc2
            CFill *a = new CFill;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->data.push_back(str_2_BigUint(val));
            }
            items[nItems++] = "uint256[]";
            a->articulatedTx = decodeRLP("fill", params, nItems, items);
            return a;

        } else if (encoding == func_finalizeFunding) {
            // function finalizeFunding()
            // 0xa19ed39d
            CFinalizeFunding *a = new CFinalizeFunding;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("finalizeFunding", params, nItems, items);
            return a;

        } else if (encoding == func_onApprove) {
            // function onApprove(address _owner, address _spender, uint256 _amount)
            // 0xda682aeb
            COnApprove *a = new COnApprove;
            a->CTransaction::operator=(*p);
            a->_owner = str_2_Addr(extract(params, 0*64, 64));
            a->_spender = str_2_Addr(extract(params, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("onApprove", params, nItems, items);
            return a;

        } else if (encoding == func_onTransfer) {
            // function onTransfer(address _from, address _to, uint256 _amount)
            // 0x4a393149
            COnTransfer *a = new COnTransfer;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("onTransfer", params, nItems, items);
            return a;

        } else if (encoding == func_proposeOwnership) {
            // function proposeOwnership(address _newOwnerCandidate)
            // 0x710bf322
            CProposeOwnership *a = new CProposeOwnership;
            a->CTransaction::operator=(*p);
            a->_newOwnerCandidate = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("proposeOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_proxyPayment) {
            // function proxyPayment(address _owner)
            // 0xf48c3054
            CProxyPayment *a = new CProxyPayment;
            a->CTransaction::operator=(*p);
            a->_owner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("proxyPayment", params, nItems, items);
            return a;

        } else if (encoding == func_removeOwnership) {
            // function removeOwnership(uint256 _dac)
            // 0xd617e0d3
            CRemoveOwnership *a = new CRemoveOwnership;
            a->CTransaction::operator=(*p);
            a->_dac = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("removeOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_replaceOwner) {
            // function replaceOwner(address owner, address newOwner)
            // 0xe20056e6
            CReplaceOwner *a = new CReplaceOwner;
            a->CTransaction::operator=(*p);
            a->owner = str_2_Addr(extract(params, 0*64, 64));
            a->newOwner = str_2_Addr(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("replaceOwner", params, nItems, items);
            return a;

        } else if (encoding == func_revokeConfirmation) {
            // function revokeConfirmation(uint256 transactionId)
            // 0x20ea8d86
            CRevokeConfirmation *a = new CRevokeConfirmation;
            a->CTransaction::operator=(*p);
            a->transactionId = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("revokeConfirmation", params, nItems, items);
            return a;

        } else if (encoding == func_seal) {
            // function seal()
            // 0x3fb27b85
            CSeal *a = new CSeal;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("seal", params, nItems, items);
            return a;

        } else if (encoding == func_setVault) {
            // function setVault(address _newVaultAddress)
            // 0x6817031b
            CSetVault *a = new CSetVault;
            a->CTransaction::operator=(*p);
            a->_newVaultAddress = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setVault", params, nItems, items);
            return a;

        } else if (encoding == func_submitTransaction) {
            // function submitTransaction(address destination, uint256 value, bytes data)
            // 0xc6427474
            CSubmitTransaction *a = new CSubmitTransaction;
            a->CTransaction::operator=(*p);
            a->destination = str_2_Addr(extract(params, 0*64, 64));
            a->value = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->data = extract(params, 2*64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->articulatedTx = decodeRLP("submitTransaction", params, nItems, items);
            return a;

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return promoteToToken(p);
}

//-----------------------------------------------------------------------
const string_q evt_Confirmation = "0x4a504a94899432a9846e1aa406dceb1bcfd538bb839071d49d1e5e23f5be30ef";
const string_q evt_EscapeHatchBlackistedToken = "0x6b44fa019116f0ba92616e27b01166e395abfb95f2d81476d0a179a538a16bb1";
const string_q evt_EscapeHatchCalled = "0xa50dde912fa22ea0d215a0236093ac45b4d55d6ef0c604c319f900029c5d10f2";
const string_q evt_Execution = "0x33e13ecb54c3076d8e8bb8c2881800a4d972b792045ffae98fdf46df365fed75";
const string_q evt_ExecutionFailure = "0x526441bb6c1aba3c9a4a6ca1d6545da9c2333c8c48343ef398eb858d72b79236";
const string_q evt_FundsSent = "0x8c8f77105ce8ee99e391fd4da140bc14a988eb4cf878b240bfed9b10ae7c6758";
const string_q evt_OwnerAddition = "0xf39e6e1eb0edcf53c221607b54b00cd28f3196fed0a24994dc308b8f611b682d";
const string_q evt_OwnerRemoval = "0x8001553a916ef2f495d26a907cc54d96ed840d7bda71e73194bf5a9df7a76b90";
const string_q evt_OwnershipRemoved = "0x94e8b32e01b9eedfddd778ffbd051a7718cdc14781702884561162dca6f74dbb";
const string_q evt_OwnershipRequested = "0x13a4b3bc0d5234dd3d87c9f1557d8faefa37986da62c36ba49309e2fb2c9aec4";
const string_q evt_OwnershipTransferred = "0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0";
const string_q evt_RequirementChange = "0xa3f1ee9126a074d9326c682f561767f710e927faa811f7a99829d49dc421797a";
const string_q evt_Revocation = "0xf6a317157440607f36269043eb55f1287a5a19ba2216afeab88cd46cbcfb88e9";
const string_q evt_Submission = "0xc0ba8fe4b176c1714197d43b9cc6bcf797a4a7461c5fe8d0ef6e184ae7601e51";

//-----------------------------------------------------------------------
const CLogEntry *promoteToEvent(const CLogEntry *p) {

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q items[256];
        size_t nItems = 0;
        string_q data = extract(p->data, 2);
        string_q params;
        bool first = true;
        for (auto t : p->topics) {
            if (!first)
                params += extract(topic_2_Str(t),2);
            first = false;
        }
        params += data;
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_Confirmation) {
            // event Confirmation(address indexed sender, uint256 indexed transactionId)
            // 0x4a504a94899432a9846e1aa406dceb1bcfd538bb839071d49d1e5e23f5be30ef
            CConfirmationEvent *a = new CConfirmationEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->transactionId = str_2_Wei(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Confirmation", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_EscapeHatchBlackistedToken) {
            // event EscapeHatchBlackistedToken(address token)
            // 0x6b44fa019116f0ba92616e27b01166e395abfb95f2d81476d0a179a538a16bb1
            CEscapeHatchBlackistedTokenEvent *a = new CEscapeHatchBlackistedTokenEvent;
            a->CLogEntry::operator=(*p);
            a->token = str_2_Addr(extract(data, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("EscapeHatchBlackistedToken", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_EscapeHatchCalled) {
            // event EscapeHatchCalled(address token, uint256 amount)
            // 0xa50dde912fa22ea0d215a0236093ac45b4d55d6ef0c604c319f900029c5d10f2
            CEscapeHatchCalledEvent *a = new CEscapeHatchCalledEvent;
            a->CLogEntry::operator=(*p);
            a->token = str_2_Addr(extract(data, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("EscapeHatchCalled", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Execution) {
            // event Execution(uint256 indexed transactionId)
            // 0x33e13ecb54c3076d8e8bb8c2881800a4d972b792045ffae98fdf46df365fed75
            CExecutionEvent *a = new CExecutionEvent;
            a->CLogEntry::operator=(*p);
            a->transactionId = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Execution", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ExecutionFailure) {
            // event ExecutionFailure(uint256 indexed transactionId)
            // 0x526441bb6c1aba3c9a4a6ca1d6545da9c2333c8c48343ef398eb858d72b79236
            CExecutionFailureEvent *a = new CExecutionFailureEvent;
            a->CLogEntry::operator=(*p);
            a->transactionId = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ExecutionFailure", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_FundsSent) {
            // event FundsSent(address indexed sender, uint256 amount)
            // 0x8c8f77105ce8ee99e391fd4da140bc14a988eb4cf878b240bfed9b10ae7c6758
            CFundsSentEvent *a = new CFundsSentEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("FundsSent", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnerAddition) {
            // event OwnerAddition(address indexed owner)
            // 0xf39e6e1eb0edcf53c221607b54b00cd28f3196fed0a24994dc308b8f611b682d
            COwnerAdditionEvent *a = new COwnerAdditionEvent;
            a->CLogEntry::operator=(*p);
            a->owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnerAddition", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnerRemoval) {
            // event OwnerRemoval(address indexed owner)
            // 0x8001553a916ef2f495d26a907cc54d96ed840d7bda71e73194bf5a9df7a76b90
            COwnerRemovalEvent *a = new COwnerRemovalEvent;
            a->CLogEntry::operator=(*p);
            a->owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnerRemoval", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnershipRemoved) {
            // event OwnershipRemoved()
            // 0x94e8b32e01b9eedfddd778ffbd051a7718cdc14781702884561162dca6f74dbb
            COwnershipRemovedEvent *a = new COwnershipRemovedEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("OwnershipRemoved", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnershipRequested) {
            // event OwnershipRequested(address indexed by, address indexed to)
            // 0x13a4b3bc0d5234dd3d87c9f1557d8faefa37986da62c36ba49309e2fb2c9aec4
            COwnershipRequestedEvent *a = new COwnershipRequestedEvent;
            a->CLogEntry::operator=(*p);
            a->by = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnershipRequested", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnershipTransferred) {
            // event OwnershipTransferred(address indexed from, address indexed to)
            // 0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0
            COwnershipTransferredEvent *a = new COwnershipTransferredEvent;
            a->CLogEntry::operator=(*p);
            a->from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnershipTransferred", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_RequirementChange) {
            // event RequirementChange(uint256 required)
            // 0xa3f1ee9126a074d9326c682f561767f710e927faa811f7a99829d49dc421797a
            CRequirementChangeEvent *a = new CRequirementChangeEvent;
            a->CLogEntry::operator=(*p);
            a->required = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("RequirementChange", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Revocation) {
            // event Revocation(address indexed sender, uint256 indexed transactionId)
            // 0xf6a317157440607f36269043eb55f1287a5a19ba2216afeab88cd46cbcfb88e9
            CRevocationEvent *a = new CRevocationEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->transactionId = str_2_Wei(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Revocation", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Submission) {
            // event Submission(uint256 indexed transactionId)
            // 0xc0ba8fe4b176c1714197d43b9cc6bcf797a4a7461c5fe8d0ef6e184ae7601e51
            CSubmissionEvent *a = new CSubmissionEvent;
            a->CLogEntry::operator=(*p);
            a->transactionId = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Submission", params, nItems, items);
            return a;

        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return promoteToTokenEvent(p);
}

//-----------------------------------------------------------------------------
string_q sigs[] = {
    // Token support
    func_approve_qb,
    func_transferFrom_qb,
    func_transfer_qb,
    // Wallet support
    func_addOwner_qb,
    func_changeOwner_qb,
    func_changeRequirement_qb,
    func_confirm_qb,
    func_execute_qb,
    func_isOwner_qb,
    func_kill_qb,
    func_removeOwner_qb,
    func_resetSpentToday_qb,
    func_revoke_qb,
    func_setDailyLimit_qb,
    // Contract support
    func_MAXOWNERCOUNT,
    func_acceptOwnership,
    func_beneficiary,
    func_changeHatchEscapeCaller,
    func_changeOwner,
    func_changeOwnership,
    func_confirmTransaction,
    func_confirmations,
    func_endFundingTime,
    func_escapeHatch,
    func_escapeHatchCaller,
    func_escapeHatchDestination,
    func_executeTransaction,
    func_fill,
    func_finalizeFunding,
    func_getConfirmationCount,
    func_getConfirmations,
    func_getOwner,
    func_getOwners,
    func_getTransactionCount,
    func_getTransactionIds,
    func_isConfirmed,
    func_isTokenEscapable,
    func_maximumFunding,
    func_newOwnerCandidate,
    func_onApprove,
    func_onTransfer,
    func_owner,
    func_owners,
    func_proposeOwnership,
    func_proxyPayment,
    func_removeOwnership,
    func_replaceOwner,
    func_required,
    func_revokeConfirmation,
    func_seal,
    func_sealed,
    func_setVault,
    func_startFundingTime,
    func_submitTransaction,
    func_tokenContract,
    func_totalCollected,
    func_transactionCount,
    func_transactions,
    func_vaultAddress,
};
size_t nSigs = sizeof(sigs) / sizeof(string_q);

//-----------------------------------------------------------------------------
string_q topics[] = {
    // Token support
    evt_Transfer_qb,
    evt_Approval_qb,
    // Wallet support
    evt_Confirmation_qb,
    evt_ConfirmationNeeded_qb,
    evt_Deposit_qb,
    evt_MultiTransact_qb,
    evt_OwnerAdded_qb,
    evt_OwnerChanged_qb,
    evt_OwnerRemoved_qb,
    evt_RequirementChanged_qb,
    evt_Revoke_qb,
    evt_SingleTransact_qb,
    // Contract support
    evt_Confirmation,
    evt_EscapeHatchBlackistedToken,
    evt_EscapeHatchCalled,
    evt_Execution,
    evt_ExecutionFailure,
    evt_FundsSent,
    evt_OwnerAddition,
    evt_OwnerRemoval,
    evt_OwnershipRemoved,
    evt_OwnershipRequested,
    evt_OwnershipTransferred,
    evt_RequirementChange,
    evt_Revocation,
    evt_Submission,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x5adf43dd006c6c36506e2b2dfa352e60002d22dc
[{"constant":true,"inputs":[],"name":"escapeHatchCaller","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"changeOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"beneficiary","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_newOwnerCandidate","type":"address"}],"name":"proposeOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_token","type":"address"}],"name":"isTokenEscapable","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"}],"name":"escapeHatch","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"newOwnerCandidate","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_dac","type":"uint256"}],"name":"removeOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_newEscapeHatchCaller","type":"address"}],"name":"changeHatchEscapeCaller","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"escapeHatchDestination","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"_beneficiary","type":"address"},{"name":"_escapeHatchCaller","type":"address"},{"name":"_escapeHatchDestination","type":"address"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"FundsSent","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"}],"name":"EscapeHatchBlackistedToken","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"EscapeHatchCalled","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"by","type":"address"},{"indexed":true,"name":"to","type":"address"}],"name":"OwnershipRequested","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"}],"name":"OwnershipTransferred","type":"event"},{"anonymous":false,"inputs":[],"name":"OwnershipRemoved","type":"event"}]

ABI for addr : 0x8702b8d7a1eefec7e54636e26a9323dc96a8dc25
[{"constant":false,"inputs":[],"name":"seal","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"vaultAddress","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"onTransfer","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenContract","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newVaultAddress","type":"address"}],"name":"setVault","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"data","type":"uint256[]"}],"name":"fill","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"getOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"maximumFunding","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"finalizeFunding","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"changeOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"startFundingTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"onApprove","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalCollected","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"endFundingTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"sealed","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"proxyPayment","outputs":[{"name":"","type":"bool"}],"payable":true,"type":"function"},{"inputs":[{"name":"_startFundingTime","type":"uint256"},{"name":"_endFundingTime","type":"uint256"},{"name":"_maximumFunding","type":"uint256"},{"name":"_vaultAddress","type":"address"},{"name":"_tokenAddress","type":"address"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"}]

ABI for addr : 0x839395e20bbb182fa440d08f850e6c7a8f6f0780
[]

ABI for addr : 0x8f951903c9360345b4e1b536c7f5ae8f88a64e79
[{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"owners","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"}],"name":"removeOwner","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"revokeConfirmation","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"isOwner","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"},{"name":"","type":"address"}],"name":"confirmations","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"pending","type":"bool"},{"name":"executed","type":"bool"}],"name":"getTransactionCount","outputs":[{"name":"count","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"}],"name":"addOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"isConfirmed","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"getConfirmationCount","outputs":[{"name":"count","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"transactions","outputs":[{"name":"destination","type":"address"},{"name":"value","type":"uint256"},{"name":"data","type":"bytes"},{"name":"executed","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"getOwners","outputs":[{"name":"","type":"address[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"from","type":"uint256"},{"name":"to","type":"uint256"},{"name":"pending","type":"bool"},{"name":"executed","type":"bool"}],"name":"getTransactionIds","outputs":[{"name":"_transactionIds","type":"uint256[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"getConfirmations","outputs":[{"name":"_confirmations","type":"address[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transactionCount","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_required","type":"uint256"}],"name":"changeRequirement","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"confirmTransaction","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"destination","type":"address"},{"name":"value","type":"uint256"},{"name":"data","type":"bytes"}],"name":"submitTransaction","outputs":[{"name":"transactionId","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"MAX_OWNER_COUNT","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"required","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"},{"name":"newOwner","type":"address"}],"name":"replaceOwner","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"executeTransaction","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_owners","type":"address[]"},{"name":"_required","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Confirmation","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Revocation","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Submission","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Execution","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"ExecutionFailure","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"}],"name":"OwnerAddition","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"}],"name":"OwnerRemoval","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"required","type":"uint256"}],"name":"RequirementChange","type":"event"}]

*/
