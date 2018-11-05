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
    CCapReachedEvent::registerClass();
    CFundTransferEvent::registerClass();
    COwnershipTransferredEvent::registerClass();
    CPauseEvent::registerClass();
    CRegistrationStatusChangedEvent::registerClass();
    CUnpauseEvent::registerClass();
    CAllocateTokens::registerClass();
    CBurn::registerClass();
    CBuy::registerClass();
    CDeactivate::registerClass();
    CDecreaseApproval::registerClass();
    CEnableTransfer::registerClass();
    CIncreaseApproval::registerClass();
    COwnerSafeWithdrawal::registerClass();
    CPause::registerClass();
    CRegisterUser::registerClass();
    CRegisterUsers::registerClass();
    CSetCap::registerClass();
    CSetCrowdsale::registerClass();
    CSetDeadline::registerClass();
    CTerminate::registerClass();
    CTransferOwnership::registerClass();
    CUnpause::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_ADMINALLOWANCE = "0x0ff42b60";
const string_q func_CROWDSALEALLOWANCE = "0x341cc1df";
const string_q func_GASLIMITINWEI = "0x828e1907";
const string_q func_INITIALSUPPLY = "0xde6ab39c";
const string_q func_RATE = "0x664e9704";
const string_q func_adminAddr = "0x81830593";
const string_q func_adminAllowance = "0xd56de6ed";
const string_q func_allocateTokens = "0x8d3f0ed5";
const string_q func_amountRaised = "0x7b3e5e7b";
const string_q func_beneficiary = "0x38af3eed";
const string_q func_burn = "0x42966c68";
const string_q func_buy = "0xa6f2ae3a";
const string_q func_cap = "0x355274ea";
const string_q func_capTime = "0x4df28ae4";
const string_q func_crowdSaleAddr = "0x8eeb33ff";
const string_q func_crowdSaleAllowance = "0xd14ac7c4";
const string_q func_currentTime = "0xd18e81b3";
const string_q func_deactivate = "0x3ea053eb";
const string_q func_deadline = "0x29dcb0cf";
const string_q func_decreaseApproval = "0x66188463";
const string_q func_enableTransfer = "0xf1b50c1d";
const string_q func_fundingCap = "0xe3b2594f";
const string_q func_fundingCapReached = "0x848b3821";
const string_q func_increaseApproval = "0xd73dd623";
const string_q func_mainsaleBalanceOf = "0xb6e09093";
const string_q func_minContribution = "0xaaffadf3";
const string_q func_owner = "0x8da5cb5b";
const string_q func_ownerSafeWithdrawal = "0x782e34c9";
const string_q func_pause = "0x8456cb59";
const string_q func_paused = "0x5c975abb";
const string_q func_refundAmount = "0xad33513f";
const string_q func_registerUser = "0x2199d5cd";
const string_q func_registerUsers = "0xe5e9a9bb";
const string_q func_registry = "0x038defd7";
const string_q func_saleClosed = "0xb8c766b8";
const string_q func_setCap = "0x47786d37";
const string_q func_setCrowdsale = "0x22ed6302";
const string_q func_setDeadline = "0x195199f6";
const string_q func_startTime = "0x78e97925";
const string_q func_terminate = "0x0c08bf88";
const string_q func_tokenReward = "0x6e66f6e9";
const string_q func_transferEnabled = "0x4cd412d5";
const string_q func_transferOwnership = "0xf2fde38b";
const string_q func_unpause = "0x3f4ba83a";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_allocateTokens) {
            // function allocateTokens(address _to, uint256 amountWei, uint256 amountMiniQsp)
            // 0x8d3f0ed5
            CAllocateTokens *a = new CAllocateTokens;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->amountWei = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->amountMiniQsp = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("allocateTokens", params, nItems, items);
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

        } else if (encoding == func_buy) {
            // function buy()
            // 0xa6f2ae3a
            CBuy *a = new CBuy;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("buy", params, nItems, items);
            return a;

        } else if (encoding == func_deactivate) {
            // function deactivate(address contributor)
            // 0x3ea053eb
            CDeactivate *a = new CDeactivate;
            a->CTransaction::operator=(*p);
            a->contributor = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("deactivate", params, nItems, items);
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

        } else if (encoding == func_enableTransfer) {
            // function enableTransfer()
            // 0xf1b50c1d
            CEnableTransfer *a = new CEnableTransfer;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("enableTransfer", params, nItems, items);
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

        } else if (encoding == func_ownerSafeWithdrawal) {
            // function ownerSafeWithdrawal()
            // 0x782e34c9
            COwnerSafeWithdrawal *a = new COwnerSafeWithdrawal;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("ownerSafeWithdrawal", params, nItems, items);
            return a;

        } else if (encoding == func_pause) {
            // function pause()
            // 0x8456cb59
            CPause *a = new CPause;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("pause", params, nItems, items);
            return a;

        } else if (encoding == func_registerUser) {
            // function registerUser(address contributor)
            // 0x2199d5cd
            CRegisterUser *a = new CRegisterUser;
            a->CTransaction::operator=(*p);
            a->contributor = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("registerUser", params, nItems, items);
            return a;

        } else if (encoding == func_registerUsers) {
            // function registerUsers(address[] contributors)
            // 0xe5e9a9bb
            CRegisterUsers *a = new CRegisterUsers;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->contributors.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("registerUsers", params, nItems, items);
            return a;

        } else if (encoding == func_setCap) {
            // function setCap(uint256 _cap)
            // 0x47786d37
            CSetCap *a = new CSetCap;
            a->CTransaction::operator=(*p);
            a->_cap = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("setCap", params, nItems, items);
            return a;

        } else if (encoding == func_setCrowdsale) {
            // function setCrowdsale(address _crowdSaleAddr, uint256 _amountForSale)
            // 0x22ed6302
            CSetCrowdsale *a = new CSetCrowdsale;
            a->CTransaction::operator=(*p);
            a->_crowdSaleAddr = str_2_Addr(extract(params, 0*64, 64));
            a->_amountForSale = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("setCrowdsale", params, nItems, items);
            return a;

        } else if (encoding == func_setDeadline) {
            // function setDeadline(uint256 timestamp)
            // 0x195199f6
            CSetDeadline *a = new CSetDeadline;
            a->CTransaction::operator=(*p);
            a->timestamp = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("setDeadline", params, nItems, items);
            return a;

        } else if (encoding == func_terminate) {
            // function terminate()
            // 0x0c08bf88
            CTerminate *a = new CTerminate;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("terminate", params, nItems, items);
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

        } else if (encoding == func_unpause) {
            // function unpause()
            // 0x3f4ba83a
            CUnpause *a = new CUnpause;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("unpause", params, nItems, items);
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
const string_q evt_CapReached = "0x195de8a1ebf5a1244af169b7e0bd0f735307a2626a73abce88f9b488628c6c24";
const string_q evt_FundTransfer = "0xe842aea7a5f1b01049d752008c53c52890b1a6daf660cf39e8eec506112bbdf6";
const string_q evt_OwnershipTransferred = "0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0";
const string_q evt_Pause = "0x6985a02210a168e66602d3235cb6db0e70f92b3ba4d376a33c0f3d9434bff625";
const string_q evt_RegistrationStatusChanged = "0x61711115e3f6f3c059d7dee6db7f77e9c82b97ae30fc2aee76c053c0048030bf";
const string_q evt_Unpause = "0x7805862f689e2f13df9f062ff482ad3ad112aca9e0847911ed832e158c525b33";

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

        } else if (topic_2_Str(p->topics[0]) % evt_CapReached) {
            // event CapReached(address _beneficiary, uint256 _amountRaised)
            // 0x195de8a1ebf5a1244af169b7e0bd0f735307a2626a73abce88f9b488628c6c24
            CCapReachedEvent *a = new CCapReachedEvent;
            a->CLogEntry::operator=(*p);
            a->_beneficiary = str_2_Addr(extract(data, 0*64, 64));
            a->_amountRaised = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("CapReached", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_FundTransfer) {
            // event FundTransfer(address _backer, uint256 _amount, bool _isContribution)
            // 0xe842aea7a5f1b01049d752008c53c52890b1a6daf660cf39e8eec506112bbdf6
            CFundTransferEvent *a = new CFundTransferEvent;
            a->CLogEntry::operator=(*p);
            a->_backer = str_2_Addr(extract(data, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_isContribution = str_2_Int("0x" + extract(data, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            a->articulatedLog = decodeRLP("FundTransfer", params, nItems, items);
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

        } else if (topic_2_Str(p->topics[0]) % evt_Pause) {
            // event Pause()
            // 0x6985a02210a168e66602d3235cb6db0e70f92b3ba4d376a33c0f3d9434bff625
            CPauseEvent *a = new CPauseEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("Pause", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_RegistrationStatusChanged) {
            // event RegistrationStatusChanged(address target, bool isRegistered)
            // 0x61711115e3f6f3c059d7dee6db7f77e9c82b97ae30fc2aee76c053c0048030bf
            CRegistrationStatusChangedEvent *a = new CRegistrationStatusChangedEvent;
            a->CLogEntry::operator=(*p);
            a->target = str_2_Addr(extract(data, 0*64, 64));
            a->isRegistered = str_2_Int("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedLog = decodeRLP("RegistrationStatusChanged", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Unpause) {
            // event Unpause()
            // 0x7805862f689e2f13df9f062ff482ad3ad112aca9e0847911ed832e158c525b33
            CUnpauseEvent *a = new CUnpauseEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("Unpause", params, nItems, items);
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
    func_ADMINALLOWANCE,
    func_CROWDSALEALLOWANCE,
    func_GASLIMITINWEI,
    func_INITIALSUPPLY,
    func_RATE,
    func_adminAddr,
    func_adminAllowance,
    func_allocateTokens,
    func_amountRaised,
    func_beneficiary,
    func_burn,
    func_buy,
    func_cap,
    func_capTime,
    func_crowdSaleAddr,
    func_crowdSaleAllowance,
    func_currentTime,
    func_deactivate,
    func_deadline,
    func_decreaseApproval,
    func_enableTransfer,
    func_fundingCap,
    func_fundingCapReached,
    func_increaseApproval,
    func_mainsaleBalanceOf,
    func_minContribution,
    func_owner,
    func_ownerSafeWithdrawal,
    func_pause,
    func_paused,
    func_refundAmount,
    func_registerUser,
    func_registerUsers,
    func_registry,
    func_saleClosed,
    func_setCap,
    func_setCrowdsale,
    func_setDeadline,
    func_startTime,
    func_terminate,
    func_tokenReward,
    func_transferEnabled,
    func_transferOwnership,
    func_unpause,
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
    evt_CapReached,
    evt_FundTransfer,
    evt_OwnershipTransferred,
    evt_Pause,
    evt_RegistrationStatusChanged,
    evt_Unpause,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x99ea4db9ee77acd40b119bd1dc4e33e1c070b80d
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_crowdSaleAddr","type":"address"},{"name":"_amountForSale","type":"uint256"}],"name":"setCrowdsale","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"INITIAL_SUPPLY","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_value","type":"uint256"}],"name":"burn","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"transferEnabled","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"CROWDSALE_ALLOWANCE","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_subtractedValue","type":"uint256"}],"name":"decreaseApproval","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"adminAddr","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"crowdSaleAddr","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"crowdSaleAllowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"adminAllowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_addedValue","type":"uint256"}],"name":"increaseApproval","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"enableTransfer","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"ADMIN_ALLOWANCE","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"_admin","type":"address"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"previousOwner","type":"address"},{"indexed":true,"name":"newOwner","type":"address"}],"name":"OwnershipTransferred","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"burner","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Burn","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"}]

ABI for addr : 0x92af6067f7fe2ae488439c2b79ef7f8fc57e5ad3
[]

ABI for addr : 0x39a03cb1b1937a41b9846f4ca48822220819884f
[]

ABI for addr : 0xad41548fa92921597da3ae783a330396b47cba3c
[{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"registry","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"terminate","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"timestamp","type":"uint256"}],"name":"setDeadline","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"contributor","type":"address"}],"name":"registerUser","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"deadline","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"cap","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"beneficiary","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"contributor","type":"address"}],"name":"deactivate","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"unpause","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_cap","type":"uint256"}],"name":"setCap","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"capTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"paused","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"RATE","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"tokenReward","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"ownerSafeWithdrawal","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"startTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"amountRaised","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"pause","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"fundingCapReached","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"amountWei","type":"uint256"},{"name":"amountMiniQsp","type":"uint256"}],"name":"allocateTokens","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"GAS_LIMIT_IN_WEI","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"buy","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"minContribution","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"refundAmount","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"mainsaleBalanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"saleClosed","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"currentTime","outputs":[{"name":"_currentTime","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"fundingCap","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"contributors","type":"address[]"}],"name":"registerUsers","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[{"name":"ifSuccessfulSendTo","type":"address"},{"name":"fundingCapInEthers","type":"uint256"},{"name":"minimumContributionInWei","type":"uint256"},{"name":"start","type":"uint256"},{"name":"durationInMinutes","type":"uint256"},{"name":"initialCap","type":"uint256"},{"name":"capDurationInMinutes","type":"uint256"},{"name":"addressOfTokenUsedAsReward","type":"address"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_beneficiary","type":"address"},{"indexed":false,"name":"_amountRaised","type":"uint256"}],"name":"CapReached","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_backer","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"},{"indexed":false,"name":"_isContribution","type":"bool"}],"name":"FundTransfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"target","type":"address"},{"indexed":false,"name":"isRegistered","type":"bool"}],"name":"RegistrationStatusChanged","type":"event"},{"anonymous":false,"inputs":[],"name":"Pause","type":"event"},{"anonymous":false,"inputs":[],"name":"Unpause","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"previousOwner","type":"address"},{"indexed":true,"name":"newOwner","type":"address"}],"name":"OwnershipTransferred","type":"event"}]

*/
