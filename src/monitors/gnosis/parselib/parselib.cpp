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
    CBidSubmissionEvent::registerClass();
    CConfirmationEvent::registerClass();
    CDailyLimitChangeEvent::registerClass();
    CExecutionEvent::registerClass();
    CExecutionFailureEvent::registerClass();
    COwnerAdditionEvent::registerClass();
    COwnerRemovalEvent::registerClass();
    CRefundReceivedEvent::registerClass();
    CRefundSubmissionEvent::registerClass();
    CRequirementChangeEvent::registerClass();
    CRevocationEvent::registerClass();
    CSubmissionEvent::registerClass();
    CBid::registerClass();
    CBidProxy::registerClass();
    CCalcCurrentTokenPrice::registerClass();
    CChangeDailyLimit::registerClass();
    CChangeSettings::registerClass();
    CClaimProxy::registerClass();
    CClaimTokens::registerClass();
    CConfirmTransaction::registerClass();
    CContribute::registerClass();
    CExecuteTransaction::registerClass();
    CRefund::registerClass();
    CReplaceOwner::registerClass();
    CRevokeConfirmation::registerClass();
    CSetup::registerClass();
    CStartAuction::registerClass();
    CSubmitTransaction::registerClass();
    CTransfer::registerClass();
    CUpdateStage::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_AUCTIONSTARTED = "0x5b652d47";
const string_q func_MAXOWNERCOUNT = "0xf7c56230";
const string_q func_MAXTOKENSSOLD = "0x46a2818b";
const string_q func_TRADINGSTARTED = "0x75dcd9e1";
const string_q func_WAITINGPERIOD = "0x088c4e70";
const string_q func_bid = "0x9cf5453d";
const string_q func_bidProxy = "0xed9a3ac1";
const string_q func_bids = "0x62ea82db";
const string_q func_calcCurrentTokenPrice = "0xd9f8a4e2";
const string_q func_calcMaxWithdraw = "0x4bc9fdc2";
const string_q func_calcStopPrice = "0xdd9dd688";
const string_q func_calcTokenPrice = "0x39f64b52";
const string_q func_ceiling = "0x753ed1bd";
const string_q func_changeDailyLimit = "0xcea08621";
const string_q func_changeSettings = "0x36d02c94";
const string_q func_claimProxy = "0x30b66cee";
const string_q func_claimTokens = "0xdf8de3e7";
const string_q func_confirmTransaction = "0xc01a8c84";
const string_q func_confirmations = "0x3411c81c";
const string_q func_contribute = "0xd7bb99ba";
const string_q func_contributions = "0x42e94c90";
const string_q func_dailyLimit = "0x67eeba0c";
const string_q func_dutchAuction = "0x9da0d7d4";
const string_q func_endTime = "0x3197cbb6";
const string_q func_executeTransaction = "0xee22610b";
const string_q func_finalPrice = "0xa6b513ee";
const string_q func_getConfirmationCount = "0x8b51d13f";
const string_q func_getConfirmations = "0xb5dc40c3";
const string_q func_getOwners = "0xa0e67e2b";
const string_q func_getTransactionCount = "0x54741525";
const string_q func_getTransactionIds = "0xa8abe69a";
const string_q func_gnosisToken = "0x60fd902c";
const string_q func_isConfirmed = "0x784547a7";
const string_q func_lastDay = "0x6b0c932d";
const string_q func_owner = "0x8da5cb5b";
const string_q func_owners = "0x025e7c27";
const string_q func_priceFactor = "0xdfb2866d";
const string_q func_refund = "0x590e1ae3";
const string_q func_replaceOwner = "0xe20056e6";
const string_q func_required = "0xdc8452cd";
const string_q func_revokeConfirmation = "0x20ea8d86";
const string_q func_setup = "0x66d38203";
const string_q func_spentToday = "0xf059cf2b";
const string_q func_stage = "0xc040e6b8";
const string_q func_startAuction = "0x6b64c769";
const string_q func_startBlock = "0x48cd4cb1";
const string_q func_submitTransaction = "0xc6427474";
const string_q func_totalBalance = "0xad7a672f";
const string_q func_totalContributions = "0x37c08923";
const string_q func_totalReceived = "0xa3c2c462";
const string_q func_totalTokens = "0x7e1c0c09";
const string_q func_transactionCount = "0xb77bf600";
const string_q func_transactions = "0x9ace38c2";
const string_q func_transfer = "0x8a4068dd";
const string_q func_updateStage = "0xc062f578";
const string_q func_wallet = "0x521eb273";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_bid) {
            // function bid(address receiver)
            // 0x9cf5453d
            CBid *a = new CBid;
            a->CTransaction::operator=(*p);
            a->receiver = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("bid", params, nItems, items);
            return a;

        } else if (encoding == func_bidProxy) {
            // function bidProxy()
            // 0xed9a3ac1
            CBidProxy *a = new CBidProxy;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("bidProxy", params, nItems, items);
            return a;

        } else if (encoding == func_calcCurrentTokenPrice) {
            // function calcCurrentTokenPrice()
            // 0xd9f8a4e2
            CCalcCurrentTokenPrice *a = new CCalcCurrentTokenPrice;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("calcCurrentTokenPrice", params, nItems, items);
            return a;

        } else if (encoding == func_changeDailyLimit) {
            // function changeDailyLimit(uint256 _dailyLimit)
            // 0xcea08621
            CChangeDailyLimit *a = new CChangeDailyLimit;
            a->CTransaction::operator=(*p);
            a->_dailyLimit = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeDailyLimit", params, nItems, items);
            return a;

        } else if (encoding == func_changeSettings) {
            // function changeSettings(uint256 _ceiling, uint256 _priceFactor)
            // 0x36d02c94
            CChangeSettings *a = new CChangeSettings;
            a->CTransaction::operator=(*p);
            a->_ceiling = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_priceFactor = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeSettings", params, nItems, items);
            return a;

        } else if (encoding == func_claimProxy) {
            // function claimProxy()
            // 0x30b66cee
            CClaimProxy *a = new CClaimProxy;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("claimProxy", params, nItems, items);
            return a;

        } else if (encoding == func_claimTokens) {
            // function claimTokens(address receiver)
            // 0xdf8de3e7
            CClaimTokens *a = new CClaimTokens;
            a->CTransaction::operator=(*p);
            a->receiver = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("claimTokens", params, nItems, items);
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

        } else if (encoding == func_contribute) {
            // function contribute()
            // 0xd7bb99ba
            CContribute *a = new CContribute;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("contribute", params, nItems, items);
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

        } else if (encoding == func_refund) {
            // function refund()
            // 0x590e1ae3
            CRefund *a = new CRefund;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("refund", params, nItems, items);
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

        } else if (encoding == func_setup) {
            // function setup(address _gnosisToken)
            // 0x66d38203
            CSetup *a = new CSetup;
            a->CTransaction::operator=(*p);
            a->_gnosisToken = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setup", params, nItems, items);
            return a;

        } else if (encoding == func_startAuction) {
            // function startAuction()
            // 0x6b64c769
            CStartAuction *a = new CStartAuction;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("startAuction", params, nItems, items);
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

        } else if (encoding == func_transfer) {
            // function transfer()
            // 0x8a4068dd
            CTransfer *a = new CTransfer;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("transfer", params, nItems, items);
            return a;

        } else if (encoding == func_updateStage) {
            // function updateStage()
            // 0xc062f578
            CUpdateStage *a = new CUpdateStage;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("updateStage", params, nItems, items);
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
const string_q evt_BidSubmission = "0x9c89e828e6cee6374b68147c9e753e41746aad98ba255a527a4fc0ac1868bff9";
const string_q evt_Confirmation = "0x4a504a94899432a9846e1aa406dceb1bcfd538bb839071d49d1e5e23f5be30ef";
const string_q evt_DailyLimitChange = "0xc71bdc6afaf9b1aa90a7078191d4fc1adf3bf680fca3183697df6b0dc226bca2";
const string_q evt_Execution = "0x33e13ecb54c3076d8e8bb8c2881800a4d972b792045ffae98fdf46df365fed75";
const string_q evt_ExecutionFailure = "0x526441bb6c1aba3c9a4a6ca1d6545da9c2333c8c48343ef398eb858d72b79236";
const string_q evt_OwnerAddition = "0xf39e6e1eb0edcf53c221607b54b00cd28f3196fed0a24994dc308b8f611b682d";
const string_q evt_OwnerRemoval = "0x8001553a916ef2f495d26a907cc54d96ed840d7bda71e73194bf5a9df7a76b90";
const string_q evt_RefundReceived = "0x5f9145ef1ebbf201190a609438526a6db7cdbd1c7a06a90825b66f8dfe65c2fb";
const string_q evt_RefundSubmission = "0x5d87eb33a71045c24cb2e99ed9335ed748942197a95c73788324ff369f500217";
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

        if (topic_2_Str(p->topics[0]) % evt_BidSubmission) {
            // event BidSubmission(address indexed sender, uint256 amount)
            // 0x9c89e828e6cee6374b68147c9e753e41746aad98ba255a527a4fc0ac1868bff9
            CBidSubmissionEvent *a = new CBidSubmissionEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("BidSubmission", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Confirmation) {
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

        } else if (topic_2_Str(p->topics[0]) % evt_DailyLimitChange) {
            // event DailyLimitChange(uint256 dailyLimit)
            // 0xc71bdc6afaf9b1aa90a7078191d4fc1adf3bf680fca3183697df6b0dc226bca2
            CDailyLimitChangeEvent *a = new CDailyLimitChangeEvent;
            a->CLogEntry::operator=(*p);
            a->dailyLimit = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("DailyLimitChange", params, nItems, items);
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

        } else if (topic_2_Str(p->topics[0]) % evt_RefundReceived) {
            // event RefundReceived(uint256 amount)
            // 0x5f9145ef1ebbf201190a609438526a6db7cdbd1c7a06a90825b66f8dfe65c2fb
            CRefundReceivedEvent *a = new CRefundReceivedEvent;
            a->CLogEntry::operator=(*p);
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("RefundReceived", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_RefundSubmission) {
            // event RefundSubmission(address indexed sender, uint256 amount)
            // 0x5d87eb33a71045c24cb2e99ed9335ed748942197a95c73788324ff369f500217
            CRefundSubmissionEvent *a = new CRefundSubmissionEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("RefundSubmission", params, nItems, items);
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
    func_AUCTIONSTARTED,
    func_MAXOWNERCOUNT,
    func_MAXTOKENSSOLD,
    func_TRADINGSTARTED,
    func_WAITINGPERIOD,
    func_bid,
    func_bidProxy,
    func_bids,
    func_calcCurrentTokenPrice,
    func_calcMaxWithdraw,
    func_calcStopPrice,
    func_calcTokenPrice,
    func_ceiling,
    func_changeDailyLimit,
    func_changeSettings,
    func_claimProxy,
    func_claimTokens,
    func_confirmTransaction,
    func_confirmations,
    func_contribute,
    func_contributions,
    func_dailyLimit,
    func_dutchAuction,
    func_endTime,
    func_executeTransaction,
    func_finalPrice,
    func_getConfirmationCount,
    func_getConfirmations,
    func_getOwners,
    func_getTransactionCount,
    func_getTransactionIds,
    func_gnosisToken,
    func_isConfirmed,
    func_lastDay,
    func_owner,
    func_owners,
    func_priceFactor,
    func_refund,
    func_replaceOwner,
    func_required,
    func_revokeConfirmation,
    func_setup,
    func_spentToday,
    func_stage,
    func_startAuction,
    func_startBlock,
    func_submitTransaction,
    func_totalBalance,
    func_totalContributions,
    func_totalReceived,
    func_totalTokens,
    func_transactionCount,
    func_transactions,
    func_transfer,
    func_updateStage,
    func_wallet,
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
    evt_BidSubmission,
    evt_Confirmation,
    evt_DailyLimitChange,
    evt_Execution,
    evt_ExecutionFailure,
    evt_OwnerAddition,
    evt_OwnerRemoval,
    evt_RefundReceived,
    evt_RefundSubmission,
    evt_RequirementChange,
    evt_Revocation,
    evt_Submission,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xbe4e25443df23382228e6347b06565edd838f901
[]

ABI for addr : 0x851b7f3ab81bd8df354f0d7640efcd7288553419
[{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"owners","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"}],"name":"removeOwner","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"revokeConfirmation","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"isOwner","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"},{"name":"","type":"address"}],"name":"confirmations","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"calcMaxWithdraw","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"pending","type":"bool"},{"name":"executed","type":"bool"}],"name":"getTransactionCount","outputs":[{"name":"count","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"dailyLimit","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"lastDay","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"}],"name":"addOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"isConfirmed","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"getConfirmationCount","outputs":[{"name":"count","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"transactions","outputs":[{"name":"destination","type":"address"},{"name":"value","type":"uint256"},{"name":"data","type":"bytes"},{"name":"executed","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"getOwners","outputs":[{"name":"","type":"address[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"from","type":"uint256"},{"name":"to","type":"uint256"},{"name":"pending","type":"bool"},{"name":"executed","type":"bool"}],"name":"getTransactionIds","outputs":[{"name":"_transactionIds","type":"uint256[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"getConfirmations","outputs":[{"name":"_confirmations","type":"address[]"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transactionCount","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_required","type":"uint256"}],"name":"changeRequirement","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"confirmTransaction","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"destination","type":"address"},{"name":"value","type":"uint256"},{"name":"data","type":"bytes"}],"name":"submitTransaction","outputs":[{"name":"transactionId","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_dailyLimit","type":"uint256"}],"name":"changeDailyLimit","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"MAX_OWNER_COUNT","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"required","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"owner","type":"address"},{"name":"newOwner","type":"address"}],"name":"replaceOwner","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"transactionId","type":"uint256"}],"name":"executeTransaction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"spentToday","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"_owners","type":"address[]"},{"name":"_required","type":"uint256"},{"name":"_dailyLimit","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"dailyLimit","type":"uint256"}],"name":"DailyLimitChange","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Confirmation","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Revocation","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Submission","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"Execution","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"transactionId","type":"uint256"}],"name":"ExecutionFailure","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"}],"name":"OwnerAddition","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"}],"name":"OwnerRemoval","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"required","type":"uint256"}],"name":"RequirementChange","type":"event"}]

ABI for addr : 0x1d0dcc8d8bcafa8e8502beaeef6cbd49d3affcdc
[{"constant":true,"inputs":[],"name":"MAX_TOKENS_SOLD","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"endTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_ceiling","type":"uint256"},{"name":"_priceFactor","type":"uint256"}],"name":"changeSettings","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"calcTokenPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"startBlock","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"wallet","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"gnosisToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"bids","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_gnosisToken","type":"address"}],"name":"setup","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"startAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"WAITING_PERIOD","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"ceiling","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"receiver","type":"address"}],"name":"bid","outputs":[{"name":"amount","type":"uint256"}],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"totalReceived","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"finalPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"stage","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"updateStage","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"calcCurrentTokenPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"calcStopPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"receiver","type":"address"}],"name":"claimTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"priceFactor","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"_wallet","type":"address"},{"name":"_ceiling","type":"uint256"},{"name":"_priceFactor","type":"uint256"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"BidSubmission","type":"event"}]

ABI for addr : 0x6810e776880c02933d47db1b9fc05908e5386b96
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"dutchAuction","type":"address"},{"name":"owners","type":"address[]"},{"name":"tokens","type":"uint256[]"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0x19076bd01e86fa3d76334e27d4b8e6789688a42d
[{"constant":true,"inputs":[],"name":"AUCTION_STARTED","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"claimProxy","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalContributions","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"contributions","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"refund","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"gnosisToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalTokens","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"transfer","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"TRADING_STARTED","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"dutchAuction","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalBalance","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"stage","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"contribute","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[],"name":"bidProxy","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"inputs":[{"name":"_dutchAuction","type":"address"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"BidSubmission","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"RefundSubmission","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"amount","type":"uint256"}],"name":"RefundReceived","type":"event"}]

*/
