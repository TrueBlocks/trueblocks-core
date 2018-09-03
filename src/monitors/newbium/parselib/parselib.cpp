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
#include "parselib.h"

//-----------------------------------------------------------------------
void parselib_init(QUITHANDLER qh) {
    etherlib_init(qh);
    CCancelEvent::registerClass();
    CDepositEvent::registerClass();
    COrderEvent::registerClass();
    CTradeEvent::registerClass();
    CWithdrawEvent::registerClass();
    CCancelOrder::registerClass();
    CChangeAccountLevelsAddr::registerClass();
    CChangeAdmin::registerClass();
    CChangeFeeAccount::registerClass();
    CChangeFeeMake::registerClass();
    CChangeFeeRebate::registerClass();
    CChangeFeeTake::registerClass();
    CDeposit::registerClass();
    CDepositToken::registerClass();
    COrder::registerClass();
    CTrade::registerClass();
    CWithdraw::registerClass();
    CWithdrawToken::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_accountLevelsAddr = "0xf3412942";
const string_q func_admin = "0xf851a440";
const string_q func_amountFilled = "0x46be96c3";
const string_q func_availableVolume = "0xfb6e155f";
const string_q func_balanceOf = "0xf7888aec";
const string_q func_cancelOrder = "0x278b8c0e";
const string_q func_changeAccountLevelsAddr = "0xe8f6bc2e";
const string_q func_changeAdmin = "0x8f283970";
const string_q func_changeFeeAccount = "0x71ffcb16";
const string_q func_changeFeeMake = "0x54d03b5c";
const string_q func_changeFeeRebate = "0x5e1d7ae4";
const string_q func_changeFeeTake = "0x8823a9c0";
const string_q func_deposit = "0xd0e30db0";
const string_q func_depositToken = "0x338b5dea";
const string_q func_feeAccount = "0x65e17c9d";
const string_q func_feeMake = "0x57786394";
const string_q func_feeRebate = "0x731c2f81";
const string_q func_feeTake = "0xc281309e";
const string_q func_order = "0x0b927666";
const string_q func_orderFills = "0x19774d43";
const string_q func_orders = "0xbb5f4629";
const string_q func_testTrade = "0x6c86888b";
const string_q func_tokens = "0x508493bc";
const string_q func_trade = "0x0a19b14a";
const string_q func_withdraw = "0x2e1a7d4d";
const string_q func_withdrawToken = "0x9e281a98";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_cancelOrder) {
            // function cancelOrder(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, uint8 v, bytes32 r, bytes32 s)
            // 0x278b8c0e
            CCancelOrder *a = new CCancelOrder;
            a->CTransaction::operator=(*p);
            a->tokenGet = str_2_Addr(extract(params, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(params, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(params, 3*64, 64));
            a->expires = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->nonce = str_2_Wei("0x" + extract(params, 5*64, 64));
            a->v = (uint32_t)str_2_Uint(extract(params, 6*64, 64));
            a->r = extract(params, 7*64, 64);
            a->s = extract(params, 8*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint8";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("cancelOrder", params, nItems, items);
            return a;

        } else if (encoding == func_changeAccountLevelsAddr) {
            // function changeAccountLevelsAddr(address accountLevelsAddr_)
            // 0xe8f6bc2e
            CChangeAccountLevelsAddr *a = new CChangeAccountLevelsAddr;
            a->CTransaction::operator=(*p);
            a->accountLevelsAddr_ = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeAccountLevelsAddr", params, nItems, items);
            return a;

        } else if (encoding == func_changeAdmin) {
            // function changeAdmin(address admin_)
            // 0x8f283970
            CChangeAdmin *a = new CChangeAdmin;
            a->CTransaction::operator=(*p);
            a->admin_ = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeAdmin", params, nItems, items);
            return a;

        } else if (encoding == func_changeFeeAccount) {
            // function changeFeeAccount(address feeAccount_)
            // 0x71ffcb16
            CChangeFeeAccount *a = new CChangeFeeAccount;
            a->CTransaction::operator=(*p);
            a->feeAccount_ = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("changeFeeAccount", params, nItems, items);
            return a;

        } else if (encoding == func_changeFeeMake) {
            // function changeFeeMake(uint256 feeMake_)
            // 0x54d03b5c
            CChangeFeeMake *a = new CChangeFeeMake;
            a->CTransaction::operator=(*p);
            a->feeMake_ = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeFeeMake", params, nItems, items);
            return a;

        } else if (encoding == func_changeFeeRebate) {
            // function changeFeeRebate(uint256 feeRebate_)
            // 0x5e1d7ae4
            CChangeFeeRebate *a = new CChangeFeeRebate;
            a->CTransaction::operator=(*p);
            a->feeRebate_ = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeFeeRebate", params, nItems, items);
            return a;

        } else if (encoding == func_changeFeeTake) {
            // function changeFeeTake(uint256 feeTake_)
            // 0x8823a9c0
            CChangeFeeTake *a = new CChangeFeeTake;
            a->CTransaction::operator=(*p);
            a->feeTake_ = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeFeeTake", params, nItems, items);
            return a;

        } else if (encoding == func_deposit) {
            // function deposit()
            // 0xd0e30db0
            CDeposit *a = new CDeposit;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("deposit", params, nItems, items);
            return a;

        } else if (encoding == func_depositToken) {
            // function depositToken(address token, uint256 amount)
            // 0x338b5dea
            CDepositToken *a = new CDepositToken;
            a->CTransaction::operator=(*p);
            a->token = str_2_Addr(extract(params, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("depositToken", params, nItems, items);
            return a;

        } else if (encoding == func_order) {
            // function order(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce)
            // 0x0b927666
            COrder *a = new COrder;
            a->CTransaction::operator=(*p);
            a->tokenGet = str_2_Addr(extract(params, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(params, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(params, 3*64, 64));
            a->expires = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->nonce = str_2_Wei("0x" + extract(params, 5*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("order", params, nItems, items);
            return a;

        } else if (encoding == func_trade) {
            // function trade(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user, uint8 v, bytes32 r, bytes32 s, uint256 amount)
            // 0x0a19b14a
            CTrade *a = new CTrade;
            a->CTransaction::operator=(*p);
            a->tokenGet = str_2_Addr(extract(params, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(params, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(params, 3*64, 64));
            a->expires = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->nonce = str_2_Wei("0x" + extract(params, 5*64, 64));
            a->user = str_2_Addr(extract(params, 6*64, 64));
            a->v = (uint32_t)str_2_Uint(extract(params, 7*64, 64));
            a->r = extract(params, 8*64, 64);
            a->s = extract(params, 9*64, 64);
            a->amount = str_2_Wei("0x" + extract(params, 10*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("trade", params, nItems, items);
            return a;

        } else if (encoding == func_withdraw) {
            // function withdraw(uint256 amount)
            // 0x2e1a7d4d
            CWithdraw *a = new CWithdraw;
            a->CTransaction::operator=(*p);
            a->amount = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdraw", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawToken) {
            // function withdrawToken(address token, uint256 amount)
            // 0x9e281a98
            CWithdrawToken *a = new CWithdrawToken;
            a->CTransaction::operator=(*p);
            a->token = str_2_Addr(extract(params, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdrawToken", params, nItems, items);
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
const string_q evt_Cancel = "0x1e0b760c386003e9cb9bcf4fcf3997886042859d9b6ed6320e804597fcdb28b0";
const string_q evt_Deposit = "0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7";
const string_q evt_Order = "0x3f7f2eda73683c21a15f9435af1028c93185b5f1fa38270762dc32be606b3e85";
const string_q evt_Trade = "0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d";
const string_q evt_Withdraw = "0xf341246adaac6f497bc2a656f546ab9e182111d630394f0c57c710a59a2cb567";

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

        if (topic_2_Str(p->topics[0]) % evt_Cancel) {
            // event Cancel(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user, uint8 v, bytes32 r, bytes32 s)
            // 0x1e0b760c386003e9cb9bcf4fcf3997886042859d9b6ed6320e804597fcdb28b0
            CCancelEvent *a = new CCancelEvent;
            a->CLogEntry::operator=(*p);
            a->tokenGet = str_2_Addr(extract(data, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(data, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->expires = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->nonce = str_2_Wei("0x" + extract(data, 5*64, 64));
            a->user = str_2_Addr(extract(data, 6*64, 64));
            a->v = (uint32_t)str_2_Uint("0x" + extract(data, 7*64, 64));
            a->r = "0x" + extract(data, 8*64, 64);
            a->s = "0x" + extract(data, 9*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedLog = decodeRLP("Cancel", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Deposit) {
            // event Deposit(address token, address user, uint256 amount, uint256 balance)
            // 0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7
            CDepositEvent *a = new CDepositEvent;
            a->CLogEntry::operator=(*p);
            a->token = str_2_Addr(extract(data, 0*64, 64));
            a->user = str_2_Addr(extract(data, 1*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->balance = str_2_Wei("0x" + extract(data, 3*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Deposit", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Order) {
            // event Order(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user)
            // 0x3f7f2eda73683c21a15f9435af1028c93185b5f1fa38270762dc32be606b3e85
            COrderEvent *a = new COrderEvent;
            a->CLogEntry::operator=(*p);
            a->tokenGet = str_2_Addr(extract(data, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(data, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->expires = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->nonce = str_2_Wei("0x" + extract(data, 5*64, 64));
            a->user = str_2_Addr(extract(data, 6*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Order", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Trade) {
            // event Trade(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, address get, address give)
            // 0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d
            CTradeEvent *a = new CTradeEvent;
            a->CLogEntry::operator=(*p);
            a->tokenGet = str_2_Addr(extract(data, 0*64, 64));
            a->amountGet = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->tokenGive = str_2_Addr(extract(data, 2*64, 64));
            a->amountGive = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->get = str_2_Addr(extract(data, 4*64, 64));
            a->give = str_2_Addr(extract(data, 5*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Trade", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Withdraw) {
            // event Withdraw(address token, address user, uint256 amount, uint256 balance)
            // 0xf341246adaac6f497bc2a656f546ab9e182111d630394f0c57c710a59a2cb567
            CWithdrawEvent *a = new CWithdrawEvent;
            a->CLogEntry::operator=(*p);
            a->token = str_2_Addr(extract(data, 0*64, 64));
            a->user = str_2_Addr(extract(data, 1*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->balance = str_2_Wei("0x" + extract(data, 3*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
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

/*
 ABI for addr : 0x814964b1bceaf24e26296d031eadf134a2ca4105
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"inputs":[{"name":"_initialAmount","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xc9d7fec9889690bbff0a0df758d13e5a55dd7822
{"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

ABI for addr : 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819
[{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"}],"name":"trade","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"}],"name":"order","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orderFills","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"cancelOrder","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"amount","type":"uint256"}],"name":"withdraw","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"depositToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"amountFilled","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"tokens","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeMake_","type":"uint256"}],"name":"changeFeeMake","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeMake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeRebate_","type":"uint256"}],"name":"changeFeeRebate","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeAccount","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"},{"name":"sender","type":"address"}],"name":"testTrade","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeAccount_","type":"address"}],"name":"changeFeeAccount","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeRebate","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeTake_","type":"uint256"}],"name":"changeFeeTake","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"admin_","type":"address"}],"name":"changeAdmin","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"withdrawToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orders","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeTake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"deposit","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"accountLevelsAddr_","type":"address"}],"name":"changeAccountLevelsAddr","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"accountLevelsAddr","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"token","type":"address"},{"name":"user","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"admin","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"availableVolume","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"admin_","type":"address"},{"name":"feeAccount_","type":"address"},{"name":"accountLevelsAddr_","type":"address"},{"name":"feeMake_","type":"uint256"},{"name":"feeTake_","type":"uint256"},{"name":"feeRebate_","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":false,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"}],"name":"Order","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"v","type":"uint8"},{"indexed":false,"name":"r","type":"bytes32"},{"indexed":false,"name":"s","type":"bytes32"}],"name":"Cancel","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"get","type":"address"},{"indexed":false,"name":"give","type":"address"}],"name":"Trade","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Withdraw","type":"event"}]

*/
