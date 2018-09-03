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
 *
 * This monitor code is part of the 'status' monitor.
 */
#include "tokenlib.h"
#include "walletlib.h"
#include "articulate.h"

//-----------------------------------------------------------------------
static const string_q func_acceptOwnership = "0x79ba5097";
static const string_q func_activationTime = "0xda4493f6";
static const string_q func_balanceOfAt = "0x4ee2cd7e";
static const string_q func_changeController = "0x3cebb823";
static const string_q func_changeOwner = "0xa6f9dae1";
static const string_q func_claimTokens = "0xdf8de3e7";
static const string_q func_contribution = "0x50520b1f";
static const string_q func_controller = "0xf77c4791";
static const string_q func_createCloneToken = "0x6638c087";
static const string_q func_createCloneTokena1c9 = "0x5b7b72c1";
static const string_q func_creationBlock = "0x17634514";
static const string_q func_destroyTokens = "0xd3ce77fe";
static const string_q func_enableTransfers = "0xf41e60c5";
static const string_q func_generateTokens = "0x827f32c0";
static const string_q func_newOwner = "0xd4ee1d90";
static const string_q func_onApprove = "0xda682aeb";
static const string_q func_onTransfer = "0x4a393149";
static const string_q func_owner = "0x8da5cb5b";
static const string_q func_parentSnapShotBlock = "0xc5bcc4f1";
static const string_q func_parentToken = "0x80a54001";
static const string_q func_proxyPayment = "0xf48c3054";
static const string_q func_sgtExchanger = "0xad344bbe";
static const string_q func_snt = "0x060eb520";
static const string_q func_tokenFactory = "0xe77772fe";
static const string_q func_totalSupplyAt = "0x981b24d0";
static const string_q func_transfersEnabled = "0xbef97c87";

//-----------------------------------------------------------------------
bool articulateTransaction(CTransaction *p) {

    if (p->func)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (size_t i = 0 ; i < p->receipt.logs.size() ; i++)
        articulateEvent(&p->receipt.logs[i]);

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_acceptOwnership) {
            // function acceptOwnership()
            // 0x79ba5097
            p->func = new CFunction("acceptOwnership");
            return true;

        } else if (encoding == func_changeController) {
            // function changeController(address _newController)
            // 0x3cebb823
            p->func = new CFunction("changeController");
            p->func->inputs.push_back(CParameter("_newController", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeOwner) {
            // function changeOwner(address _newOwner)
            // 0xa6f9dae1
            p->func = new CFunction("changeOwner");
            p->func->inputs.push_back(CParameter("_newOwner", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_claimTokens) {
            // function claimTokens(address _token)
            // 0xdf8de3e7
            p->func = new CFunction("claimTokens");
            p->func->inputs.push_back(CParameter("_token", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_createCloneToken) {
            // function createCloneToken(string _cloneTokenName, uint8 _cloneDecimalUnits, string _cloneTokenSymbol, uint256 _snapshotBlock, bool _transfersEnabled)
            // 0x6638c087
            p->func = new CFunction("createCloneToken");
            p->func->inputs.push_back(CParameter("_cloneTokenName", "string", extract(params, 0*64, 64)));
            p->func->inputs.push_back(CParameter("_cloneDecimalUnits", "uint8", str_2_Uint(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_cloneTokenSymbol", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("_snapshotBlock", "uint256", str_2_Wei("0x" + extract(params, 3*64, 64))));
            p->func->inputs.push_back(CParameter("_transfersEnabled", "bool", str_2_Int(extract(params, 4*64, 64))));
            return true;

        } else if (encoding == func_createCloneTokena1c9) {
            // function createCloneToken(address _parentToken, uint256 _snapshotBlock, string _tokenName, uint8 _decimalUnits, string _tokenSymbol, bool _transfersEnabled)
            // 0x5b7b72c1
            p->func = new CFunction("createCloneTokena1c9");
            p->func->inputs.push_back(CParameter("_parentToken", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("_snapshotBlock", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_tokenName", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("_decimalUnits", "uint8", str_2_Uint(extract(params, 3*64, 64))));
            p->func->inputs.push_back(CParameter("_tokenSymbol", "string", extract(params, 4*64, 64)));
            p->func->inputs.push_back(CParameter("_transfersEnabled", "bool", str_2_Int(extract(params, 5*64, 64))));
            return true;

        } else if (encoding == func_destroyTokens) {
            // function destroyTokens(address _owner, uint256 _amount)
            // 0xd3ce77fe
            p->func = new CFunction("destroyTokens");
            p->func->inputs.push_back(CParameter("_owner", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("_amount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_enableTransfers) {
            // function enableTransfers(bool _transfersEnabled)
            // 0xf41e60c5
            p->func = new CFunction("enableTransfers");
            p->func->inputs.push_back(CParameter("_transfersEnabled", "bool", str_2_Int(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_generateTokens) {
            // function generateTokens(address _owner, uint256 _amount)
            // 0x827f32c0
            p->func = new CFunction("generateTokens");
            p->func->inputs.push_back(CParameter("_owner", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("_amount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_onApprove) {
            // function onApprove(address _from, address param_0, uint256 param_1)
            // 0xda682aeb
            p->func = new CFunction("onApprove");
            p->func->inputs.push_back(CParameter("_from", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("param_0", "address", str_2_Addr(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("param_1", "uint256", str_2_Wei("0x" + extract(params, 2*64, 64))));
            return true;

        } else if (encoding == func_onTransfer) {
            // function onTransfer(address _from, address param_0, uint256 param_1)
            // 0x4a393149
            p->func = new CFunction("onTransfer");
            p->func->inputs.push_back(CParameter("_from", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("param_0", "address", str_2_Addr(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("param_1", "uint256", str_2_Wei("0x" + extract(params, 2*64, 64))));
            return true;

        } else if (encoding == func_proxyPayment) {
            // function proxyPayment(address param_0)
            // 0xf48c3054
            p->func = new CFunction("proxyPayment");
            p->func->inputs.push_back(CParameter("param_0", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return articulateToken(p);
}

//-----------------------------------------------------------------------
static const string_q evt_ClaimedTokens = "0xf931edb47c50b4b4104c187b5814a9aef5f709e17e2ecf9617e860cacade929c";
static const string_q evt_ControllerChanged = "0x027c3e080ed9215f564a9455a666f7e459b3edc0bb6e02a1bf842fde4d0ccfc1";
static const string_q evt_NewCloneToken = "0x086c875b377f900b07ce03575813022f05dd10ed7640b5282cf6d3c3fc352ade";

//-----------------------------------------------------------------------
bool articulateEvent(CLogEntry *p) {

    if (p->func)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_ClaimedTokens) {
            // event ClaimedTokens(address indexed _token, address indexed _controller, uint256 _amount)
            // 0xf931edb47c50b4b4104c187b5814a9aef5f709e17e2ecf9617e860cacade929c
            p->func = new CFunction("ClaimedTokensEvent");
            p->func->inputs.push_back(CParameter("_token", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_controller", "address", str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "")));
            p->func->inputs.push_back(CParameter("_amount", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ControllerChanged) {
            // event ControllerChanged(address indexed _newController)
            // 0x027c3e080ed9215f564a9455a666f7e459b3edc0bb6e02a1bf842fde4d0ccfc1
            p->func = new CFunction("ControllerChangedEvent");
            p->func->inputs.push_back(CParameter("_newController", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_NewCloneToken) {
            // event NewCloneToken(address indexed _cloneToken, uint256 _snapshotBlock)
            // 0x086c875b377f900b07ce03575813022f05dd10ed7640b5282cf6d3c3fc352ade
            p->func = new CFunction("NewCloneTokenEvent");
            p->func->inputs.push_back(CParameter("_cloneToken", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_snapshotBlock", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return articulateTokenEvent(p);
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
    func_acceptOwnership,
    func_activationTime,
    func_balanceOfAt,
    func_changeController,
    func_changeOwner,
    func_claimTokens,
    func_contribution,
    func_controller,
    func_createCloneToken,
    func_createCloneTokena1c9,
    func_creationBlock,
    func_destroyTokens,
    func_enableTransfers,
    func_generateTokens,
    func_newOwner,
    func_onApprove,
    func_onTransfer,
    func_owner,
    func_parentSnapShotBlock,
    func_parentToken,
    func_proxyPayment,
    func_sgtExchanger,
    func_snt,
    func_tokenFactory,
    func_totalSupplyAt,
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
    evt_ClaimedTokens,
    evt_ControllerChanged,
    evt_NewCloneToken,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x744d70fdbe2ba4cf95131626614a1763df805b9e
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"creationBlock","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newController","type":"address"}],"name":"changeController","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_blockNumber","type":"uint256"}],"name":"balanceOfAt","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_cloneTokenName","type":"string"},{"name":"_cloneDecimalUnits","type":"uint8"},{"name":"_cloneTokenSymbol","type":"string"},{"name":"_snapshotBlock","type":"uint256"},{"name":"_transfersEnabled","type":"bool"}],"name":"createCloneToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"parentToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_amount","type":"uint256"}],"name":"generateTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_blockNumber","type":"uint256"}],"name":"totalSupplyAt","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transfersEnabled","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"parentSnapShotBlock","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"},{"name":"_amount","type":"uint256"}],"name":"destroyTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"}],"name":"claimTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"tokenFactory","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_transfersEnabled","type":"bool"}],"name":"enableTransfers","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"controller","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"inputs":[{"name":"_tokenFactory","type":"address"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_token","type":"address"},{"indexed":true,"name":"_controller","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"ClaimedTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_cloneToken","type":"address"},{"indexed":false,"name":"_snapshotBlock","type":"uint256"}],"name":"NewCloneToken","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xa1c957c0210397d2d0296341627b74411756d476
[{"constant":false,"inputs":[{"name":"_parentToken","type":"address"},{"name":"_snapshotBlock","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"},{"name":"_transfersEnabled","type":"bool"}],"name":"createCloneToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"}]

ABI for addr : 0x52ae2b53c847327f95a5084a7c38c0adb12fd302
[{"constant":true,"inputs":[],"name":"snt","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newController","type":"address"}],"name":"changeController","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"","type":"address"},{"name":"","type":"uint256"}],"name":"onTransfer","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"contribution","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"changeOwner","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"sgtExchanger","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"activationTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"","type":"address"},{"name":"","type":"uint256"}],"name":"onApprove","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"}],"name":"claimTokens","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"","type":"address"}],"name":"proxyPayment","outputs":[{"name":"","type":"bool"}],"payable":true,"type":"function"},{"inputs":[{"name":"_owner","type":"address"},{"name":"_snt","type":"address"},{"name":"_contribution","type":"address"},{"name":"_sgtExchanger","type":"address"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_token","type":"address"},{"indexed":true,"name":"_controller","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"ClaimedTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_newController","type":"address"}],"name":"ControllerChanged","type":"event"}]

*/
