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

 */
