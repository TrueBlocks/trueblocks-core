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
    CAllocateBountyAndEcosystemTokensEvent::registerClass();
    CAllocateFounderTokensEvent::registerClass();
    CBuyEvent::registerClass();
    CWithdrawEvent::registerClass();
    CAllocateBountyAndEcosystemTokens::registerClass();
    CAllocateFounderTokens::registerClass();
    CBuy::registerClass();
    CBuyRecipient::registerClass();
    CChangeFounder::registerClass();
    CHalt::registerClass();
    CUnhalt::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_allocateBountyAndEcosystemTokens = "0x4a8b5389";
const string_q func_allocateFounderTokens = "0xbcfcb03e";
const string_q func_bountyAllocated = "0x1bc59aa6";
const string_q func_bountyAllocation = "0xf6b9d05d";
const string_q func_buy = "0xe5fe4f31";
const string_q func_buyRecipient = "0xdda44b10";
const string_q func_changeFounder = "0x93c32e06";
const string_q func_ecosystemAllocated = "0x771d9d05";
const string_q func_ecosystemAllocation = "0xc0f496ac";
const string_q func_endBlock = "0x083c6323";
const string_q func_etherCap = "0x7228b9db";
const string_q func_founder = "0x4d853ee5";
const string_q func_founderAllocated = "0x99d22e48";
const string_q func_founderAllocation = "0x824338bd";
const string_q func_founderLockup = "0x83e811a6";
const string_q func_halt = "0x5ed7ca5b";
const string_q func_halted = "0xb9b8af0b";
const string_q func_presaleEtherRaised = "0x44b49958";
const string_q func_presaleTokenSupply = "0xc4fc3a3b";
const string_q func_price = "0xa035b1fe";
const string_q func_signer = "0x238ac933";
const string_q func_startBlock = "0x48cd4cb1";
const string_q func_testPrice = "0x2af7ceff";
const string_q func_transferLockup = "0xc24fe21b";
const string_q func_unhalt = "0xcb3e64fd";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_allocateBountyAndEcosystemTokens) {
            // function allocateBountyAndEcosystemTokens()
            // 0x4a8b5389
            CAllocateBountyAndEcosystemTokens *a = new CAllocateBountyAndEcosystemTokens;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("allocateBountyAndEcosystemTokens", params, nItems, items);
            return a;

        } else if (encoding == func_allocateFounderTokens) {
            // function allocateFounderTokens()
            // 0xbcfcb03e
            CAllocateFounderTokens *a = new CAllocateFounderTokens;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("allocateFounderTokens", params, nItems, items);
            return a;

        } else if (encoding == func_buy) {
            // function buy(uint8 v, bytes32 r, bytes32 s)
            // 0xe5fe4f31
            CBuy *a = new CBuy;
            a->CTransaction::operator=(*p);
            a->v = (uint32_t)str_2_Uint(extract(params, 0*64, 64));
            a->r = extract(params, 1*64, 64);
            a->s = extract(params, 2*64, 64);
            items[nItems++] = "uint8";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("buy", params, nItems, items);
            return a;

        } else if (encoding == func_buyRecipient) {
            // function buyRecipient(address recipient, uint8 v, bytes32 r, bytes32 s)
            // 0xdda44b10
            CBuyRecipient *a = new CBuyRecipient;
            a->CTransaction::operator=(*p);
            a->recipient = str_2_Addr(extract(params, 0*64, 64));
            a->v = (uint32_t)str_2_Uint(extract(params, 1*64, 64));
            a->r = extract(params, 2*64, 64);
            a->s = extract(params, 3*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("buyRecipient", params, nItems, items);
            return a;

        } else if (encoding == func_changeFounder) {
            // function changeFounder(address newFounder)
            // 0x93c32e06
            CChangeFounder *a = new CChangeFounder;
            a->CTransaction::operator=(*p);
            a->newFounder = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeFounder", params, nItems, items);
            return a;

        } else if (encoding == func_halt) {
            // function halt()
            // 0x5ed7ca5b
            CHalt *a = new CHalt;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("halt", params, nItems, items);
            return a;

        } else if (encoding == func_unhalt) {
            // function unhalt()
            // 0xcb3e64fd
            CUnhalt *a = new CUnhalt;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("unhalt", params, nItems, items);
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
const string_q evt_AllocateBountyAndEcosystemTokens = "0xad156d34337c07db4f9b140cea03fb74d167641e252f85c85ccf3b47f36c9c49";
const string_q evt_AllocateFounderTokens = "0x1c8a3821e439862f28669f7fbc210e74e947ebf0e947b591cc5a02e410030265";
const string_q evt_Buy = "0x1cbc5ab135991bd2b6a4b034a04aa2aa086dac1371cb9b16b8b5e2ed6b036bed";
const string_q evt_Withdraw = "0x9b1bfa7fa9ee420a16e124f794c35ac9f90472acc99140eb2f6447c714cad8eb";

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

        if (topic_2_Str(p->topics[0]) % evt_AllocateBountyAndEcosystemTokens) {
            // event AllocateBountyAndEcosystemTokens(address indexed sender)
            // 0xad156d34337c07db4f9b140cea03fb74d167641e252f85c85ccf3b47f36c9c49
            CAllocateBountyAndEcosystemTokensEvent *a = new CAllocateBountyAndEcosystemTokensEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("AllocateBountyAndEcosystemTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_AllocateFounderTokens) {
            // event AllocateFounderTokens(address indexed sender)
            // 0x1c8a3821e439862f28669f7fbc210e74e947ebf0e947b591cc5a02e410030265
            CAllocateFounderTokensEvent *a = new CAllocateFounderTokensEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("AllocateFounderTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Buy) {
            // event Buy(address indexed sender, uint256 eth, uint256 fbt)
            // 0x1cbc5ab135991bd2b6a4b034a04aa2aa086dac1371cb9b16b8b5e2ed6b036bed
            CBuyEvent *a = new CBuyEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->eth = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->fbt = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Buy", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Withdraw) {
            // event Withdraw(address indexed sender, address to, uint256 eth)
            // 0x9b1bfa7fa9ee420a16e124f794c35ac9f90472acc99140eb2f6447c714cad8eb
            CWithdrawEvent *a = new CWithdrawEvent;
            a->CLogEntry::operator=(*p);
            a->sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->to = str_2_Addr(extract(data, 0*64, 64));
            a->eth = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Withdraw", params, nItems, items);
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
    func_allocateBountyAndEcosystemTokens,
    func_allocateFounderTokens,
    func_bountyAllocated,
    func_bountyAllocation,
    func_buy,
    func_buyRecipient,
    func_changeFounder,
    func_ecosystemAllocated,
    func_ecosystemAllocation,
    func_endBlock,
    func_etherCap,
    func_founder,
    func_founderAllocated,
    func_founderAllocation,
    func_founderLockup,
    func_halt,
    func_halted,
    func_presaleEtherRaised,
    func_presaleTokenSupply,
    func_price,
    func_signer,
    func_startBlock,
    func_testPrice,
    func_transferLockup,
    func_unhalt,
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
    evt_AllocateBountyAndEcosystemTokens,
    evt_AllocateFounderTokens,
    evt_Buy,
    evt_Withdraw,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xaf30d2a7e90d7dc361c8c4585e9bb7d2f6f15bc7
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":true,"inputs":[],"name":"endBlock","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"bountyAllocated","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"signer","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"blockNumber","type":"uint256"}],"name":"testPrice","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"presaleEtherRaised","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"startBlock","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"allocateBountyAndEcosystemTokens","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"founder","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[],"name":"halt","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"etherCap","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"ecosystemAllocated","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"founderAllocation","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"founderLockup","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"newFounder","type":"address"}],"name":"changeFounder","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":true,"inputs":[],"name":"founderAllocated","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"price","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"halted","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"allocateFounderTokens","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"ecosystemAllocation","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"transferLockup","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"presaleTokenSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"unhalt","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"recipient","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"buyRecipient","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"buy","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"bountyAllocation","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"inputs":[{"name":"founderInput","type":"address"},{"name":"signerInput","type":"address"},{"name":"startBlockInput","type":"uint256"},{"name":"endBlockInput","type":"uint256"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"eth","type":"uint256"},{"indexed":false,"name":"fbt","type":"uint256"}],"name":"Buy","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"eth","type":"uint256"}],"name":"Withdraw","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"}],"name":"AllocateFounderTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"sender","type":"address"}],"name":"AllocateBountyAndEcosystemTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0x3286652eacf7abf27383dd1a9218ba9d8169d15e
[]

ABI for addr : 0xa5384627f6dcd3440298e2d8b0da9d5f0fcbcef7
[]

*/
