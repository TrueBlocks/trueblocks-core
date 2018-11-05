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
    CLogNoteEvent::registerClass();
    CLogSetAuthorityEvent::registerClass();
    CLogSetOwnerEvent::registerClass();
    CMintEvent::registerClass();
    CApprove::registerClass();
    CBurn::registerClass();
    CBurn9f8f::registerClass();
    CMint::registerClass();
    CMint9f8f::registerClass();
    CMove::registerClass();
    CPull::registerClass();
    CPush::registerClass();
    CSetAuthority::registerClass();
    CSetName::registerClass();
    CSetOwner::registerClass();
    CStart::registerClass();
    CStop::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_approve = "0xdaea85c5";
const string_q func_authority = "0xbf7e214f";
const string_q func_burn = "0x42966c68";
const string_q func_burn9f8f = "0x9dc29fac";
const string_q func_mint = "0x40c10f19";
const string_q func_mint9f8f = "0xa0712d68";
const string_q func_move = "0xbb35783b";
const string_q func_owner = "0x8da5cb5b";
const string_q func_pull = "0xf2d5d56b";
const string_q func_push = "0xb753a98c";
const string_q func_setAuthority = "0x7a9e5e4b";
const string_q func_setName = "0x5ac801fe";
const string_q func_setOwner = "0x13af4035";
const string_q func_start = "0xbe9a6555";
const string_q func_stop = "0x07da68f5";
const string_q func_stopped = "0x75f12b21";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_approve) {
            // function approve(address guy)
            // 0xdaea85c5
            CApprove *a = new CApprove;
            a->CTransaction::operator=(*p);
            a->guy = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("approve", params, nItems, items);
            return a;

        } else if (encoding == func_burn) {
            // function burn(uint256 wad)
            // 0x42966c68
            CBurn *a = new CBurn;
            a->CTransaction::operator=(*p);
            a->wad = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("burn", params, nItems, items);
            return a;

        } else if (encoding == func_burn9f8f) {
            // function burn(address guy, uint256 wad)
            // 0x9dc29fac
            CBurn9f8f *a = new CBurn9f8f;
            a->CTransaction::operator=(*p);
            a->guy = str_2_Addr(extract(params, 0*64, 64));
            a->wad = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("burn9f8f", params, nItems, items);
            return a;

        } else if (encoding == func_mint) {
            // function mint(address guy, uint256 wad)
            // 0x40c10f19
            CMint *a = new CMint;
            a->CTransaction::operator=(*p);
            a->guy = str_2_Addr(extract(params, 0*64, 64));
            a->wad = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("mint", params, nItems, items);
            return a;

        } else if (encoding == func_mint9f8f) {
            // function mint(uint256 wad)
            // 0xa0712d68
            CMint9f8f *a = new CMint9f8f;
            a->CTransaction::operator=(*p);
            a->wad = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("mint9f8f", params, nItems, items);
            return a;

        } else if (encoding == func_move) {
            // function move(address src, address dst, uint256 wad)
            // 0xbb35783b
            CMove *a = new CMove;
            a->CTransaction::operator=(*p);
            a->src = str_2_Addr(extract(params, 0*64, 64));
            a->dst = str_2_Addr(extract(params, 1*64, 64));
            a->wad = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("move", params, nItems, items);
            return a;

        } else if (encoding == func_pull) {
            // function pull(address src, uint256 wad)
            // 0xf2d5d56b
            CPull *a = new CPull;
            a->CTransaction::operator=(*p);
            a->src = str_2_Addr(extract(params, 0*64, 64));
            a->wad = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("pull", params, nItems, items);
            return a;

        } else if (encoding == func_push) {
            // function push(address dst, uint256 wad)
            // 0xb753a98c
            CPush *a = new CPush;
            a->CTransaction::operator=(*p);
            a->dst = str_2_Addr(extract(params, 0*64, 64));
            a->wad = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("push", params, nItems, items);
            return a;

        } else if (encoding == func_setAuthority) {
            // function setAuthority(address authority_)
            // 0x7a9e5e4b
            CSetAuthority *a = new CSetAuthority;
            a->CTransaction::operator=(*p);
            a->authority_ = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setAuthority", params, nItems, items);
            return a;

        } else if (encoding == func_setName) {
            // function setName(bytes32 name_)
            // 0x5ac801fe
            CSetName *a = new CSetName;
            a->CTransaction::operator=(*p);
            a->name_ = extract(params, 0*64, 64);
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("setName", params, nItems, items);
            return a;

        } else if (encoding == func_setOwner) {
            // function setOwner(address owner_)
            // 0x13af4035
            CSetOwner *a = new CSetOwner;
            a->CTransaction::operator=(*p);
            a->owner_ = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setOwner", params, nItems, items);
            return a;

        } else if (encoding == func_start) {
            // function start()
            // 0xbe9a6555
            CStart *a = new CStart;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("start", params, nItems, items);
            return a;

        } else if (encoding == func_stop) {
            // function stop()
            // 0x07da68f5
            CStop *a = new CStop;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("stop", params, nItems, items);
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
const string_q evt_LogNote = "0x644843f351d3fba4abcd60109eaff9f54bac8fb8ccf0bab941009c21df21cf31";
const string_q evt_LogSetAuthority = "0x1abebea81bfa2637f28358c371278fb15ede7ea8dd28d2e03b112ff6d936ada4";
const string_q evt_LogSetOwner = "0xce241d7ca1f669fee44b6fc00b8eba2df3bb514eed0f6f668f8f89096e81ed94";
const string_q evt_Mint = "0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885";

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
            // event Burn(address indexed guy, uint256 wad)
            // 0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5
            CBurnEvent *a = new CBurnEvent;
            a->CLogEntry::operator=(*p);
            a->guy = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->wad = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Burn", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_LogNote) {
            // event LogNote(bytes4 indexed sig, address indexed guy, bytes32 indexed foo, bytes32 indexed bar, uint256 wad, bytes fax)
            // 0x644843f351d3fba4abcd60109eaff9f54bac8fb8ccf0bab941009c21df21cf31
            CLogNoteEvent *a = new CLogNoteEvent;
            a->CLogEntry::operator=(*p);
            a->sig = nTops > 1 ? topic_2_Str(p->topics[1]) : "";
            a->guy = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->foo = nTops > 3 ? topic_2_Str(p->topics[3]) : "";
            a->bar = nTops > 4 ? topic_2_Str(p->topics[4]) : "";
            a->wad = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->fax = "0x" + extract(data, 1*64);
            items[nItems++] = "bytes4";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->articulatedLog = decodeRLP("LogNote", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_LogSetAuthority) {
            // event LogSetAuthority(address indexed authority)
            // 0x1abebea81bfa2637f28358c371278fb15ede7ea8dd28d2e03b112ff6d936ada4
            CLogSetAuthorityEvent *a = new CLogSetAuthorityEvent;
            a->CLogEntry::operator=(*p);
            a->authority = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("LogSetAuthority", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_LogSetOwner) {
            // event LogSetOwner(address indexed owner)
            // 0xce241d7ca1f669fee44b6fc00b8eba2df3bb514eed0f6f668f8f89096e81ed94
            CLogSetOwnerEvent *a = new CLogSetOwnerEvent;
            a->CLogEntry::operator=(*p);
            a->owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("LogSetOwner", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Mint) {
            // event Mint(address indexed guy, uint256 wad)
            // 0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885
            CMintEvent *a = new CMintEvent;
            a->CLogEntry::operator=(*p);
            a->guy = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->wad = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Mint", params, nItems, items);
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
    func_approve,
    func_authority,
    func_burn,
    func_burn9f8f,
    func_mint,
    func_mint9f8f,
    func_move,
    func_owner,
    func_pull,
    func_push,
    func_setAuthority,
    func_setName,
    func_setOwner,
    func_start,
    func_stop,
    func_stopped,
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
    evt_LogNote,
    evt_LogSetAuthority,
    evt_LogSetOwner,
    evt_Mint,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x9f8f72aa9304c8b593d555f12ef6589cc3a579a2
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"stop","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"guy","type":"address"},{"name":"wad","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"owner_","type":"address"}],"name":"setOwner","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"src","type":"address"},{"name":"dst","type":"address"},{"name":"wad","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"guy","type":"address"},{"name":"wad","type":"uint256"}],"name":"mint","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"wad","type":"uint256"}],"name":"burn","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"name_","type":"bytes32"}],"name":"setName","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"src","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"stopped","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"authority_","type":"address"}],"name":"setAuthority","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"guy","type":"address"},{"name":"wad","type":"uint256"}],"name":"burn","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"wad","type":"uint256"}],"name":"mint","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"dst","type":"address"},{"name":"wad","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"dst","type":"address"},{"name":"wad","type":"uint256"}],"name":"push","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"src","type":"address"},{"name":"dst","type":"address"},{"name":"wad","type":"uint256"}],"name":"move","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"start","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"authority","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"guy","type":"address"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"src","type":"address"},{"name":"guy","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"src","type":"address"},{"name":"wad","type":"uint256"}],"name":"pull","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[{"name":"symbol_","type":"bytes32"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"guy","type":"address"},{"indexed":false,"name":"wad","type":"uint256"}],"name":"Mint","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"guy","type":"address"},{"indexed":false,"name":"wad","type":"uint256"}],"name":"Burn","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"authority","type":"address"}],"name":"LogSetAuthority","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"}],"name":"LogSetOwner","type":"event"},{"anonymous":true,"inputs":[{"indexed":true,"name":"sig","type":"bytes4"},{"indexed":true,"name":"guy","type":"address"},{"indexed":true,"name":"foo","type":"bytes32"},{"indexed":true,"name":"bar","type":"bytes32"},{"indexed":false,"name":"wad","type":"uint256"},{"indexed":false,"name":"fax","type":"bytes"}],"name":"LogNote","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0x7bb0b08587b8a6b8945e09f1baca426558b0f06a
[]

*/
