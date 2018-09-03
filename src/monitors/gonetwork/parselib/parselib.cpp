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
 * This monitor code is part of the 'gonetwork' monitor.
 */
#include "tokenlib.h"
#include "walletlib.h"
#include "parselib.h"

//-----------------------------------------------------------------------
void parselib_init(QUITHANDLER qh) {
    etherlib_init(qh);

    CAuctionEndedEvent::registerClass();
    CAuctionStartedEvent::registerClass();
    CBidSubmissionEvent::registerClass();
    CClaimedTokensEvent::registerClass();
    CDeployedEvent::registerClass();
    CDeployed423bEvent::registerClass();
    CPrivateSaleWhitelistedEvent::registerClass();
    CPublicSaleWhitelistedEvent::registerClass();
    CRemovedFromPrivateSaleWhitelistEvent::registerClass();
    CRemovedFromPublicSaleWhitelistEvent::registerClass();
    CSetupEvent::registerClass();
    CTokensDistributedEvent::registerClass();
    CAddToPrivateSaleWhitelist::registerClass();
    CAddToPublicSaleWhitelist::registerClass();
    CBid::registerClass();
    CDistributeGoTokens::registerClass();
    CFinalizeAuction::registerClass();
    CRemoveFromPrivateSaleWhitelist::registerClass();
    CRemoveFromPublicSaleWhitelist::registerClass();
    CSetup::registerClass();
    CStartAuction::registerClass();

    tokenlib_init();
}

//-----------------------------------------------------------------------
static const string_q func_BONUSDAY1DURATION = "0x9fe07a2d";
static const string_q func_BONUSDAY2DURATION = "0x48f03604";
static const string_q func_BONUSDAY3DURATION = "0x1386c747";
static const string_q func_CURVECUTOFFDURATION = "0x66076d4c";
static const string_q func_TOKENCLAIMWAITPERIOD = "0x66679cfe";
static const string_q func_addToPrivateSaleWhitelist = "0xb6d31d97";
static const string_q func_addToPublicSaleWhitelist = "0x8a0e5a75";
static const string_q func_auctionstarttime = "0xb73610f7";
static const string_q func_bid = "0x1998aeef";
static const string_q func_bids = "0x62ea82db";
static const string_q func_bidthreshold = "0x57c736c2";
static const string_q func_currentBonus = "0xeca73f64";
static const string_q func_distributeGoTokens = "0x9ff90e77";
static const string_q func_distributoraddress = "0x3789121c";
static const string_q func_endtime = "0x2ca126f8";
static const string_q func_finalizeAuction = "0xf77282ab";
static const string_q func_finalprice = "0x58ccaf1e";
static const string_q func_fundsclaimed = "0x8aa79920";
static const string_q func_numtokensauctioned = "0xc194a0bd";
static const string_q func_owneraddress = "0xaaa48425";
static const string_q func_price = "0xa035b1fe";
static const string_q func_priceconstant1 = "0x733442d5";
static const string_q func_priceconstant2 = "0x943d2d1f";
static const string_q func_priceexponent1 = "0xecadc936";
static const string_q func_priceexponent2 = "0x7a30f183";
static const string_q func_pricestart = "0x5fbda511";
static const string_q func_privatesalestarttime = "0xa79666ef";
static const string_q func_privatesalewhitelist = "0x83d46469";
static const string_q func_publicsalewhitelist = "0x0a177cc6";
static const string_q func_receivedwei = "0xd08f3c30";
static const string_q func_receivedweiwithbonus = "0x99bc0d04";
static const string_q func_remainingFundsToEndAuction = "0xb26209f6";
static const string_q func_removeFromPrivateSaleWhitelist = "0xaed83846";
static const string_q func_removeFromPublicSaleWhitelist = "0xe22497e4";
static const string_q func_setup = "0x66d38203";
static const string_q func_stage = "0xc040e6b8";
static const string_q func_startAuction = "0x6b64c769";
static const string_q func_startblock = "0x0f7875c6";
static const string_q func_token = "0xfc0c546a";
static const string_q func_tokenmultiplier = "0x5635d039";
static const string_q func_walletaddress = "0x9d3978ed";
static const string_q func_whitelisteraddress = "0xac715455";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_addToPrivateSaleWhitelist) {
            // function addToPrivateSaleWhitelist(address[] _bidder_addresses)
            // 0xb6d31d97
            CAddToPrivateSaleWhitelist *a = new CAddToPrivateSaleWhitelist;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->_bidder_addresses.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("addToPrivateSaleWhitelist", params, nItems, items);
            return a;

        } else if (encoding == func_addToPublicSaleWhitelist) {
            // function addToPublicSaleWhitelist(address[] _bidder_addresses)
            // 0x8a0e5a75
            CAddToPublicSaleWhitelist *a = new CAddToPublicSaleWhitelist;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->_bidder_addresses.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("addToPublicSaleWhitelist", params, nItems, items);
            return a;

        } else if (encoding == func_bid) {
            // function bid()
            // 0x1998aeef
            CBid *a = new CBid;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("bid", params, nItems, items);
            return a;

        } else if (encoding == func_distributeGoTokens) {
            // function distributeGoTokens(address receiver_address)
            // 0x9ff90e77
            CDistributeGoTokens *a = new CDistributeGoTokens;
            a->CTransaction::operator=(*p);
            a->receiver_address = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("distributeGoTokens", params, nItems, items);
            return a;

        } else if (encoding == func_finalizeAuction) {
            // function finalizeAuction()
            // 0xf77282ab
            CFinalizeAuction *a = new CFinalizeAuction;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("finalizeAuction", params, nItems, items);
            return a;

        } else if (encoding == func_removeFromPrivateSaleWhitelist) {
            // function removeFromPrivateSaleWhitelist(address[] _bidder_addresses)
            // 0xaed83846
            CRemoveFromPrivateSaleWhitelist *a = new CRemoveFromPrivateSaleWhitelist;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->_bidder_addresses.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("removeFromPrivateSaleWhitelist", params, nItems, items);
            return a;

        } else if (encoding == func_removeFromPublicSaleWhitelist) {
            // function removeFromPublicSaleWhitelist(address[] _bidder_addresses)
            // 0xe22497e4
            CRemoveFromPublicSaleWhitelist *a = new CRemoveFromPublicSaleWhitelist;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->_bidder_addresses.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("removeFromPublicSaleWhitelist", params, nItems, items);
            return a;

        } else if (encoding == func_setup) {
            // function setup(address _token_address)
            // 0x66d38203
            CSetup *a = new CSetup;
            a->CTransaction::operator=(*p);
            a->_token_address = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setup", params, nItems, items);
            return a;

        } else if (encoding == func_startAuction) {
            // function startAuction()
            // 0x6b64c769
            CStartAuction *a = new CStartAuction;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("startAuction", params, nItems, items);
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
static const string_q evt_AuctionEnded = "0x45806e512b1f4f10e33e8b3cb64d1d11d998d8c554a95e0841fc1c701278bd5d";
static const string_q evt_AuctionStarted = "0xf8910119ddbef5440c54532457dfe8250a10ed39e583292818f44724b9e1344c";
static const string_q evt_BidSubmission = "0x95c7f715d8ba820069888095983b8c8ef0960bd6371027a386f81ad7a03dd471";
static const string_q evt_ClaimedTokens = "0xe9aa550fd75d0d28e07fa9dd67d3ae705678776f6c4a75abd09534f93e7d7907";
static const string_q evt_Deployed = "0xb94ae47ec9f4248692e2ecf9740b67ab493f3dcc8452bedc7d9cd911c28d1ca5";
static const string_q evt_Deployed423b = "0x10f552ccef5a3576f823ef456145fc90b129f5728315fd5d20a35693278328c3";
static const string_q evt_PrivateSaleWhitelisted = "0x3a4d818091b5f82c68d5a051412c5120883733a5f9ae246e1b51ac6a5c0cc228";
static const string_q evt_PublicSaleWhitelisted = "0x9cb56c3c4406adeb11f1b9c8f2821408099c71151c5b593e6b5134d2ca9365e5";
static const string_q evt_RemovedFromPrivateSaleWhitelist = "0xb16e2862ca4a5512ecbe3359ee0899e72c16b9df25c879e716fb457bafe700ab";
static const string_q evt_RemovedFromPublicSaleWhitelist = "0xdf9548ac0f1ec0772eb68261447352dd3c0ffca6dc25cbaab31ccd6c7f9bf8d8";
static const string_q evt_Setup = "0x587930504fa5b1062f394d90f9dac9ecadd354ed23a97af9ea4e44dff4870a84";
static const string_q evt_TokensDistributed = "0xcea85459abe456c560868e61c476933dcee35a72aba5f546e93715929a696186";

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

        if (topic_2_Str(p->topics[0]) % evt_AuctionEnded) {
            // event AuctionEnded(uint256 _final_price)
            // 0x45806e512b1f4f10e33e8b3cb64d1d11d998d8c554a95e0841fc1c701278bd5d
            CAuctionEndedEvent *a = new CAuctionEndedEvent;
            a->CLogEntry::operator=(*p);
            a->_final_price = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("AuctionEnded", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_AuctionStarted) {
            // event AuctionStarted(uint256 indexed _auction_start_time, uint256 indexed _block_number)
            // 0xf8910119ddbef5440c54532457dfe8250a10ed39e583292818f44724b9e1344c
            CAuctionStartedEvent *a = new CAuctionStartedEvent;
            a->CLogEntry::operator=(*p);
            a->_auction_start_time = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_block_number = str_2_Wei(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("AuctionStarted", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_BidSubmission) {
            // event BidSubmission(address indexed _sender, uint256 _amount, uint256 _amount_with_bonus, uint256 _remaining_funds_to_end_auction)
            // 0x95c7f715d8ba820069888095983b8c8ef0960bd6371027a386f81ad7a03dd471
            CBidSubmissionEvent *a = new CBidSubmissionEvent;
            a->CLogEntry::operator=(*p);
            a->_sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_amount_with_bonus = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_remaining_funds_to_end_auction = str_2_Wei("0x" + extract(data, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("BidSubmission", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ClaimedTokens) {
            // event ClaimedTokens(address indexed _recipient, uint256 _sent_amount)
            // 0xe9aa550fd75d0d28e07fa9dd67d3ae705678776f6c4a75abd09534f93e7d7907
            CClaimedTokensEvent *a = new CClaimedTokensEvent;
            a->CLogEntry::operator=(*p);
            a->_recipient = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_sent_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ClaimedTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Deployed) {
            // event Deployed(uint256 indexed _total_supply)
            // 0xb94ae47ec9f4248692e2ecf9740b67ab493f3dcc8452bedc7d9cd911c28d1ca5
            CDeployedEvent *a = new CDeployedEvent;
            a->CLogEntry::operator=(*p);
            a->_total_supply = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Deployed", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Deployed423b) {
            // event Deployed(uint256 indexed _price_start, uint256 _price_constant1, uint256 _price_exponent1, uint256 _price_constant2, uint256 _price_exponent2)
            // 0x10f552ccef5a3576f823ef456145fc90b129f5728315fd5d20a35693278328c3
            CDeployed423bEvent *a = new CDeployed423bEvent;
            a->CLogEntry::operator=(*p);
            a->_price_start = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_price_constant1 = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_price_exponent1 = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_price_constant2 = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->_price_exponent2 = str_2_Wei("0x" + extract(data, 3*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Deployed423b", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_PrivateSaleWhitelisted) {
            // event PrivateSaleWhitelisted(address indexed who)
            // 0x3a4d818091b5f82c68d5a051412c5120883733a5f9ae246e1b51ac6a5c0cc228
            CPrivateSaleWhitelistedEvent *a = new CPrivateSaleWhitelistedEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("PrivateSaleWhitelisted", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_PublicSaleWhitelisted) {
            // event PublicSaleWhitelisted(address indexed who)
            // 0x9cb56c3c4406adeb11f1b9c8f2821408099c71151c5b593e6b5134d2ca9365e5
            CPublicSaleWhitelistedEvent *a = new CPublicSaleWhitelistedEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("PublicSaleWhitelisted", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_RemovedFromPrivateSaleWhitelist) {
            // event RemovedFromPrivateSaleWhitelist(address indexed who)
            // 0xb16e2862ca4a5512ecbe3359ee0899e72c16b9df25c879e716fb457bafe700ab
            CRemovedFromPrivateSaleWhitelistEvent *a = new CRemovedFromPrivateSaleWhitelistEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("RemovedFromPrivateSaleWhitelist", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_RemovedFromPublicSaleWhitelist) {
            // event RemovedFromPublicSaleWhitelist(address indexed who)
            // 0xdf9548ac0f1ec0772eb68261447352dd3c0ffca6dc25cbaab31ccd6c7f9bf8d8
            CRemovedFromPublicSaleWhitelistEvent *a = new CRemovedFromPublicSaleWhitelistEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("RemovedFromPublicSaleWhitelist", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Setup) {
            // event Setup()
            // 0x587930504fa5b1062f394d90f9dac9ecadd354ed23a97af9ea4e44dff4870a84
            CSetupEvent *a = new CSetupEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("Setup", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_TokensDistributed) {
            // event TokensDistributed()
            // 0xcea85459abe456c560868e61c476933dcee35a72aba5f546e93715929a696186
            CTokensDistributedEvent *a = new CTokensDistributedEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("TokensDistributed", params, nItems, items);
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
 ABI for addr : 0x423b5f62b328d0d6d44870f4eee316befa0b2df5
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"auction_address","type":"address"},{"name":"wallet_address","type":"address"},{"name":"initial_supply","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_total_supply","type":"uint256"}],"name":"Deployed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"constant":true,"inputs":[],"name":"num_tokens_auctioned","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"final_price","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"publicsalewhitelist","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"auction_start_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_constant2","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"end_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"bid","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY2_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"token_multiplier","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY3_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_start","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"bid_threshold","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"funds_claimed","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_CLAIM_WAIT_PERIOD","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"privatesale_start_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"whitelister_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_constant1","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"bids","outputs":[{"name":"accounted","type":"uint256"},{"name":"received","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_token_address","type":"address"}],"name":"setup","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"startAuction","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"received_wei","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_exponent1","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"owner_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"privatesalewhitelist","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"addToPublicSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"receiver_address","type":"address"}],"name":"distributeGoTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"price","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_exponent2","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY1_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"removeFromPrivateSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"remainingFundsToEndAuction","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"distributor_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"addToPrivateSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"start_block","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"stage","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"received_wei_with_bonus","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"removeFromPublicSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"wallet_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"currentBonus","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"CURVE_CUTOFF_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"finalizeAuction","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"token","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"_wallet_address","type":"address"},{"name":"_whitelister_address","type":"address"},{"name":"_distributor_address","type":"address"},{"name":"_price_start","type":"uint256"},{"name":"_price_constant1","type":"uint256"},{"name":"_price_exponent1","type":"uint256"},{"name":"_price_constant2","type":"uint256"},{"name":"_price_exponent2","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_price_start","type":"uint256"},{"indexed":false,"name":"_price_constant1","type":"uint256"},{"indexed":false,"name":"_price_exponent1","type":"uint256"},{"indexed":false,"name":"_price_constant2","type":"uint256"},{"indexed":false,"name":"_price_exponent2","type":"uint256"}],"name":"Deployed","type":"event"},{"anonymous":false,"inputs":[],"name":"Setup","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_auction_start_time","type":"uint256"},{"indexed":true,"name":"_block_number","type":"uint256"}],"name":"AuctionStarted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_sender","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"},{"indexed":false,"name":"_amount_with_bonus","type":"uint256"},{"indexed":false,"name":"_remaining_funds_to_end_auction","type":"uint256"}],"name":"BidSubmission","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_recipient","type":"address"},{"indexed":false,"name":"_sent_amount","type":"uint256"}],"name":"ClaimedTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_final_price","type":"uint256"}],"name":"AuctionEnded","type":"event"},{"anonymous":false,"inputs":[],"name":"TokensDistributed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"PrivateSaleWhitelisted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"RemovedFromPrivateSaleWhitelist","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"PublicSaleWhitelisted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"RemovedFromPublicSaleWhitelist","type":"event"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

*/
