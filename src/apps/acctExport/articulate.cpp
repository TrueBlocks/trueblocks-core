/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "tokenlib.h"
#include "walletlib.h"
#include "options.h"

//-----------------------------------------------------------------------
static const string_q func_acceptRegistrarTransfer = "0xea9e107a";
static const string_q func_cancelBid = "0x2525f5c1";
static const string_q func_ens = "0x3f15457f";
static const string_q func_entries = "0x267b6922";
static const string_q func_eraseNode = "0xde10f04b";
static const string_q func_finalizeAuction = "0x983b94fb";
static const string_q func_getAllowedTime = "0x13c89a8f";
static const string_q func_invalidateName = "0x15f73331";
static const string_q func_isAllowed = "0x93503337";
static const string_q func_launchLength = "0xae1a0b0c";
static const string_q func_newBid = "0xce92dced";
static const string_q func_owner = "0x8da5cb5b";
static const string_q func_registryStarted = "0x9c67f06f";
static const string_q func_releaseDeed = "0x0230a07c";
static const string_q func_rootNode = "0xfaff50a8";
static const string_q func_sealedBids = "0x5e431709";
static const string_q func_shaBid = "0x22ec1244";
static const string_q func_startAuction = "0xede8acdb";
static const string_q func_startAuctions = "0xe27fe50f";
static const string_q func_startAuctionsAndBid = "0xfebefd61";
static const string_q func_state = "0x61d585da";
static const string_q func_transfer = "0x79ce9fac";
static const string_q func_transferAnyERC20Token = "0xdc39d06d";
static const string_q func_transferOwnership = "0xf2fde38b";
static const string_q func_transferRegistrars = "0x5ddae283";
static const string_q func_unsealBid = "0x47872b42";
static const string_q func_unsealBid0122 = "0xaefc8c72";
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
static const string_q func_changeMembership = "0x824dbc9a";
static const string_q func_changeUnicorn = "0x522103fa";
static const string_q func_changeVotingRules = "0xbcca1fd3";
static const string_q func_checkProposalCode = "0xeceb2945";
static const string_q func_debatingPeriodInMinutes = "0x69bd3436";
static const string_q func_executeProposal = "0x237e9492";
static const string_q func_freezeAccount = "0xe724529c";
static const string_q func_frozenAccount = "0xb414d4b6";
static const string_q func_majorityMargin = "0xaa02a90f";
static const string_q func_memberId = "0x39106821";
static const string_q func_members = "0x5daf08ca";
static const string_q func_minimumQuorum = "0x8160f0b5";
static const string_q func_mintToken = "0x79c65068";
static const string_q func_newProposalInEther = "0xb9f256cd";
static const string_q func_newProposalInWei = "0x83876bc9";
static const string_q func_numProposals = "0x400e3949";
//static const string_q func_owner = "0x8da5cb5b";
static const string_q func_priceOfAUnicornInFinney = "0x50c34878";
static const string_q func_proposals = "0x013cf08b";
static const string_q func_spentAllowance = "0xdc3080f2";
//static const string_q func_transferOwnership = "0xf2fde38b";
static const string_q func_unicornAddress = "0xb7782455";
static const string_q func_vote = "0xd3c0715b";
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
static const string_q func_finalizeAuction2 = "0xf77282ab";
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
static const string_q func_startAuction2 = "0x6b64c769";
static const string_q func_startblock = "0x0f7875c6";
static const string_q func_token = "0xfc0c546a";
static const string_q func_tokenmultiplier = "0x5635d039";
static const string_q func_walletaddress = "0x9d3978ed";
static const string_q func_whitelisteraddress = "0xac715455";

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

        if (encoding == func_acceptRegistrarTransfer) {
            // function acceptRegistrarTransfer(bytes32 hash, address deed, uint256 registrationDate)
            // 0xea9e107a
            p->func = new CFunction;
            p->func->name = "acceptRegistrarTransfer";
            p->func->inputs.push_back(CParameter("hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            p->func->inputs.push_back(CParameter("deed", "address", str_2_Addr(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("registrationDate", "uint256", str_2_Wei("0x" + extract(params, 2*64, 64))));
            return true;

        } else if (encoding == func_cancelBid) {
            // function cancelBid(address bidder, bytes32 seal)
            // 0x2525f5c1
            p->func = new CFunction;
            p->func->name = "cancelBid";
            p->func->inputs.push_back(CParameter("bidder", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("seal", "bytes32", "0x" + extract(params, 1*64, 64)));
            return true;

        } else if (encoding == func_eraseNode) {
            // function eraseNode(bytes32[] labels)
            // 0xde10f04b
            p->func = new CFunction;
            p->func->name = "eraseNode";
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            p->func->inputs.push_back(CParameter("labels", "bytes32[]", array));
            return true;

        } else if (encoding == func_finalizeAuction) {
            // function finalizeAuction(bytes32 _hash)
            // 0x983b94fb
            p->func = new CFunction;
            p->func->name = "finalizeAuction";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_invalidateName) {
            // function invalidateName(string unhashedName)
            // 0x15f73331
            p->func = new CFunction;
            p->func->name = "invalidateName";
            p->func->inputs.push_back(CParameter("unhashedName", "string", extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_newBid) {
            // function newBid(bytes32 sealedBid)
            // 0xce92dced
            p->func = new CFunction;
            p->func->name = "newBid";
            p->func->inputs.push_back(CParameter("sealedBid", "bytes32", "0x" + extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_releaseDeed) {
            // function releaseDeed(bytes32 _hash)
            // 0x0230a07c
            p->func = new CFunction;
            p->func->name = "releaseDeed";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_startAuction) {
            // function startAuction(bytes32 _hash)
            // 0xede8acdb
            p->func = new CFunction;
            p->func->name = "startAuction";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_startAuctions) {
            // function startAuctions(bytes32[] _hashes)
            // 0xe27fe50f
            p->func = new CFunction;
            p->func->name = "startAuctions";
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            p->func->inputs.push_back(CParameter("hashes", "bytes32[]", array));
            return true;

        } else if (encoding == func_startAuctionsAndBid) {
            // function startAuctionsAndBid(bytes32[] hashes, bytes32 sealedBid)
            // 0xfebefd61
            p->func = new CFunction;
            p->func->name = "startAuctionsAndBid";
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            string_q sealedBid;
            if (array.size())
                sealedBid = array[array.size()-1];
            array.pop_back();
            p->func->inputs.push_back(CParameter("hashes", "bytes32[]", array));
            p->func->inputs.push_back(CParameter("sealedBid", "bytes32", "0x" + sealedBid));
            return true;

        } else if (encoding == func_transfer) {
            // function transfer(bytes32 _hash, address newOwner)
            // 0x79ce9fac
            p->func = new CFunction;
            p->func->name = "transfer";
            p->func->inputs.push_back(CParameter("_hash", "bytes", extract(params, 0*64, 64)));
            p->func->inputs.push_back(CParameter("newOwner", "address", str_2_Addr(extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_transferAnyERC20Token) {
            // function transferAnyERC20Token(address tokenAddress, uint256 amount)
            // 0xdc39d06d
            p->func = new CFunction;
            p->func->name = "transferAnyERC20Token";
            p->func->inputs.push_back(CParameter("tokenAddress", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address _newOwner)
            // 0xf2fde38b
            p->func = new CFunction;
            p->func->name = "transferOwnership";
            p->func->inputs.push_back(CParameter("_newOwner", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_transferRegistrars) {
            // function transferRegistrars(bytes32 _hash)
            // 0x5ddae283
            p->func = new CFunction;
            p->func->name = "transferRegistrars";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            return true;

        } else if (encoding == func_unsealBid) {
            // function unsealBid(bytes32 _hash, uint256 _value, bytes32 _salt)
            // 0x47872b42
            p->func = new CFunction;
            p->func->name = "unsealBid";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            p->func->inputs.push_back(CParameter("_value", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_salt", "bytes32", "0x" + extract(params, 2*64, 64)));
            return true;

        } else if (encoding == func_unsealBid0122) {
            // function unsealBid(bytes32 _hash, address _owner, uint256 _value, bytes32 _salt)
            // 0xaefc8c72
            p->func = new CFunction;
            p->func->name = "unsealBid0122";
            p->func->inputs.push_back(CParameter("_hash", "bytes32", "0x" + extract(params, 0*64, 64)));
            p->func->inputs.push_back(CParameter("_owner", "address", str_2_Addr(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_value", "uint256", str_2_Wei("0x" + extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("_salt", "bytes32", "0x" + extract(params, 3*64, 64)));
            return true;

        } else if (encoding == func_cancelOrder) {
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

        } else if (encoding == func_changeMembership) {
            // function changeMembership(address targetMember, uint256 voteWeight, bool canAddProposals, string memberName)
            // 0x824dbc9a
            p->func = new CFunction("changeMembership");
            p->func->inputs.push_back(CParameter("targetMember", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("voteWeight", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("canAddProposals", "bool", str_2_Int(extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("memberName", "string", extract(params, 3*64, 64)));
            return true;

        } else if (encoding == func_changeUnicorn) {
            // function changeUnicorn(uint256 newUnicornPriceInFinney, address newUnicornAddress)
            // 0x522103fa
            p->func = new CFunction("changeUnicorn");
            p->func->inputs.push_back(CParameter("newUnicornPriceInFinney", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("newUnicornAddress", "address", str_2_Addr(extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_changeVotingRules) {
            // function changeVotingRules(uint256 minimumQuorumForProposals, uint256 minutesForDebate, int256 marginOfVotesForMajority)
            // 0xbcca1fd3
            p->func = new CFunction("changeVotingRules");
            p->func->inputs.push_back(CParameter("minimumQuorumForProposals", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("minutesForDebate", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("marginOfVotesForMajority", "int256", str_2_Int(extract(params, 2*64, 64))));
            return true;

        } else if (encoding == func_executeProposal) {
            // function executeProposal(uint256 proposalNumber, bytes transactionBytecode)
            // 0x237e9492
            p->func = new CFunction("executeProposal");
            p->func->inputs.push_back(CParameter("proposalNumber", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 1*64)));
            return true;

        } else if (encoding == func_freezeAccount) {
            // function freezeAccount(address target, bool freeze)
            // 0xe724529c
            p->func = new CFunction("freezeAccount");
            p->func->inputs.push_back(CParameter("target", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("freeze", "bool", str_2_Int(extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_mintToken) {
            // function mintToken(address target, uint256 mintedAmount)
            // 0x79c65068
            p->func = new CFunction("mintToken");
            p->func->inputs.push_back(CParameter("target", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("mintedAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_newProposalInEther) {
            // function newProposalInEther(address beneficiary, uint256 etherAmount, string JobDescription, bytes transactionBytecode)
            // 0xb9f256cd
            p->func = new CFunction("newProposalInEther");
            p->func->inputs.push_back(CParameter("beneficiary", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("etherAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("JobDescription", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 3*64)));
            return true;

        } else if (encoding == func_newProposalInWei) {
            // function newProposalInWei(address beneficiary, uint256 weiAmount, string JobDescription, bytes transactionBytecode)
            // 0x83876bc9
            p->func = new CFunction("newProposalInWei");
            p->func->inputs.push_back(CParameter("beneficiary", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("weiAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("JobDescription", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 3*64)));
            return true;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address newOwner)
            // 0xf2fde38b
            p->func = new CFunction("transferOwnership");
            p->func->inputs.push_back(CParameter("newOwner", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_vote) {
            // function vote(uint256 proposalNumber, bool supportsProposal, string justificationText)
            // 0xd3c0715b
            p->func = new CFunction("vote");
            p->func->inputs.push_back(CParameter("proposalNumber", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("supportsProposal", "bool", str_2_Int(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("justificationText", "string", extract(params, 2*64, 64)));
            return true;

        } else if (encoding == func_addToPrivateSaleWhitelist) {
            // function addToPrivateSaleWhitelist(address[] _bidder_addresses)
            // 0xb6d31d97
            p->func = new CFunction("addToPrivateSaleWhitelist");
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            p->func->inputs.push_back(CParameter("_bidder_addresses", "address[]", array));
            return true;

        } else if (encoding == func_addToPublicSaleWhitelist) {
            // function addToPublicSaleWhitelist(address[] _bidder_addresses)
            // 0x8a0e5a75
            p->func = new CFunction("addToPublicSaleWhitelist");
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val + "|");
            }
            p->func->inputs.push_back(CParameter("_bidder_addresses", "address[]", array));
            return true;

        } else if (encoding == func_bid) {
            // function bid()
            // 0x1998aeef
            p->func = new CFunction("bid");
            return true;

        } else if (encoding == func_distributeGoTokens) {
            // function distributeGoTokens(address receiver_address)
            // 0x9ff90e77
            p->func = new CFunction("distributeGoTokens");
            p->func->inputs.push_back(CParameter("receiver_address", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_finalizeAuction2) {
            // function finalizeAuction()
            // 0xf77282ab
            p->func = new CFunction("finalizeAuction");
            return true;

        } else if (encoding == func_removeFromPrivateSaleWhitelist) {
            // function removeFromPrivateSaleWhitelist(address[] _bidder_addresses)
            // 0xaed83846
            p->func = new CFunction("removeFromPrivateSaleWhitelist");
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            p->func->inputs.push_back(CParameter("_bidder_addresses", "address[]", array));
            return true;

        } else if (encoding == func_removeFromPublicSaleWhitelist) {
            // function removeFromPublicSaleWhitelist(address[] _bidder_addresses)
            // 0xe22497e4
            p->func = new CFunction("removeFromPublicSaleWhitelist");
            CStringArray array;
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                array.push_back(val);
            }
            p->func->inputs.push_back(CParameter("_bidder_addresses", "address[]", array));
            return true;

        } else if (encoding == func_setup) {
            // function setup(address _token_address)
            // 0x66d38203
            p->func = new CFunction("setup");
            p->func->inputs.push_back(CParameter("_token_address", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_startAuction2) {
            // function startAuction()
            // 0x6b64c769
            p->func = new CFunction("startAuction");
            return true;

        }
        // falls through
    }


    // never returns NULL
    return articulateToken(p);
}

//-----------------------------------------------------------------------
const string_q evt_AuctionStarted = "0x87e97e825a1d1fa0c54e1d36c7506c1dea8b1efd451fe68b000cf96f7cf40003";
const string_q evt_BidRevealed = "0x7b6c4b278d165a6b33958f8ea5dfb00c8c9d4d0acf1985bef5d10786898bc3e7";
const string_q evt_HashInvalidated = "0x1f9c649fe47e58bb60f4e52f0d90e4c47a526c9f90c5113df842c025970b66ad";
const string_q evt_HashRegistered = "0x0f0c27adfd84b60b6f456b0e87cdccb1e5fb9603991588d87fa99f5b6b61e670";
const string_q evt_HashReleased = "0x292b79b9246fa2c8e77d3fe195b251f9cb839d7d038e667c069ee7708c631e16";
const string_q evt_NewBid = "0xb556ff269c1b6714f432c36431e2041d28436a73b6c3f19c021827bbdc6bfc29";
//-----------------------------------------------------------------------
static const string_q evt_Cancel = "0x1e0b760c386003e9cb9bcf4fcf3997886042859d9b6ed6320e804597fcdb28b0";
static const string_q evt_Deposit = "0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7";
static const string_q evt_Order = "0x3f7f2eda73683c21a15f9435af1028c93185b5f1fa38270762dc32be606b3e85";
static const string_q evt_Trade = "0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d";
static const string_q evt_Withdraw = "0xf341246adaac6f497bc2a656f546ab9e182111d630394f0c57c710a59a2cb567";
//-----------------------------------------------------------------------
static const string_q evt_ChangeOfRules = "0xa439d3fa452be5e0e1e24a8145e715f4fd8b9c08c96a42fd82a855a85e5d57de";
static const string_q evt_FrozenFunds = "0x48335238b4855f35377ed80f164e8c6f3c366e54ac00b96a6402d4a9814a03a5";
static const string_q evt_MembershipChanged = "0x4cea7af04e7093442a4d8d039e7b517557c4787a8cf68f900473c9bf3abfae4c";
static const string_q evt_ProposalAdded = "0x646fec02522b41e7125cfc859a64fd4f4cefd5dc3b6237ca0abe251ded1fa881";
static const string_q evt_ProposalTallied = "0xd220b7272a8b6d0d7d6bcdace67b936a8f175e6d5c1b3ee438b72256b32ab3af";
static const string_q evt_Voted = "0xc34f869b7ff431b034b7b9aea9822dac189a685e0b015c7d1be3add3f89128e8";
//-----------------------------------------------------------------------
static const string_q evt_AuctionEnded = "0x45806e512b1f4f10e33e8b3cb64d1d11d998d8c554a95e0841fc1c701278bd5d";
static const string_q evt_AuctionStarted2 = "0xf8910119ddbef5440c54532457dfe8250a10ed39e583292818f44724b9e1344c";
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
bool articulateEvent(CLogEntry *p) {

    if (p->func)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);

        if (topic_2_Str(p->topics[0]) % evt_AuctionStarted) {
            // event AuctionStarted(bytes32 indexed hash, uint256 registrationDate)
            // 0x87e97e825a1d1fa0c54e1d36c7506c1dea8b1efd451fe68b000cf96f7cf40003
            p->func = new CFunction;
            p->func->name = "AuctionStartedEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("registrationDate", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_BidRevealed) {
            // event BidRevealed(bytes32 indexed hash, address indexed owner, uint256 value, uint8 status)
            // 0x7b6c4b278d165a6b33958f8ea5dfb00c8c9d4d0acf1985bef5d10786898bc3e7
            p->func = new CFunction;
            p->func->name = "BidRevealedEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("owner", "address", str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "")));
            p->func->inputs.push_back(CParameter("value", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("status", "uint8", str_2_Uint("0x" + extract(data, 1*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_HashInvalidated) {
            // event HashInvalidated(bytes32 indexed hash, string indexed name, uint256 value, uint256 registrationDate)
            // 0x1f9c649fe47e58bb60f4e52f0d90e4c47a526c9f90c5113df842c025970b66ad
            p->func = new CFunction;
            p->func->name = "HashInvalidatedEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("name", "string", nTops > 2 ? topic_2_Str(p->topics[2]) : ""));
            p->func->inputs.push_back(CParameter("value", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("registrationDate", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_HashRegistered) {
            // event HashRegistered(bytes32 indexed hash, address indexed owner, uint256 value, uint256 registrationDate)
            // 0x0f0c27adfd84b60b6f456b0e87cdccb1e5fb9603991588d87fa99f5b6b61e670
            p->func = new CFunction;
            p->func->name = "HashRegisteredEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("owner", "address", str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "")));
            p->func->inputs.push_back(CParameter("value", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("registrationDate", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_HashReleased) {
            // event HashReleased(bytes32 indexed hash, uint256 value)
            // 0x292b79b9246fa2c8e77d3fe195b251f9cb839d7d038e667c069ee7708c631e16
            p->func = new CFunction;
            p->func->name = "HashReleasedEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("value", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_NewBid) {
            // event NewBid(bytes32 indexed hash, address indexed bidder, uint256 deposit)
            // 0xb556ff269c1b6714f432c36431e2041d28436a73b6c3f19c021827bbdc6bfc29
            p->func = new CFunction;
            p->func->name = "NewBidEvent";
            p->func->inputs.push_back(CParameter("hash", "bytes32", nTops > 1 ? topic_2_Str(p->topics[1]) : ""));
            p->func->inputs.push_back(CParameter("bidder", "address", str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "")));
            p->func->inputs.push_back(CParameter("deposit", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Cancel) {
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

        } else if (topic_2_Str(p->topics[0]) % evt_ChangeOfRules) {
            // event ChangeOfRules(uint256 minimumQuorum, uint256 debatingPeriodInMinutes, int256 majorityMargin)
            // 0xa439d3fa452be5e0e1e24a8145e715f4fd8b9c08c96a42fd82a855a85e5d57de
            p->func = new CFunction("ChangeOfRulesEvent");
            p->func->inputs.push_back(CParameter("minimumQuorum", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("debatingPeriodInMinutes", "", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("majorityMargin", "", str_2_Int("0x" + extract(data, 2*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_FrozenFunds) {
            // event FrozenFunds(address target, bool frozen)
            // 0x48335238b4855f35377ed80f164e8c6f3c366e54ac00b96a6402d4a9814a03a5
            p->func = new CFunction("FrozenFundsEvent");
            p->func->inputs.push_back(CParameter("target", "", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("frozen", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_MembershipChanged) {
            // event MembershipChanged(address member)
            // 0x4cea7af04e7093442a4d8d039e7b517557c4787a8cf68f900473c9bf3abfae4c
            p->func = new CFunction("MembershipChangedEvent");
            p->func->inputs.push_back(CParameter("member", "", str_2_Addr(extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalAdded) {
            // event ProposalAdded(uint256 proposalID, address recipient, uint256 amount, string description)
            // 0x646fec02522b41e7125cfc859a64fd4f4cefd5dc3b6237ca0abe251ded1fa881
            p->func = new CFunction("ProposalAddedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("recipient", "", str_2_Addr(extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("description", "", "0x" + extract(data, 3*64, 64)));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalTallied) {
            // event ProposalTallied(uint256 proposalID, int256 result, uint256 quorum, bool active)
            // 0xd220b7272a8b6d0d7d6bcdace67b936a8f175e6d5c1b3ee438b72256b32ab3af
            p->func = new CFunction("ProposalTalliedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("result", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("quorum", "", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("active", "", str_2_Int("0x" + extract(data, 3*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Voted) {
            // event Voted(uint256 proposalID, bool position, address voter, string justification)
            // 0xc34f869b7ff431b034b7b9aea9822dac189a685e0b015c7d1be3add3f89128e8
            p->func = new CFunction("VotedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("position", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("voter", "", str_2_Addr(extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("justification", "", "0x" + extract(data, 3*64, 64)));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_AuctionEnded) {
            // event AuctionEnded(uint256 _final_price)
            // 0x45806e512b1f4f10e33e8b3cb64d1d11d998d8c554a95e0841fc1c701278bd5d
            p->func = new CFunction("AuctionEndedEvent");
            p->func->inputs.push_back(CParameter("_final_price", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_AuctionStarted2) {
            // event AuctionStarted(uint256 indexed _auction_start_time, uint256 indexed _block_number)
            // 0xf8910119ddbef5440c54532457dfe8250a10ed39e583292818f44724b9e1344c
            p->func = new CFunction("AuctionStartedEvent");
            p->func->inputs.push_back(CParameter("_auction_start_time", "uint256", str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_block_number", "uint256", str_2_Wei(nTops > 2 ? topic_2_Str(p->topics[2]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_BidSubmission) {
            // event BidSubmission(address indexed _sender, uint256 _amount, uint256 _amount_with_bonus, uint256 _remaining_funds_to_end_auction)
            // 0x95c7f715d8ba820069888095983b8c8ef0960bd6371027a386f81ad7a03dd471
            p->func = new CFunction("BidSubmissionEvent");
            p->func->inputs.push_back(CParameter("_sender", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_amount", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("_amount_with_bonus", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_remaining_funds_to_end_auction", "uint256", str_2_Wei("0x" + extract(data, 2*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ClaimedTokens) {
            // event ClaimedTokens(address indexed _recipient, uint256 _sent_amount)
            // 0xe9aa550fd75d0d28e07fa9dd67d3ae705678776f6c4a75abd09534f93e7d7907
            p->func = new CFunction("ClaimedTokensEvent");
            p->func->inputs.push_back(CParameter("_recipient", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_sent_amount", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Deployed) {
            // event Deployed(uint256 indexed _total_supply)
            // 0xb94ae47ec9f4248692e2ecf9740b67ab493f3dcc8452bedc7d9cd911c28d1ca5
            p->func = new CFunction("DeployedEvent");
            p->func->inputs.push_back(CParameter("_total_supply", "uint256", str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Deployed423b) {
            // event Deployed(uint256 indexed _price_start, uint256 _price_constant1, uint256 _price_exponent1, uint256 _price_constant2, uint256 _price_exponent2)
            // 0x10f552ccef5a3576f823ef456145fc90b129f5728315fd5d20a35693278328c3
            p->func = new CFunction("Deployed423bEvent");
            p->func->inputs.push_back(CParameter("_price_start", "uint256", str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            p->func->inputs.push_back(CParameter("_price_constant1", "uint256", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("_price_exponent1", "uint256", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("_price_constant2", "uint256", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("_price_exponent2", "uint256", str_2_Wei("0x" + extract(data, 3*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_PrivateSaleWhitelisted) {
            // event PrivateSaleWhitelisted(address indexed who)
            // 0x3a4d818091b5f82c68d5a051412c5120883733a5f9ae246e1b51ac6a5c0cc228
            p->func = new CFunction("PrivateSaleWhitelistedEvent");
            p->func->inputs.push_back(CParameter("who", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_PublicSaleWhitelisted) {
            // event PublicSaleWhitelisted(address indexed who)
            // 0x9cb56c3c4406adeb11f1b9c8f2821408099c71151c5b593e6b5134d2ca9365e5
            p->func = new CFunction("PublicSaleWhitelistedEvent");
            p->func->inputs.push_back(CParameter("who", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_RemovedFromPrivateSaleWhitelist) {
            // event RemovedFromPrivateSaleWhitelist(address indexed who)
            // 0xb16e2862ca4a5512ecbe3359ee0899e72c16b9df25c879e716fb457bafe700ab
            p->func = new CFunction("RemovedFromPrivateSaleWhitelistEvent");
            p->func->inputs.push_back(CParameter("who", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_RemovedFromPublicSaleWhitelist) {
            // event RemovedFromPublicSaleWhitelist(address indexed who)
            // 0xdf9548ac0f1ec0772eb68261447352dd3c0ffca6dc25cbaab31ccd6c7f9bf8d8
            p->func = new CFunction("RemovedFromPublicSaleWhitelistEvent");
            p->func->inputs.push_back(CParameter("who", "address", str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "")));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Setup) {
            // event Setup()
            // 0x587930504fa5b1062f394d90f9dac9ecadd354ed23a97af9ea4e44dff4870a84
            p->func = new CFunction("SetupEvent");
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_TokensDistributed) {
            // event TokensDistributed()
            // 0xcea85459abe456c560868e61c476933dcee35a72aba5f546e93715929a696186
            p->func = new CFunction("TokensDistributedEvent");
            return true;

        }
        // fall through
    }

    // never returns NULL
    return articulateTokenEvent(p);
}

//-----------------------------------------------------------------------------
CStringArray signatures = {
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
    func_acceptRegistrarTransfer,
    func_cancelBid,
    func_ens,
    func_entries,
    func_eraseNode,
    func_finalizeAuction,
    func_getAllowedTime,
    func_invalidateName,
    func_isAllowed,
    func_launchLength,
    func_newBid,
    func_owner,
    func_registryStarted,
    func_releaseDeed,
    func_rootNode,
    func_sealedBids,
    func_shaBid,
    func_startAuction,
    func_startAuctions,
    func_startAuctionsAndBid,
    func_state,
    func_transfer,
    func_transferAnyERC20Token,
    func_transferOwnership,
    func_transferRegistrars,
    func_unsealBid,
    func_unsealBid0122,
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
    func_changeMembership,
    func_changeUnicorn,
    func_changeVotingRules,
    func_checkProposalCode,
    func_debatingPeriodInMinutes,
    func_executeProposal,
    func_freezeAccount,
    func_frozenAccount,
    func_majorityMargin,
    func_memberId,
    func_members,
    func_minimumQuorum,
    func_mintToken,
    func_newProposalInEther,
    func_newProposalInWei,
    func_numProposals,
    func_owner,
    func_priceOfAUnicornInFinney,
    func_proposals,
    func_spentAllowance,
    func_transferOwnership,
    func_unicornAddress,
    func_vote,
    func_BONUSDAY1DURATION,
    func_BONUSDAY2DURATION,
    func_BONUSDAY3DURATION,
    func_CURVECUTOFFDURATION,
    func_TOKENCLAIMWAITPERIOD,
    func_addToPrivateSaleWhitelist,
    func_addToPublicSaleWhitelist,
    func_auctionstarttime,
    func_bid,
    func_bids,
    func_bidthreshold,
    func_currentBonus,
    func_distributeGoTokens,
    func_distributoraddress,
    func_endtime,
    func_finalizeAuction,
    func_finalprice,
    func_fundsclaimed,
    func_numtokensauctioned,
    func_owneraddress,
    func_price,
    func_priceconstant1,
    func_priceconstant2,
    func_priceexponent1,
    func_priceexponent2,
    func_pricestart,
    func_privatesalestarttime,
    func_privatesalewhitelist,
    func_publicsalewhitelist,
    func_receivedwei,
    func_receivedweiwithbonus,
    func_remainingFundsToEndAuction,
    func_removeFromPrivateSaleWhitelist,
    func_removeFromPublicSaleWhitelist,
    func_setup,
    func_stage,
    func_startAuction2,
    func_startblock,
    func_token,
    func_tokenmultiplier,
    func_walletaddress,
    func_whitelisteraddress,
};

//-----------------------------------------------------------------------------
CStringArray topics = {
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
    evt_AuctionStarted,
    evt_BidRevealed,
    evt_HashInvalidated,
    evt_HashRegistered,
    evt_HashReleased,
    evt_NewBid,
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
    evt_ChangeOfRules,
    evt_FrozenFunds,
    evt_MembershipChanged,
    evt_ProposalAdded,
    evt_ProposalTallied,
    evt_Voted,
    evt_AuctionEnded,
    evt_AuctionStarted2,
    evt_BidSubmission,
    evt_ClaimedTokens,
    evt_Deployed,
    evt_Deployed423b,
    evt_PrivateSaleWhitelisted,
    evt_PublicSaleWhitelisted,
    evt_RemovedFromPrivateSaleWhitelist,
    evt_RemovedFromPublicSaleWhitelist,
    evt_Setup,
    evt_TokensDistributed,
};

/*
 ABI for addr : 0x1d40c444ad973d9ddc7c14989d96b5a01ed0afda
 {"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

 ABI for addr : 0x314159265dd8dbb310642f98f50c066173c1259b
 {"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

 ABI for addr : 0x6090a6e47849629b7245dfa1ca21d94cd15878ef
 [{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"releaseDeed","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"getAllowedTime","outputs":[{"name":"timestamp","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"unhashedName","type":"string"}],"name":"invalidateName","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"hash","type":"bytes32"},{"name":"owner","type":"address"},{"name":"value","type":"uint256"},{"name":"salt","type":"bytes32"}],"name":"shaBid","outputs":[{"name":"sealedBid","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"bidder","type":"address"},{"name":"seal","type":"bytes32"}],"name":"cancelBid","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"entries","outputs":[{"name":"","type":"uint8"},{"name":"","type":"address"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"ens","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"_value","type":"uint256"},{"name":"_salt","type":"bytes32"}],"name":"unsealBid","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"transferRegistrars","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"sealedBids","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"state","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"newOwner","type":"address"}],"name":"transfer","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"_timestamp","type":"uint256"}],"name":"isAllowed","outputs":[{"name":"allowed","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"finalizeAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"registryStarted","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"launchLength","outputs":[{"name":"","type":"uint32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"sealedBid","type":"bytes32"}],"name":"newBid","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"labels","type":"bytes32[]"}],"name":"eraseNode","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hashes","type":"bytes32[]"}],"name":"startAuctions","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"hash","type":"bytes32"},{"name":"deed","type":"address"},{"name":"registrationDate","type":"uint256"}],"name":"acceptRegistrarTransfer","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"startAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"rootNode","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"hashes","type":"bytes32[]"},{"name":"sealedBid","type":"bytes32"}],"name":"startAuctionsAndBid","outputs":[],"payable":true,"type":"function"},{"inputs":[{"name":"_ens","type":"address"},{"name":"_rootNode","type":"bytes32"},{"name":"_startDate","type":"uint256"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"AuctionStarted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"bidder","type":"address"},{"indexed":false,"name":"deposit","type":"uint256"}],"name":"NewBid","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"status","type":"uint8"}],"name":"BidRevealed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashRegistered","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"value","type":"uint256"}],"name":"HashReleased","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"name","type":"string"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashInvalidated","type":"event"}]

 ABI for addr : 0x012233b3c8177f0778d910ed88170b82de3bfe57
 [{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"releaseDeed","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"unhashedName","type":"string"}],"name":"invalidateName","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"hash","type":"bytes32"},{"name":"owner","type":"address"},{"name":"value","type":"uint256"},{"name":"salt","type":"bytes32"}],"name":"shaBid","outputs":[{"name":"sealedBid","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"bidder","type":"address"},{"name":"seal","type":"bytes32"}],"name":"cancelBid","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"entries","outputs":[{"name":"","type":"uint8"},{"name":"","type":"address"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"ens","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"transferRegistrars","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"sealedBids","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"state","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"newOwner","type":"address"}],"name":"transfer","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"finalizeAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"registryStarted","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"_owner","type":"address"},{"name":"_value","type":"uint256"},{"name":"_salt","type":"bytes32"}],"name":"unsealBid","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"sealedBid","type":"bytes32"}],"name":"newBid","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"_hashes","type":"bytes32[]"}],"name":"startAuctions","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"startAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"rootNode","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"inputs":[{"name":"_ens","type":"address"},{"name":"_rootNode","type":"bytes32"},{"name":"_startDate","type":"uint256"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"AuctionStarted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"bidder","type":"address"},{"indexed":false,"name":"deposit","type":"uint256"}],"name":"NewBid","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"status","type":"uint8"}],"name":"BidRevealed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashRegistered","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"value","type":"uint256"}],"name":"HashReleased","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"name","type":"string"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashInvalidated","type":"event"}]

 ABI for addr : 0x1122b6a0e00dce0563082b6e2953f3a943855c1f
 [{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"tokenAddress","type":"address"},{"name":"amount","type":"uint256"}],"name":"transferAnyERC20Token","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":false,"stateMutability":"nonpayable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]

 ABI for addr : 0x814964b1bceaf24e26296d031eadf134a2ca4105
 [{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"inputs":[{"name":"_initialAmount","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

 ABI for addr : 0xc9d7fec9889690bbff0a0df758d13e5a55dd7822
 {"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

 ABI for addr : 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819
 [{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"}],"name":"trade","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"}],"name":"order","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orderFills","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"cancelOrder","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"amount","type":"uint256"}],"name":"withdraw","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"depositToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"amountFilled","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"tokens","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeMake_","type":"uint256"}],"name":"changeFeeMake","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeMake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeRebate_","type":"uint256"}],"name":"changeFeeRebate","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeAccount","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"},{"name":"amount","type":"uint256"},{"name":"sender","type":"address"}],"name":"testTrade","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeAccount_","type":"address"}],"name":"changeFeeAccount","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeRebate","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"feeTake_","type":"uint256"}],"name":"changeFeeTake","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"admin_","type":"address"}],"name":"changeAdmin","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"token","type":"address"},{"name":"amount","type":"uint256"}],"name":"withdrawToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"orders","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"feeTake","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"deposit","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"accountLevelsAddr_","type":"address"}],"name":"changeAccountLevelsAddr","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"accountLevelsAddr","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"token","type":"address"},{"name":"user","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"admin","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"tokenGet","type":"address"},{"name":"amountGet","type":"uint256"},{"name":"tokenGive","type":"address"},{"name":"amountGive","type":"uint256"},{"name":"expires","type":"uint256"},{"name":"nonce","type":"uint256"},{"name":"user","type":"address"},{"name":"v","type":"uint8"},{"name":"r","type":"bytes32"},{"name":"s","type":"bytes32"}],"name":"availableVolume","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"admin_","type":"address"},{"name":"feeAccount_","type":"address"},{"name":"accountLevelsAddr_","type":"address"},{"name":"feeMake_","type":"uint256"},{"name":"feeTake_","type":"uint256"},{"name":"feeRebate_","type":"uint256"}],"payable":false,"type":"constructor"},{"payable":false,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"}],"name":"Order","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"expires","type":"uint256"},{"indexed":false,"name":"nonce","type":"uint256"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"v","type":"uint8"},{"indexed":false,"name":"r","type":"bytes32"},{"indexed":false,"name":"s","type":"bytes32"}],"name":"Cancel","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"tokenGet","type":"address"},{"indexed":false,"name":"amountGet","type":"uint256"},{"indexed":false,"name":"tokenGive","type":"address"},{"indexed":false,"name":"amountGive","type":"uint256"},{"indexed":false,"name":"get","type":"address"},{"indexed":false,"name":"give","type":"address"}],"name":"Trade","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Deposit","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"token","type":"address"},{"indexed":false,"name":"user","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"balance","type":"uint256"}],"name":"Withdraw","type":"event"}]

 ABI for addr : 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359
[{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"proposals","outputs":[{"name":"recipient","type":"address"},{"name":"amount","type":"uint256"},{"name":"description","type":"string"},{"name":"votingDeadline","type":"uint256"},{"name":"executed","type":"bool"},{"name":"proposalPassed","type":"bool"},{"name":"numberOfVotes","type":"uint256"},{"name":"currentResult","type":"int256"},{"name":"proposalHash","type":"bytes32"}],"type":"function"},{"constant":false,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"transactionBytecode","type":"bytes"}],"name":"executeProposal","outputs":[{"name":"result","type":"int256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"memberId","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"numProposals","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"priceOfAUnicornInFinney","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"newUnicornPriceInFinney","type":"uint256"},{"name":"newUnicornAddress","type":"address"}],"name":"changeUnicorn","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"members","outputs":[{"name":"member","type":"address"},{"name":"voteWeight","type":"uint256"},{"name":"canAddProposals","type":"bool"},{"name":"name","type":"string"},{"name":"memberSince","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"debatingPeriodInMinutes","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"minimumQuorum","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"targetMember","type":"address"},{"name":"voteWeight","type":"uint256"},{"name":"canAddProposals","type":"bool"},{"name":"memberName","type":"string"}],"name":"changeMembership","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"beneficiary","type":"address"},{"name":"weiAmount","type":"uint256"},{"name":"JobDescription","type":"string"},{"name":"transactionBytecode","type":"bytes"}],"name":"newProposalInWei","outputs":[{"name":"proposalID","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"majorityMargin","outputs":[{"name":"","type":"int256"}],"type":"function"},{"constant":true,"inputs":[],"name":"unicornAddress","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"beneficiary","type":"address"},{"name":"etherAmount","type":"uint256"},{"name":"JobDescription","type":"string"},{"name":"transactionBytecode","type":"bytes"}],"name":"newProposalInEther","outputs":[{"name":"proposalID","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"minimumQuorumForProposals","type":"uint256"},{"name":"minutesForDebate","type":"uint256"},{"name":"marginOfVotesForMajority","type":"int256"}],"name":"changeVotingRules","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"supportsProposal","type":"bool"},{"name":"justificationText","type":"string"}],"name":"vote","outputs":[{"name":"voteID","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"beneficiary","type":"address"},{"name":"amount","type":"uint256"},{"name":"transactionBytecode","type":"bytes"}],"name":"checkProposalCode","outputs":[{"name":"codeChecksOut","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"type":"function"},{"inputs":[{"name":"minimumQuorumForProposals","type":"uint256"},{"name":"minutesForDebate","type":"uint256"},{"name":"marginOfVotesForMajority","type":"int256"},{"name":"congressLeader","type":"address"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"recipient","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"description","type":"string"}],"name":"ProposalAdded","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"position","type":"bool"},{"indexed":false,"name":"voter","type":"address"},{"indexed":false,"name":"justification","type":"string"}],"name":"Voted","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"result","type":"int256"},{"indexed":false,"name":"quorum","type":"uint256"},{"indexed":false,"name":"active","type":"bool"}],"name":"ProposalTallied","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"member","type":"address"}],"name":"MembershipChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"minimumQuorum","type":"uint256"},{"indexed":false,"name":"debatingPeriodInMinutes","type":"uint256"},{"indexed":false,"name":"majorityMargin","type":"int256"}],"name":"ChangeOfRules","type":"event"}]

ABI for addr : 0x89205a3a3b2a69de6dbf7f01ed13b2108b2c43e7
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"target","type":"address"},{"name":"mintedAmount","type":"uint256"}],"name":"mintToken","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"frozenAccount","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"spentAllowance","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"target","type":"address"},{"name":"freeze","type":"bool"}],"name":"freezeAccount","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"type":"function"},{"inputs":[{"name":"initialSupply","type":"uint256"},{"name":"tokenName","type":"string"},{"name":"decimalUnits","type":"uint8"},{"name":"tokenSymbol","type":"string"},{"name":"centralMinter","type":"address"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"target","type":"address"},{"indexed":false,"name":"frozen","type":"bool"}],"name":"FrozenFunds","type":"event"}]
 ABI for addr : 0x423b5f62b328d0d6d44870f4eee316befa0b2df5
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"auction_address","type":"address"},{"name":"wallet_address","type":"address"},{"name":"initial_supply","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_total_supply","type":"uint256"}],"name":"Deployed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"constant":true,"inputs":[],"name":"num_tokens_auctioned","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"final_price","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"publicsalewhitelist","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"auction_start_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_constant2","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"end_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"bid","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY2_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"token_multiplier","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY3_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_start","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"bid_threshold","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"funds_claimed","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_CLAIM_WAIT_PERIOD","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"privatesale_start_time","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"whitelister_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_constant1","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"bids","outputs":[{"name":"accounted","type":"uint256"},{"name":"received","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_token_address","type":"address"}],"name":"setup","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"startAuction","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"received_wei","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_exponent1","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"owner_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"privatesalewhitelist","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"addToPublicSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"receiver_address","type":"address"}],"name":"distributeGoTokens","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"price","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price_exponent2","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"BONUS_DAY1_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"removeFromPrivateSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"remainingFundsToEndAuction","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"distributor_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"addToPrivateSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"start_block","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"stage","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"received_wei_with_bonus","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_bidder_addresses","type":"address[]"}],"name":"removeFromPublicSaleWhitelist","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"wallet_address","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"currentBonus","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"CURVE_CUTOFF_DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"finalizeAuction","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"token","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[{"name":"_wallet_address","type":"address"},{"name":"_whitelister_address","type":"address"},{"name":"_distributor_address","type":"address"},{"name":"_price_start","type":"uint256"},{"name":"_price_constant1","type":"uint256"},{"name":"_price_exponent1","type":"uint256"},{"name":"_price_constant2","type":"uint256"},{"name":"_price_exponent2","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_price_start","type":"uint256"},{"indexed":false,"name":"_price_constant1","type":"uint256"},{"indexed":false,"name":"_price_exponent1","type":"uint256"},{"indexed":false,"name":"_price_constant2","type":"uint256"},{"indexed":false,"name":"_price_exponent2","type":"uint256"}],"name":"Deployed","type":"event"},{"anonymous":false,"inputs":[],"name":"Setup","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_auction_start_time","type":"uint256"},{"indexed":true,"name":"_block_number","type":"uint256"}],"name":"AuctionStarted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_sender","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"},{"indexed":false,"name":"_amount_with_bonus","type":"uint256"},{"indexed":false,"name":"_remaining_funds_to_end_auction","type":"uint256"}],"name":"BidSubmission","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_recipient","type":"address"},{"indexed":false,"name":"_sent_amount","type":"uint256"}],"name":"ClaimedTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_final_price","type":"uint256"}],"name":"AuctionEnded","type":"event"},{"anonymous":false,"inputs":[],"name":"TokensDistributed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"PrivateSaleWhitelisted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"RemovedFromPrivateSaleWhitelist","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"PublicSaleWhitelisted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"RemovedFromPublicSaleWhitelist","type":"event"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_who","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

*/
