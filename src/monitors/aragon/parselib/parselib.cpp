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
    CNewCloneTokenEvent::registerClass();
    CNewTokenGrantEvent::registerClass();
    CChangeController::registerClass();
    CChangeVestingWhitelister::registerClass();
    CCreateCloneToken::registerClass();
    CDestroyTokens::registerClass();
    CEnableTransfers::registerClass();
    CGenerateTokens::registerClass();
    CGrantVestedTokens::registerClass();
    COnApprove::registerClass();
    COnTransfer::registerClass();
    CProxyPayment::registerClass();
    CRevokeTokenGrant::registerClass();
    CSetCanCreateGrants::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_balanceOfAt = "0x4ee2cd7e";
const string_q func_changeController = "0x3cebb823";
const string_q func_changeVestingWhitelister = "0xfb13a707";
const string_q func_controller = "0xf77c4791";
const string_q func_createCloneToken = "0x6638c087";
const string_q func_creationBlock = "0x17634514";
const string_q func_destroyTokens = "0xd3ce77fe";
const string_q func_enableTransfers = "0xf41e60c5";
const string_q func_generateTokens = "0x827f32c0";
const string_q func_grantVestedTokens = "0x00e1986d";
const string_q func_grants = "0x2c71e60a";
const string_q func_lastTokenIsTransferableDate = "0x6c182e99";
const string_q func_onApprove = "0xda682aeb";
const string_q func_onTransfer = "0x4a393149";
const string_q func_parentSnapShotBlock = "0xc5bcc4f1";
const string_q func_parentToken = "0x80a54001";
const string_q func_proxyPayment = "0xf48c3054";
const string_q func_revokeTokenGrant = "0xeb944e4c";
const string_q func_sale = "0x6ad1fe02";
const string_q func_setCanCreateGrants = "0x1f9a391c";
const string_q func_spendableBalanceOf = "0x0f8f8b83";
const string_q func_token = "0xfc0c546a";
const string_q func_tokenFactory = "0xe77772fe";
const string_q func_tokenGrant = "0x600e85b7";
const string_q func_tokenGrantsCount = "0x02a72a4c";
const string_q func_totalSupplyAt = "0x981b24d0";
const string_q func_transferableTokens = "0xd347c205";
const string_q func_transfersEnabled = "0xbef97c87";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_changeController) {
            // function changeController(address _newController)
            // 0x3cebb823
            CChangeController *a = new CChangeController;
            a->CTransaction::operator=(*p);
            a->_newController = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeController", params, nItems, items);
            return a;

        } else if (encoding == func_changeVestingWhitelister) {
            // function changeVestingWhitelister(address _newWhitelister)
            // 0xfb13a707
            CChangeVestingWhitelister *a = new CChangeVestingWhitelister;
            a->CTransaction::operator=(*p);
            a->_newWhitelister = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeVestingWhitelister", params, nItems, items);
            return a;

        } else if (encoding == func_createCloneToken) {
            // function createCloneToken(string _cloneTokenName, uint8 _cloneDecimalUnits, string _cloneTokenSymbol, uint256 _snapshotBlock, bool _transfersEnabled)
            // 0x6638c087
            CCreateCloneToken *a = new CCreateCloneToken;
            a->CTransaction::operator=(*p);
            a->_cloneTokenName = extract(params, 0*64, 64);
            a->_cloneDecimalUnits = (uint32_t)str_2_Uint(extract(params, 1*64, 64));
            a->_cloneTokenSymbol = extract(params, 2*64, 64);
            a->_snapshotBlock = str_2_Wei("0x" + extract(params, 3*64, 64));
            a->_transfersEnabled = str_2_Int(extract(params, 4*64, 64));
            items[nItems++] = "string";
            items[nItems++] = "uint8";
            items[nItems++] = "string";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("createCloneToken", params, nItems, items);
            return a;

        } else if (encoding == func_destroyTokens) {
            // function destroyTokens(address _owner, uint256 _amount)
            // 0xd3ce77fe
            CDestroyTokens *a = new CDestroyTokens;
            a->CTransaction::operator=(*p);
            a->_owner = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("destroyTokens", params, nItems, items);
            return a;

        } else if (encoding == func_enableTransfers) {
            // function enableTransfers(bool _transfersEnabled)
            // 0xf41e60c5
            CEnableTransfers *a = new CEnableTransfers;
            a->CTransaction::operator=(*p);
            a->_transfersEnabled = str_2_Int(extract(params, 0*64, 64));
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("enableTransfers", params, nItems, items);
            return a;

        } else if (encoding == func_generateTokens) {
            // function generateTokens(address _owner, uint256 _amount)
            // 0x827f32c0
            CGenerateTokens *a = new CGenerateTokens;
            a->CTransaction::operator=(*p);
            a->_owner = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("generateTokens", params, nItems, items);
            return a;

        } else if (encoding == func_grantVestedTokens) {
            // function grantVestedTokens(address _to, uint256 _value, uint64 _start, uint64 _cliff, uint64 _vesting)
            // 0x00e1986d
            CGrantVestedTokens *a = new CGrantVestedTokens;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_start = str_2_Uint(extract(params, 2*64, 64));
            a->_cliff = str_2_Uint(extract(params, 3*64, 64));
            a->_vesting = str_2_Uint(extract(params, 4*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint64";
            items[nItems++] = "uint64";
            items[nItems++] = "uint64";
            a->articulatedTx = decodeRLP("grantVestedTokens", params, nItems, items);
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

        } else if (encoding == func_proxyPayment) {
            // function proxyPayment(address _owner)
            // 0xf48c3054
            CProxyPayment *a = new CProxyPayment;
            a->CTransaction::operator=(*p);
            a->_owner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("proxyPayment", params, nItems, items);
            return a;

        } else if (encoding == func_revokeTokenGrant) {
            // function revokeTokenGrant(address _holder, uint256 _grantId)
            // 0xeb944e4c
            CRevokeTokenGrant *a = new CRevokeTokenGrant;
            a->CTransaction::operator=(*p);
            a->_holder = str_2_Addr(extract(params, 0*64, 64));
            a->_grantId = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("revokeTokenGrant", params, nItems, items);
            return a;

        } else if (encoding == func_setCanCreateGrants) {
            // function setCanCreateGrants(address _addr, bool _allowed)
            // 0x1f9a391c
            CSetCanCreateGrants *a = new CSetCanCreateGrants;
            a->CTransaction::operator=(*p);
            a->_addr = str_2_Addr(extract(params, 0*64, 64));
            a->_allowed = str_2_Int(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("setCanCreateGrants", params, nItems, items);
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
const string_q evt_NewCloneToken = "0x086c875b377f900b07ce03575813022f05dd10ed7640b5282cf6d3c3fc352ade";
const string_q evt_NewTokenGrant = "0x9e12d725ade130ef3f3727e13815b3fcf01a631419ce8142bafb0752a61121e8";

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

        if (topic_2_Str(p->topics[0]) % evt_NewCloneToken) {
            // event NewCloneToken(address indexed _cloneToken, uint256 _snapshotBlock)
            // 0x086c875b377f900b07ce03575813022f05dd10ed7640b5282cf6d3c3fc352ade
            CNewCloneTokenEvent *a = new CNewCloneTokenEvent;
            a->CLogEntry::operator=(*p);
            a->_cloneToken = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_snapshotBlock = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("NewCloneToken", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewTokenGrant) {
            // event NewTokenGrant(address indexed from, address indexed to, uint256 value, uint64 start, uint64 cliff, uint64 vesting)
            // 0x9e12d725ade130ef3f3727e13815b3fcf01a631419ce8142bafb0752a61121e8
            CNewTokenGrantEvent *a = new CNewTokenGrantEvent;
            a->CLogEntry::operator=(*p);
            a->from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->start = str_2_Uint("0x" + extract(data, 1*64, 64));
            a->cliff = str_2_Uint("0x" + extract(data, 2*64, 64));
            a->vesting = str_2_Uint("0x" + extract(data, 3*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint64";
            items[nItems++] = "uint64";
            items[nItems++] = "uint64";
            a->articulatedLog = decodeRLP("NewTokenGrant", params, nItems, items);
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
    func_balanceOfAt,
    func_changeController,
    func_changeVestingWhitelister,
    func_controller,
    func_createCloneToken,
    func_creationBlock,
    func_destroyTokens,
    func_enableTransfers,
    func_generateTokens,
    func_grantVestedTokens,
    func_grants,
    func_lastTokenIsTransferableDate,
    func_onApprove,
    func_onTransfer,
    func_parentSnapShotBlock,
    func_parentToken,
    func_proxyPayment,
    func_revokeTokenGrant,
    func_sale,
    func_setCanCreateGrants,
    func_spendableBalanceOf,
    func_token,
    func_tokenFactory,
    func_tokenGrant,
    func_tokenGrantsCount,
    func_totalSupplyAt,
    func_transferableTokens,
    func_transfersEnabled,
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
    evt_NewCloneToken,
    evt_NewTokenGrant,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x960b236a07cf122663c4303350609a66a7b288c0
[{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"},{"name":"_start","type":"uint64"},{"name":"_cliff","type":"uint64"},{"name":"_vesting","type":"uint64"}],"name":"grantVestedTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_holder","type":"address"}],"name":"tokenGrantsCount","outputs":[{"name":"index","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_holder","type":"address"}],"name":"spendableBalanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"creationBlock","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_addr","type":"address"},{"name":"_allowed","type":"bool"}],"name":"setCanCreateGrants","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"uint256"}],"name":"grants","outputs":[{"name":"granter","type":"address"},{"name":"value","type":"uint256"},{"name":"cliff","type":"uint64"},{"name":"vesting","type":"uint64"},{"name":"start","type":"uint64"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newController","type":"address"}],"name":"changeController","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_blockNumber","type":"uint256"}],"name":"balanceOfAt","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_holder","type":"address"},{"name":"_grantId","type":"uint256"}],"name":"tokenGrant","outputs":[{"name":"granter","type":"address"},{"name":"value","type":"uint256"},{"name":"vested","type":"uint256"},{"name":"start","type":"uint64"},{"name":"cliff","type":"uint64"},{"name":"vesting","type":"uint64"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_cloneTokenName","type":"string"},{"name":"_cloneDecimalUnits","type":"uint8"},{"name":"_cloneTokenSymbol","type":"string"},{"name":"_snapshotBlock","type":"uint256"},{"name":"_transfersEnabled","type":"bool"}],"name":"createCloneToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"holder","type":"address"}],"name":"lastTokenIsTransferableDate","outputs":[{"name":"date","type":"uint64"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"parentToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_amount","type":"uint256"}],"name":"generateTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_blockNumber","type":"uint256"}],"name":"totalSupplyAt","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transfersEnabled","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"parentSnapShotBlock","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"holder","type":"address"},{"name":"time","type":"uint64"}],"name":"transferableTokens","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_amount","type":"uint256"}],"name":"destroyTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenFactory","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_holder","type":"address"},{"name":"_grantId","type":"uint256"}],"name":"revokeTokenGrant","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_transfersEnabled","type":"bool"}],"name":"enableTransfers","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"controller","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newWhitelister","type":"address"}],"name":"changeVestingWhitelister","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_tokenFactory","type":"address"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"start","type":"uint64"},{"indexed":false,"name":"cliff","type":"uint64"},{"indexed":false,"name":"vesting","type":"uint64"}],"name":"NewTokenGrant","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_cloneToken","type":"address"},{"indexed":false,"name":"_snapshotBlock","type":"uint256"}],"name":"NewCloneToken","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0x175b5d76b0eaa2c66bee02c393f96d6cc05e7ff9
{"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

ABI for addr : 0xd39902f046b5885d70e9e66594b65f84d4d1c952
[{"constant":false,"inputs":[{"name":"network","type":"address"}],"name":"changeController","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"onTransfer","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"sale","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"onApprove","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"proxyPayment","outputs":[{"name":"","type":"bool"}],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"token","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"inputs":[{"name":"_sale","type":"address"},{"name":"_ant","type":"address"}],"payable":false,"type":"constructor"}]

*/
