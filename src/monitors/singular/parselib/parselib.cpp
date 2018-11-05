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
    CChangeBaseValue::registerClass();
    CDepositRevenue::registerClass();
    CEmergencyCall::registerClass();
    CFund::registerClass();
    CIssueTokens::registerClass();
    CSetup::registerClass();
    CSoftWithdrawRevenueFor::registerClass();
    CUpdateStage::registerClass();
    CWithdrawForWorkshop::registerClass();
    CWithdrawFunding::registerClass();
    CWithdrawRevenue::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_CAP = "0xec81b483";
const string_q func_CROWDFUNDINGPERIOD = "0xcb19e913";
const string_q func_TOKENLOCKINGPERIOD = "0x705ef86c";
const string_q func_TOKENTARGET = "0x2279b0a1";
const string_q func_baseValue = "0x61bb9c52";
const string_q func_campaignEndedSuccessfully = "0xdcf8113e";
const string_q func_changeBaseValue = "0x92a781d8";
const string_q func_depositRevenue = "0x44e43cb8";
const string_q func_emergencyCall = "0x299ed37a";
const string_q func_fund = "0xb60d4288";
const string_q func_fundBalance = "0x3c067945";
const string_q func_investments = "0x96b98862";
const string_q func_issueTokens = "0x475a9fa9";
const string_q func_mlastDay = "0xe17db230";
const string_q func_mspentToday = "0x3938e00a";
const string_q func_owed = "0xdf18e047";
const string_q func_owner = "0x8da5cb5b";
const string_q func_revenueAtTimeOfWithdraw = "0x7e77a2c0";
const string_q func_setup = "0x2d34ba79";
const string_q func_singularDTVCrowdfunding = "0xa9dc1d54";
const string_q func_singularDTVFund = "0x58a74333";
const string_q func_singularDTVToken = "0x87efeeb6";
const string_q func_softWithdrawRevenueFor = "0x5d0be9de";
const string_q func_stage = "0xc040e6b8";
const string_q func_startDate = "0x0b97bc86";
const string_q func_totalRevenue = "0xbf2d9e0b";
const string_q func_twoYearsPassed = "0x49cc954b";
const string_q func_updateStage = "0xc062f578";
const string_q func_valuePerShare = "0xc0a239e3";
const string_q func_withdrawForWorkshop = "0xf3a44fe1";
const string_q func_withdrawFunding = "0x30b9af98";
const string_q func_withdrawRevenue = "0x4f573cb2";
const string_q func_workshop = "0x720c4798";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_changeBaseValue) {
            // function changeBaseValue(uint256 valueInWei)
            // 0x92a781d8
            CChangeBaseValue *a = new CChangeBaseValue;
            a->CTransaction::operator=(*p);
            a->valueInWei = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeBaseValue", params, nItems, items);
            return a;

        } else if (encoding == func_depositRevenue) {
            // function depositRevenue()
            // 0x44e43cb8
            CDepositRevenue *a = new CDepositRevenue;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("depositRevenue", params, nItems, items);
            return a;

        } else if (encoding == func_emergencyCall) {
            // function emergencyCall()
            // 0x299ed37a
            CEmergencyCall *a = new CEmergencyCall;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("emergencyCall", params, nItems, items);
            return a;

        } else if (encoding == func_fund) {
            // function fund()
            // 0xb60d4288
            CFund *a = new CFund;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("fund", params, nItems, items);
            return a;

        } else if (encoding == func_issueTokens) {
            // function issueTokens(address _for, uint256 tokenCount)
            // 0x475a9fa9
            CIssueTokens *a = new CIssueTokens;
            a->CTransaction::operator=(*p);
            a->_for = str_2_Addr(extract(params, 0*64, 64));
            a->tokenCount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("issueTokens", params, nItems, items);
            return a;

        } else if (encoding == func_setup) {
            // function setup(address singularDTVCrowdfundingAddress, address singularDTVTokenAddress)
            // 0x2d34ba79
            CSetup *a = new CSetup;
            a->CTransaction::operator=(*p);
            a->singularDTVCrowdfundingAddress = str_2_Addr(extract(params, 0*64, 64));
            a->singularDTVTokenAddress = str_2_Addr(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setup", params, nItems, items);
            return a;

        } else if (encoding == func_softWithdrawRevenueFor) {
            // function softWithdrawRevenueFor(address forAddress)
            // 0x5d0be9de
            CSoftWithdrawRevenueFor *a = new CSoftWithdrawRevenueFor;
            a->CTransaction::operator=(*p);
            a->forAddress = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("softWithdrawRevenueFor", params, nItems, items);
            return a;

        } else if (encoding == func_updateStage) {
            // function updateStage()
            // 0xc062f578
            CUpdateStage *a = new CUpdateStage;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("updateStage", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawForWorkshop) {
            // function withdrawForWorkshop()
            // 0xf3a44fe1
            CWithdrawForWorkshop *a = new CWithdrawForWorkshop;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdrawForWorkshop", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawFunding) {
            // function withdrawFunding()
            // 0x30b9af98
            CWithdrawFunding *a = new CWithdrawFunding;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdrawFunding", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawRevenue) {
            // function withdrawRevenue()
            // 0x4f573cb2
            CWithdrawRevenue *a = new CWithdrawRevenue;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdrawRevenue", params, nItems, items);
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
// No events

//-----------------------------------------------------------------------
const CLogEntry *promoteToEvent(const CLogEntry *p) {

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        // EXISTING_CODE
        // EXISTING_CODE

        {
            // No events
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
    func_CAP,
    func_CROWDFUNDINGPERIOD,
    func_TOKENLOCKINGPERIOD,
    func_TOKENTARGET,
    func_baseValue,
    func_campaignEndedSuccessfully,
    func_changeBaseValue,
    func_depositRevenue,
    func_emergencyCall,
    func_fund,
    func_fundBalance,
    func_investments,
    func_issueTokens,
    func_mlastDay,
    func_mspentToday,
    func_owed,
    func_owner,
    func_revenueAtTimeOfWithdraw,
    func_setup,
    func_singularDTVCrowdfunding,
    func_singularDTVFund,
    func_singularDTVToken,
    func_softWithdrawRevenueFor,
    func_stage,
    func_startDate,
    func_totalRevenue,
    func_twoYearsPassed,
    func_updateStage,
    func_valuePerShare,
    func_withdrawForWorkshop,
    func_withdrawFunding,
    func_withdrawRevenue,
    func_workshop,
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
    // No events
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xaec2e87e0a235266d9c5adc9deb4b2e29b54d009
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"from","type":"address"},{"name":"to","type":"address"},{"name":"value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":false,"inputs":[{"name":"_for","type":"address"},{"name":"tokenCount","type":"uint256"}],"name":"issueTokens","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"to","type":"address"},{"name":"value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"inputs":[],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xe736091fc36f1ad476f5e4e03e4425940822d3ba
[{"constant":false,"inputs":[{"name":"singularDTVCrowdfundingAddress","type":"address"},{"name":"singularDTVTokenAddress","type":"address"}],"name":"setup","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"depositRevenue","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"withdrawRevenue","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"forAddress","type":"address"}],"name":"softWithdrawRevenueFor","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"workshop","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"revenueAtTimeOfWithdraw","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"singularDTVToken","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"singularDTVCrowdfunding","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalRevenue","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"owed","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"inputs":[],"type":"constructor"}]

ABI for addr : 0xbdf5c4f1c1a9d7335a6a68d9aa011d5f40cf5520
[{"constant":true,"inputs":[],"name":"startDate","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"CROWDFUNDING_PERIOD","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"emergencyCall","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"singularDTVFundAddress","type":"address"},{"name":"singularDTVTokenAddress","type":"address"}],"name":"setup","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"withdrawFunding","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"fundBalance","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"twoYearsPassed","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"singularDTVFund","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"baseValue","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_TARGET","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"singularDTVToken","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"valueInWei","type":"uint256"}],"name":"changeBaseValue","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"investments","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"fund","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"stage","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":false,"inputs":[],"name":"updateStage","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[],"name":"valuePerShare","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_LOCKING_PERIOD","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"campaignEndedSuccessfully","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"CAP","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"withdrawForWorkshop","outputs":[{"name":"","type":"bool"}],"type":"function"},{"inputs":[],"type":"constructor"}]

ABI for addr : 0xc78310231aa53bd3d0fea2f8c705c67730929d8f
[]

ABI for addr : 0x5901deb2c898d5dbe5923e05e510e95968a35067
[{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"removeOwner","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"_addr","type":"address"}],"name":"isOwner","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"m_numOwners","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"m_lastDay","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"resetSpentToday","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"m_spentToday","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_owner","type":"address"}],"name":"addOwner","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"m_required","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_h","type":"bytes32"}],"name":"confirm","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_newLimit","type":"uint256"}],"name":"setDailyLimit","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"},{"name":"_data","type":"bytes"}],"name":"execute","outputs":[{"name":"_r","type":"bytes32"}],"type":"function"},{"constant":false,"inputs":[{"name":"_operation","type":"bytes32"}],"name":"revoke","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"_newRequired","type":"uint256"}],"name":"changeRequirement","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"_operation","type":"bytes32"},{"name":"_owner","type":"address"}],"name":"hasConfirmed","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"}],"name":"kill","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"}],"name":"changeOwner","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"m_dailyLimit","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"inputs":[{"name":"_owners","type":"address[]"},{"name":"_required","type":"uint256"},{"name":"_daylimit","type":"uint256"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"}],"name":"Confirmation","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"}],"name":"Revoke","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"oldOwner","type":"address"},{"indexed":false,"name":"newOwner","type":"address"}],"name":"OwnerChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"newOwner","type":"address"}],"name":"OwnerAdded","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"oldOwner","type":"address"}],"name":"OwnerRemoved","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"newRequirement","type":"uint256"}],"name":"RequirementChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"from","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"SingleTransact","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"owner","type":"address"},{"indexed":false,"name":"operation","type":"bytes32"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"MultiTransact","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"operation","type":"bytes32"},{"indexed":false,"name":"initiator","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"to","type":"address"},{"indexed":false,"name":"data","type":"bytes"}],"name":"ConfirmationNeeded","type":"event"}]

*/
