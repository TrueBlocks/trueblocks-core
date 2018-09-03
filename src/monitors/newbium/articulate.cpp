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
 * This monitor code is part of the 'newbium' monitor.
 */
#include "tokenlib.h"
#include "walletlib.h"
#include "articulate.h"

//-----------------------------------------------------------------------
static const string_q func_accountLevelsAddr = "0xf3412942";
static const string_q func_admin = "0xf851a440";
static const string_q func_amountFilled = "0x46be96c3";
static const string_q func_availableVolume = "0xfb6e155f";
static const string_q func_balanceOf = "0xf7888aec";
static const string_q func_cancelOrder = "0x278b8c0e";
static const string_q func_changeAccountLevelsAddr = "0xe8f6bc2e";
static const string_q func_changeAdmin = "0x8f283970";
static const string_q func_changeFeeAccount = "0x71ffcb16";
static const string_q func_changeFeeMake = "0x54d03b5c";
static const string_q func_changeFeeRebate = "0x5e1d7ae4";
static const string_q func_changeFeeTake = "0x8823a9c0";
static const string_q func_deposit = "0xd0e30db0";
static const string_q func_depositToken = "0x338b5dea";
static const string_q func_feeAccount = "0x65e17c9d";
static const string_q func_feeMake = "0x57786394";
static const string_q func_feeRebate = "0x731c2f81";
static const string_q func_feeTake = "0xc281309e";
static const string_q func_order = "0x0b927666";
static const string_q func_orderFills = "0x19774d43";
static const string_q func_orders = "0xbb5f4629";
static const string_q func_testTrade = "0x6c86888b";
static const string_q func_tokens = "0x508493bc";
static const string_q func_trade = "0x0a19b14a";
static const string_q func_withdraw = "0x2e1a7d4d";
static const string_q func_withdrawToken = "0x9e281a98";

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

        if (encoding == func_cancelOrder) {
            // function cancelOrder(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, uint8 v, bytes32 r, bytes32 s)
            // 0x278b8c0e
            p->func = new CFunction("cancelOrder");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(params, 3*64, 64))));
            p->func->inputs.push_back(CParameter("expires", "uint256", str_2_Wei("0x" + extract(params, 4*64, 64))));
            p->func->inputs.push_back(CParameter("nonce", "uint256", str_2_Wei("0x" + extract(params, 5*64, 64))));
            p->func->inputs.push_back(CParameter("v", "uint8", str_2_Uint("0x" + extract(params, 6*64, 64))));
            p->func->inputs.push_back(CParameter("r", "bytes32", "0x" + extract(params, 7*64, 64)));
            p->func->inputs.push_back(CParameter("s", "bytes32", "0x" + extract(params, 8*64, 64)));
            return true;

        } else if (encoding == func_changeAccountLevelsAddr) {
            // function changeAccountLevelsAddr(address accountLevelsAddr_)
            // 0xe8f6bc2e
            p->func = new CFunction("changeAccountLevelsAddr");
            p->func->inputs.push_back(CParameter("accountLevelsAddr_", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeAdmin) {
            // function changeAdmin(address admin_)
            // 0x8f283970
            p->func = new CFunction("changeAdmin");
            p->func->inputs.push_back(CParameter("admin_", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeFeeAccount) {
            // function changeFeeAccount(address feeAccount_)
            // 0x71ffcb16
            p->func = new CFunction("changeFeeAccount");
            p->func->inputs.push_back(CParameter("feeAccount_", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeFeeMake) {
            // function changeFeeMake(uint256 feeMake_)
            // 0x54d03b5c
            p->func = new CFunction("changeFeeMake");
            p->func->inputs.push_back(CParameter("feeMake_", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeFeeRebate) {
            // function changeFeeRebate(uint256 feeRebate_)
            // 0x5e1d7ae4
            p->func = new CFunction("changeFeeRebate");
            p->func->inputs.push_back(CParameter("feeRebate_", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_changeFeeTake) {
            // function changeFeeTake(uint256 feeTake_)
            // 0x8823a9c0
            p->func = new CFunction("changeFeeTake");
            p->func->inputs.push_back(CParameter("feeTake_", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_deposit) {
            // function deposit()
            // 0xd0e30db0
            p->func = new CFunction("deposit");
            return true;

        } else if (encoding == func_depositToken) {
            // function depositToken(address token, uint256 amount)
            // 0x338b5dea
            p->func = new CFunction("depositToken");
            p->func->inputs.push_back(CParameter("token", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_order) {
            // function order(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce)
            // 0x0b927666
            p->func = new CFunction("order");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(params, 3*64, 64))));
            p->func->inputs.push_back(CParameter("expires", "uint256", str_2_Wei("0x" + extract(params, 4*64, 64))));
            p->func->inputs.push_back(CParameter("nonce", "uint256", str_2_Wei("0x" + extract(params, 5*64, 64))));
            return true;

        } else if (encoding == func_trade) {
            // function trade(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user, uint8 v, bytes32 r, bytes32 s, uint256 amount)
            // 0x0a19b14a
            p->func = new CFunction("trade");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(params, 3*64, 64))));
            p->func->inputs.push_back(CParameter("expires", "uint256", str_2_Wei("0x" + extract(params, 4*64, 64))));
            p->func->inputs.push_back(CParameter("nonce", "uint256", str_2_Wei("0x" + extract(params, 5*64, 64))));
            p->func->inputs.push_back(CParameter("user", "address", str_2_Addr(extract(params, 6*64, 64))));
            p->func->inputs.push_back(CParameter("v", "uint8", str_2_Uint("0x" + extract(params, 7*64, 64))));
            p->func->inputs.push_back(CParameter("r", "bytes32", "0x" + extract(params, 8*64, 64)));
            p->func->inputs.push_back(CParameter("s", "bytes32", "0x" + extract(params, 9*64, 64)));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(params, 10*64, 64))));
            return true;

        } else if (encoding == func_withdraw) {
            // function withdraw(uint256 amount)
            // 0x2e1a7d4d
            p->func = new CFunction("withdraw");
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_withdrawToken) {
            // function withdrawToken(address token, uint256 amount)
            // 0x9e281a98
            p->func = new CFunction("withdrawToken");
            p->func->inputs.push_back(CParameter("token", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
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
static const string_q evt_Cancel = "0x1e0b760c386003e9cb9bcf4fcf3997886042859d9b6ed6320e804597fcdb28b0";
static const string_q evt_Deposit = "0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7";
static const string_q evt_Order = "0x3f7f2eda73683c21a15f9435af1028c93185b5f1fa38270762dc32be606b3e85";
static const string_q evt_Trade = "0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d";
static const string_q evt_Withdraw = "0xf341246adaac6f497bc2a656f546ab9e182111d630394f0c57c710a59a2cb567";

//-----------------------------------------------------------------------
bool articulateEvent(CLogEntry *p) {

    if (p->func)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_Cancel) {
            // event Cancel(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user, uint8 v, bytes32 r, bytes32 s)
            // 0x1e0b760c386003e9cb9bcf4fcf3997886042859d9b6ed6320e804597fcdb28b0
            p->func = new CFunction("CancelEvent");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
            p->func->inputs.push_back(CParameter("expires", "uint256", str_2_Wei("0x" + extract(data, 4*64, 64))));
            p->func->inputs.push_back(CParameter("nonce", "uint256", str_2_Wei("0x" + extract(data, 5*64, 64))));
            p->func->inputs.push_back(CParameter("user", "address", str_2_Addr(extract(data, 6*64, 64))));
            p->func->inputs.push_back(CParameter("v", "uint8", str_2_Uint("0x" + extract(data, 7*64, 64))));
            p->func->inputs.push_back(CParameter("r", "bytes32", "0x" + extract(data, 8*64, 64)));
            p->func->inputs.push_back(CParameter("s", "bytes32", "0x" + extract(data, 9*64, 64)));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Deposit) {
            // event Deposit(address token, address user, uint256 amount, uint256 balance)
            // 0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7
            p->func = new CFunction("DepositEvent");
            p->func->inputs.push_back(CParameter("token", "address", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("user", "address", str_2_Addr(extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("balance", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Order) {
            // event Order(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user)
            // 0x3f7f2eda73683c21a15f9435af1028c93185b5f1fa38270762dc32be606b3e85
            p->func = new CFunction("OrderEvent");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
            p->func->inputs.push_back(CParameter("expires", "uint256", str_2_Wei("0x" + extract(data, 4*64, 64))));
            p->func->inputs.push_back(CParameter("nonce", "uint256", str_2_Wei("0x" + extract(data, 5*64, 64))));
            p->func->inputs.push_back(CParameter("user", "address", str_2_Addr(extract(data, 6*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Trade) {
            // event Trade(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, address get, address give)
            // 0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d
            p->func = new CFunction("TradeEvent");
            p->func->inputs.push_back(CParameter("tokenGet", "address", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amountGet", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("tokenGive", "address", str_2_Addr(extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("amountGive", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
            p->func->inputs.push_back(CParameter("get", "address", str_2_Addr(extract(data, 4*64, 64))));
            p->func->inputs.push_back(CParameter("give", "address", str_2_Addr(extract(data, 5*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Withdraw) {
            // event Withdraw(address token, address user, uint256 amount, uint256 balance)
            // 0xf341246adaac6f497bc2a656f546ab9e182111d630394f0c57c710a59a2cb567
            p->func = new CFunction("WithdrawEvent");
            p->func->inputs.push_back(CParameter("token", "address", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("user", "address", str_2_Addr(extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("balance", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
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
    func_accountLevelsAddr,
    func_admin,
    func_amountFilled,
    func_availableVolume,
    func_balanceOf,
    func_cancelOrder,
    func_changeAccountLevelsAddr,
    func_changeAdmin,
    func_changeFeeAccount,
    func_changeFeeMake,
    func_changeFeeRebate,
    func_changeFeeTake,
    func_deposit,
    func_depositToken,
    func_feeAccount,
    func_feeMake,
    func_feeRebate,
    func_feeTake,
    func_order,
    func_orderFills,
    func_orders,
    func_testTrade,
    func_tokens,
    func_trade,
    func_withdraw,
    func_withdrawToken,
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
    evt_Cancel,
    evt_Deposit,
    evt_Order,
    evt_Trade,
    evt_Withdraw,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x814964b1bceaf24e26296d031eadf134a2ca4105
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"inputs":[{"name":"_initialAmount","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xc9d7fec9889690bbff0a0df758d13e5a55dd7822
{"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

ABI for addr : 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819
[{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"}],"name":"trade","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"}],"name":"order","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orderFills","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"cancelOrder","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"amount","type":"uint256"}],"name":"withdraw","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"depositToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"amountFilled","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"tokens","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeMake_","type":"uint256"}],"name":"changeFeeMake","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeMake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeRebate_","type":"uint256"}],"name":"changeFeeRebate","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeAccount","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"},{"name":"sender","type":"address"}],"name":"testTrade","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeAccount_","type":"address"}],"name":"changeFeeAccount","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeRebate","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeTake_","type":"uint256"}],"name":"changeFeeTake","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"admin_","type":"address"}],"name":"changeAdmin","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"withdrawToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orders","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeTake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"deposit","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"accountLevelsAddr_","type":"address"}],"name":"changeAccountLevelsAddr","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"accountLevelsAddr","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"token","type":"address"},{"name":"user","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"admin","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"availableVolume","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"admin_","type":"address"},{"name":"feeAccount_","type":"address"},{"name":"accountLevelsAddr_","type":"address"},{"name":"feeMake_","type":"uint256"},{"name":"feeTake_","type":"uint256"},{"name":"feeRebate_","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":false,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"}],"name":"Order","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"v","type":"uint8"},{"indexed":false,"name":"r","type":"bytes32"},{"indexed":false,"name":"s","type":"bytes32"}],"name":"Cancel","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"get","type":"address"},{"indexed":false,"name":"give","type":"address"}],"name":"Trade","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Withdraw","type":"event"}]

*/
