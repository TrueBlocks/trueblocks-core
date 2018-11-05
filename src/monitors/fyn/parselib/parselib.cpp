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
    CEmergencyStopActivatedEvent::registerClass();
    CTokenMintEvent::registerClass();
    CTokenSwapOverEvent::registerClass();
    CBuyTokens::registerClass();
    CDisableTokenSwapLock::registerClass();
    CMintReserve::registerClass();
    CMintTokens::registerClass();
    CSetTokenContract::registerClass();
    CStartTokenSwap::registerClass();
    CStopToken::registerClass();
    CStopTokenSwap::registerClass();
    CWithdrawReserve::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_MAXETH = "0x8bf87d18";
const string_q func_PRESALELENGTH = "0x1afff278";
const string_q func_SWAPLENGTH = "0x71614d3c";
const string_q func_TRANSITIONWINDOW = "0x35437611";
const string_q func_amountRaised = "0x7b3e5e7b";
const string_q func_buyTokens = "0xec8ac4d8";
const string_q func_creationTime = "0xd8270dce";
const string_q func_currentSwapRate = "0x5334c231";
const string_q func_disableTokenSwapLock = "0x3592f369";
const string_q func_getOwner = "0xc41a360a";
const string_q func_mintReserve = "0xa91ed8c6";
const string_q func_mintTokens = "0xf0dda65c";
const string_q func_mlastDay = "0xe17db230";
const string_q func_mspentToday = "0x3938e00a";
const string_q func_setTokenContract = "0xbbcd5bbe";
const string_q func_startTokenSwap = "0x40482475";
const string_q func_stopToken = "0xd21c700f";
const string_q func_stopTokenSwap = "0x3c1cc14e";
const string_q func_tokenCap = "0xdd54291b";
const string_q func_tokenCtr = "0xaef251a7";
const string_q func_tokenSwap = "0x9653dee5";
const string_q func_transferStop = "0x6780a311";
const string_q func_walletAddress = "0x6ad5b3ea";
const string_q func_withdrawReserve = "0x52a387ab";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_buyTokens) {
            // function buyTokens(address _beneficiary)
            // 0xec8ac4d8
            CBuyTokens *a = new CBuyTokens;
            a->CTransaction::operator=(*p);
            a->_beneficiary = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("buyTokens", params, nItems, items);
            return a;

        } else if (encoding == func_disableTokenSwapLock) {
            // function disableTokenSwapLock()
            // 0x3592f369
            CDisableTokenSwapLock *a = new CDisableTokenSwapLock;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("disableTokenSwapLock", params, nItems, items);
            return a;

        } else if (encoding == func_mintReserve) {
            // function mintReserve(address beneficiary)
            // 0xa91ed8c6
            CMintReserve *a = new CMintReserve;
            a->CTransaction::operator=(*p);
            a->beneficiary = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("mintReserve", params, nItems, items);
            return a;

        } else if (encoding == func_mintTokens) {
            // function mintTokens(address newTokenHolder, uint256 etherAmount)
            // 0xf0dda65c
            CMintTokens *a = new CMintTokens;
            a->CTransaction::operator=(*p);
            a->newTokenHolder = str_2_Addr(extract(params, 0*64, 64));
            a->etherAmount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("mintTokens", params, nItems, items);
            return a;

        } else if (encoding == func_setTokenContract) {
            // function setTokenContract(address newTokenContractAddr)
            // 0xbbcd5bbe
            CSetTokenContract *a = new CSetTokenContract;
            a->CTransaction::operator=(*p);
            a->newTokenContractAddr = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setTokenContract", params, nItems, items);
            return a;

        } else if (encoding == func_startTokenSwap) {
            // function startTokenSwap()
            // 0x40482475
            CStartTokenSwap *a = new CStartTokenSwap;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("startTokenSwap", params, nItems, items);
            return a;

        } else if (encoding == func_stopToken) {
            // function stopToken()
            // 0xd21c700f
            CStopToken *a = new CStopToken;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("stopToken", params, nItems, items);
            return a;

        } else if (encoding == func_stopTokenSwap) {
            // function stopTokenSwap()
            // 0x3c1cc14e
            CStopTokenSwap *a = new CStopTokenSwap;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("stopTokenSwap", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawReserve) {
            // function withdrawReserve(address _beneficiary)
            // 0x52a387ab
            CWithdrawReserve *a = new CWithdrawReserve;
            a->CTransaction::operator=(*p);
            a->_beneficiary = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("withdrawReserve", params, nItems, items);
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
const string_q evt_EmergencyStopActivated = "0x4e360eaf5c2a0e4ebb03155da87ffa096252cc2ec66db5848b4ef13e99172bbd";
const string_q evt_TokenMint = "0x36bf5aa3964be01dbd95a0154a8930793fe68353bdc580871ffb2c911366bbc7";
const string_q evt_TokenSwapOver = "0x0d27864fb2752ddaaa945c943c62b77c1476125056d0343ab1e2159da779fa40";

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

        if (topic_2_Str(p->topics[0]) % evt_EmergencyStopActivated) {
            // event EmergencyStopActivated()
            // 0x4e360eaf5c2a0e4ebb03155da87ffa096252cc2ec66db5848b4ef13e99172bbd
            CEmergencyStopActivatedEvent *a = new CEmergencyStopActivatedEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("EmergencyStopActivated", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_TokenMint) {
            // event TokenMint(address newTokenHolder, uint256 amountOfTokens)
            // 0x36bf5aa3964be01dbd95a0154a8930793fe68353bdc580871ffb2c911366bbc7
            CTokenMintEvent *a = new CTokenMintEvent;
            a->CLogEntry::operator=(*p);
            a->newTokenHolder = str_2_Addr(extract(data, 0*64, 64));
            a->amountOfTokens = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("TokenMint", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_TokenSwapOver) {
            // event TokenSwapOver()
            // 0x0d27864fb2752ddaaa945c943c62b77c1476125056d0343ab1e2159da779fa40
            CTokenSwapOverEvent *a = new CTokenSwapOverEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("TokenSwapOver", params, nItems, items);
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
    func_MAXETH,
    func_PRESALELENGTH,
    func_SWAPLENGTH,
    func_TRANSITIONWINDOW,
    func_amountRaised,
    func_buyTokens,
    func_creationTime,
    func_currentSwapRate,
    func_disableTokenSwapLock,
    func_getOwner,
    func_mintReserve,
    func_mintTokens,
    func_mlastDay,
    func_mspentToday,
    func_setTokenContract,
    func_startTokenSwap,
    func_stopToken,
    func_stopTokenSwap,
    func_tokenCap,
    func_tokenCtr,
    func_tokenSwap,
    func_transferStop,
    func_walletAddress,
    func_withdrawReserve,
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
    evt_EmergencyStopActivated,
    evt_TokenMint,
    evt_TokenSwapOver,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x88fcfbc22c6d3dbaa25af478c578978339bde77a
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"spender","type":"address"},{"name":"value","type":"uint256"}],"name":"approve","outputs":[{"name":"ok","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"supply","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"from","type":"address"},{"name":"to","type":"address"},{"name":"value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"ok","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"disableTokenSwapLock","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"currentSwapRate","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transferStop","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"walletAddress","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"who","type":"address"}],"name":"balanceOf","outputs":[{"name":"value","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"to","type":"address"},{"name":"value","type":"uint256"}],"name":"transfer","outputs":[{"name":"ok","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"beneficiary","type":"address"}],"name":"mintReserve","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"stopToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"creationTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenCap","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"owner","type":"address"},{"name":"spender","type":"address"}],"name":"allowance","outputs":[{"name":"_allowance","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"newTokenHolder","type":"address"},{"name":"etherAmount","type":"uint256"}],"name":"mintTokens","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"initial_balance","type":"uint256"},{"name":"wallet","type":"address"},{"name":"crowdsaleTime","type":"uint256"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"name":"newTokenHolder","type":"address"},{"indexed":false,"name":"amountOfTokens","type":"uint256"}],"name":"TokenMint","type":"event"},{"anonymous":false,"inputs":[],"name":"TokenSwapOver","type":"event"},{"anonymous":false,"inputs":[],"name":"EmergencyStopActivated","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xb1021477444c6566509e1b80d2c99e9603a31c47
[{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"removeOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"TRANSITION_WINDOW","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_addr","type":"address"}],"name":"isOwner","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"SWAP_LENGTH","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"stopTokenSwap","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"startTokenSwap","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"m_numOwners","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"m_lastDay","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_beneficiary","type":"address"}],"name":"withdrawReserve","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"resetSpentToday","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"m_spentToday","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"addOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"m_required","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_h","type":"bytes32"}],"name":"confirm","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"amountRaised","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"PRESALE_LENGTH","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenSwap","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"MAX_ETH","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenCtr","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newLimit","type":"uint256"}],"name":"setDailyLimit","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"},{"name":"_data","type":"bytes"}],"name":"execute","outputs":[{"name":"_r","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_operation","type":"bytes32"}],"name":"revoke","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newRequired","type":"uint256"}],"name":"changeRequirement","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"newTokenContractAddr","type":"address"}],"name":"setTokenContract","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_operation","type":"bytes32"},{"name":"_owner","type":"address"}],"name":"hasConfirmed","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"ownerIndex","type":"uint256"}],"name":"getOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"}],"name":"kill","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"stopToken","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_beneficiary","type":"address"}],"name":"buyTokens","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"}],"name":"changeOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"m_dailyLimit","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"_owners","type":"address[]"},{"name":"_required","type":"uint256"},{"name":"_daylimit","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"}],"name":"Confirmation","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"}],"name":"Revoke","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"oldOwner","type":"address"},{"indexed":false,"name":"newOwner","type":"address"}],"name":"OwnerChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"newOwner","type":"address"}],"name":"OwnerAdded","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"oldOwner","type":"address"}],"name":"OwnerRemoved","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"newRequirement","type":"uint256"}],"name":"RequirementChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_from","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"SingleTransact","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"MultiTransact","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"operation","type":"bytes32"},{"indexed":false,"name":"initiator","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"ConfirmationNeeded","type":"event"}]

*/
