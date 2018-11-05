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
    CBurnEvent::registerClass();
    CMintEvent::registerClass();
    CMintFinishedEvent::registerClass();
    CMintTimelockedEvent::registerClass();
    COwnershipTransferredEvent::registerClass();
    CAddExcluded::registerClass();
    CAllowMoveTokens::registerClass();
    CBurn::registerClass();
    CDecreaseApproval::registerClass();
    CFinishMinting::registerClass();
    CIncreaseApproval::registerClass();
    CMint::registerClass();
    CMintTimelocked::registerClass();
    CReturnFrozenFreeFunds::registerClass();
    CTransferOwnership::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_addExcluded = "0xa9321573";
const string_q func_allowMoveTokens = "0x337a8cdb";
const string_q func_burn = "0x42966c68";
const string_q func_decreaseApproval = "0x66188463";
const string_q func_finishMinting = "0x7d64bcb4";
const string_q func_frozenFunds = "0x4fa87fd3";
const string_q func_increaseApproval = "0xd73dd623";
const string_q func_mint = "0x40c10f19";
const string_q func_mintTimelocked = "0xc78cdf66";
const string_q func_mintingFinished = "0x05d2035b";
const string_q func_owner = "0x8da5cb5b";
const string_q func_paused = "0x5c975abb";
const string_q func_returnFrozenFreeFunds = "0xc84748f9";
const string_q func_transferOwnership = "0xf2fde38b";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_addExcluded) {
            // function addExcluded(address _toExclude)
            // 0xa9321573
            CAddExcluded *a = new CAddExcluded;
            a->CTransaction::operator=(*p);
            a->_toExclude = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("addExcluded", params, nItems, items);
            return a;

        } else if (encoding == func_allowMoveTokens) {
            // function allowMoveTokens()
            // 0x337a8cdb
            CAllowMoveTokens *a = new CAllowMoveTokens;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("allowMoveTokens", params, nItems, items);
            return a;

        } else if (encoding == func_burn) {
            // function burn(uint256 _value)
            // 0x42966c68
            CBurn *a = new CBurn;
            a->CTransaction::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("burn", params, nItems, items);
            return a;

        } else if (encoding == func_decreaseApproval) {
            // function decreaseApproval(address _spender, uint256 _subtractedValue)
            // 0x66188463
            CDecreaseApproval *a = new CDecreaseApproval;
            a->CTransaction::operator=(*p);
            a->_spender = str_2_Addr(extract(params, 0*64, 64));
            a->_subtractedValue = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("decreaseApproval", params, nItems, items);
            return a;

        } else if (encoding == func_finishMinting) {
            // function finishMinting()
            // 0x7d64bcb4
            CFinishMinting *a = new CFinishMinting;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("finishMinting", params, nItems, items);
            return a;

        } else if (encoding == func_increaseApproval) {
            // function increaseApproval(address _spender, uint256 _addedValue)
            // 0xd73dd623
            CIncreaseApproval *a = new CIncreaseApproval;
            a->CTransaction::operator=(*p);
            a->_spender = str_2_Addr(extract(params, 0*64, 64));
            a->_addedValue = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("increaseApproval", params, nItems, items);
            return a;

        } else if (encoding == func_mint) {
            // function mint(address _to, uint256 _amount)
            // 0x40c10f19
            CMint *a = new CMint;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("mint", params, nItems, items);
            return a;

        } else if (encoding == func_mintTimelocked) {
            // function mintTimelocked(address _to, uint256 _amount, uint32 _releaseTime)
            // 0xc78cdf66
            CMintTimelocked *a = new CMintTimelocked;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_releaseTime = (uint32_t)str_2_Uint(extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint32";
            a->articulatedTx = decodeRLP("mintTimelocked", params, nItems, items);
            return a;

        } else if (encoding == func_returnFrozenFreeFunds) {
            // function returnFrozenFreeFunds()
            // 0xc84748f9
            CReturnFrozenFreeFunds *a = new CReturnFrozenFreeFunds;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("returnFrozenFreeFunds", params, nItems, items);
            return a;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address newOwner)
            // 0xf2fde38b
            CTransferOwnership *a = new CTransferOwnership;
            a->CTransaction::operator=(*p);
            a->newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("transferOwnership", params, nItems, items);
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
const string_q evt_Burn = "0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5";
const string_q evt_Mint = "0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885";
const string_q evt_MintFinished = "0xae5184fba832cb2b1f702aca6117b8d265eaf03ad33eb133f19dde0f5920fa08";
const string_q evt_MintTimelocked = "0x1385625d3103ed8e66df4d088b0073c5061943280caaa7e96986e0e59391e8c5";
const string_q evt_OwnershipTransferred = "0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0";

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

        if (topic_2_Str(p->topics[0]) % evt_Burn) {
            // event Burn(address indexed burner, uint256 value)
            // 0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5
            CBurnEvent *a = new CBurnEvent;
            a->CLogEntry::operator=(*p);
            a->burner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Burn", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Mint) {
            // event Mint(address indexed to, uint256 amount)
            // 0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885
            CMintEvent *a = new CMintEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Mint", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_MintFinished) {
            // event MintFinished()
            // 0xae5184fba832cb2b1f702aca6117b8d265eaf03ad33eb133f19dde0f5920fa08
            CMintFinishedEvent *a = new CMintFinishedEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("MintFinished", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_MintTimelocked) {
            // event MintTimelocked(address indexed beneficiary, uint256 amount)
            // 0x1385625d3103ed8e66df4d088b0073c5061943280caaa7e96986e0e59391e8c5
            CMintTimelockedEvent *a = new CMintTimelockedEvent;
            a->CLogEntry::operator=(*p);
            a->beneficiary = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("MintTimelocked", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnershipTransferred) {
            // event OwnershipTransferred(address indexed previousOwner, address indexed newOwner)
            // 0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0
            COwnershipTransferredEvent *a = new COwnershipTransferredEvent;
            a->CLogEntry::operator=(*p);
            a->previousOwner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->newOwner = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnershipTransferred", params, nItems, items);
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
    func_addExcluded,
    func_allowMoveTokens,
    func_burn,
    func_decreaseApproval,
    func_finishMinting,
    func_frozenFunds,
    func_increaseApproval,
    func_mint,
    func_mintTimelocked,
    func_mintingFinished,
    func_owner,
    func_paused,
    func_returnFrozenFreeFunds,
    func_transferOwnership,
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
    evt_Burn,
    evt_Mint,
    evt_MintFinished,
    evt_MintTimelocked,
    evt_OwnershipTransferred,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xe29c5b523590165795bbd7d52369c2895b18841f
[{"constant":true,"inputs":[],"name":"mintingFinished","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"_name","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"_decimals","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"allowMoveTokens","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"mint","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_value","type":"uint256"}],"name":"burn","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"uint256"}],"name":"frozenFunds","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"paused","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_subtractedValue","type":"uint256"}],"name":"decreaseApproval","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"finishMinting","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"_symbol","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_toExclude","type":"address"}],"name":"addExcluded","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_releaseTime","type":"uint32"}],"name":"mintTimelocked","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"returnFrozenFreeFunds","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_addedValue","type":"uint256"}],"name":"increaseApproval","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"burner","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Burn","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"beneficiary","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"MintTimelocked","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"Mint","type":"event"},{"anonymous":false,"inputs":[],"name":"MintFinished","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"previousOwner","type":"address"},{"indexed":true,"name":"newOwner","type":"address"}],"name":"OwnershipTransferred","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"}]

*/
