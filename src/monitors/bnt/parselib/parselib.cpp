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
    CChangeEvent::registerClass();
    CContributionEvent::registerClass();
    CDestructionEvent::registerClass();
    CIssuanceEvent::registerClass();
    CNewSmartTokenEvent::registerClass();
    COwnerUpdateEvent::registerClass();
    CAcceptOwnership::registerClass();
    CAcceptTokenOwnership::registerClass();
    CAddReserve::registerClass();
    CBuy::registerClass();
    CBuy77a7::registerClass();
    CChange::registerClass();
    CContributeBTCs::registerClass();
    CContributeETH::registerClass();
    CDeposit::registerClass();
    CDestroy::registerClass();
    CDestroyTokens::registerClass();
    CDisableReservePurchases::registerClass();
    CDisableTokenTransfers::registerClass();
    CDisableTransfers::registerClass();
    CEnableRealCap::registerClass();
    CIssue::registerClass();
    CIssueTokens::registerClass();
    CSell::registerClass();
    CSetFormula::registerClass();
    CTransferOwnership::registerClass();
    CTransferTokenOwnership::registerClass();
    CUpdateReserve::registerClass();
    CWithdraw::registerClass();
    CWithdrawFromToken::registerClass();
    CWithdrawTokens::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_BTCSETHERCAP = "0x1b1e009f";
const string_q func_DURATION = "0x1be05289";
const string_q func_MAXGASPRICE = "0xba57c0fa";
const string_q func_TOKENPRICED = "0x4482ca59";
const string_q func_TOKENPRICEN = "0x9099bbe9";
const string_q func_acceptOwnership = "0x79ba5097";
const string_q func_acceptTokenOwnership = "0x38a5e016";
const string_q func_addReserve = "0xc1596534";
const string_q func_beneficiary = "0x38af3eed";
const string_q func_btcs = "0xbb632244";
const string_q func_buy = "0xa59ac6dd";
const string_q func_buy77a7 = "0xa6f2ae3a";
const string_q func_change = "0x5e5144eb";
const string_q func_changeableToken = "0x503adbf6";
const string_q func_changeableTokenCount = "0x59f8714b";
const string_q func_changerType = "0x2ee6f8b6";
const string_q func_computeRealCap = "0xe2119c80";
const string_q func_computeReturn = "0x1fc3a519";
const string_q func_contributeBTCs = "0xb3cffc6e";
const string_q func_contributeETH = "0x175323a8";
const string_q func_deposit = "0xd0e30db0";
const string_q func_destroy = "0xa24835d1";
const string_q func_destroyTokens = "0xd3ce77fe";
const string_q func_disableReservePurchases = "0xe8c78934";
const string_q func_disableTokenTransfers = "0x85d5e631";
const string_q func_disableTransfers = "0x1608f18f";
const string_q func_enableRealCap = "0xad03abc5";
const string_q func_endTime = "0x3197cbb6";
const string_q func_etherToken = "0xb8066bcb";
const string_q func_formula = "0x4b75f54f";
const string_q func_getPurchaseReturn = "0xa2c4c336";
const string_q func_getReserveBalance = "0x15226b54";
const string_q func_getReturn = "0x1e1401f8";
const string_q func_getSaleReturn = "0x72b44b2c";
const string_q func_issue = "0x867904b4";
const string_q func_issueTokens = "0x475a9fa9";
const string_q func_newOwner = "0xd4ee1d90";
const string_q func_owner = "0x8da5cb5b";
const string_q func_realEtherCapHash = "0xaa4bdd3b";
const string_q func_reserveTokenCount = "0x9b99a8e2";
const string_q func_reserveTokens = "0xd031370b";
const string_q func_reserves = "0xd66bd524";
const string_q func_sell = "0x6a272462";
const string_q func_setFormula = "0xecc06c76";
const string_q func_standard = "0x5a3b7e42";
const string_q func_startTime = "0x78e97925";
const string_q func_token = "0xfc0c546a";
const string_q func_tokenChanger = "0x387a76ce";
const string_q func_totalEtherCap = "0x34971dd6";
const string_q func_totalEtherContributed = "0xb591fc69";
const string_q func_transferOwnership = "0xf2fde38b";
const string_q func_transferTokenOwnership = "0x21e6b53d";
const string_q func_transfersEnabled = "0xbef97c87";
const string_q func_updateReserve = "0x466e561f";
const string_q func_withdraw = "0x2e1a7d4d";
const string_q func_withdrawFromToken = "0x41a5b33d";
const string_q func_withdrawTokens = "0x5e35359e";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        if (encoding == "0x" && p->to % "0xBbc79794599b19274850492394004087cBf89710")
            encoding = func_contributeETH;
        else if (encoding == "0x" && p->to % "0x77a77EcA75445841875eBb67a33d0A97Dc34d924")
            encoding = func_buy;
        else if (encoding == "0x" && p->to % "0xD76b5c2A23ef78368d8E34288B5b65D616B746aE")
            encoding = func_deposit;
        // EXISTING_CODE

        if (encoding == func_acceptOwnership) {
            // function acceptOwnership()
            // 0x79ba5097
            CAcceptOwnership *a = new CAcceptOwnership;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("acceptOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_acceptTokenOwnership) {
            // function acceptTokenOwnership()
            // 0x38a5e016
            CAcceptTokenOwnership *a = new CAcceptTokenOwnership;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("acceptTokenOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_addReserve) {
            // function addReserve(address _token, uint8 _ratio, bool _enableVirtualBalance)
            // 0xc1596534
            CAddReserve *a = new CAddReserve;
            a->CTransaction::operator=(*p);
            a->_token = str_2_Addr(extract(params, 0*64, 64));
            a->_ratio = (uint32_t)str_2_Uint(extract(params, 1*64, 64));
            a->_enableVirtualBalance = str_2_Int(extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("addReserve", params, nItems, items);
            return a;

        } else if (encoding == func_buy) {
            // function buy(address _reserveToken, uint256 _depositAmount, uint256 _minReturn)
            // 0xa59ac6dd
            CBuy *a = new CBuy;
            a->CTransaction::operator=(*p);
            a->_reserveToken = str_2_Addr(extract(params, 0*64, 64));
            a->_depositAmount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_minReturn = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("buy", params, nItems, items);
            return a;

        } else if (encoding == func_buy77a7) {
            // function buy()
            // 0xa6f2ae3a
            CBuy77a7 *a = new CBuy77a7;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("buy77a7", params, nItems, items);
            return a;

        } else if (encoding == func_change) {
            // function change(address _fromToken, address _toToken, uint256 _amount, uint256 _minReturn)
            // 0x5e5144eb
            CChange *a = new CChange;
            a->CTransaction::operator=(*p);
            a->_fromToken = str_2_Addr(extract(params, 0*64, 64));
            a->_toToken = str_2_Addr(extract(params, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 2*64, 64));
            a->_minReturn = str_2_Wei("0x" + extract(params, 3*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("change", params, nItems, items);
            return a;

        } else if (encoding == func_contributeBTCs) {
            // function contributeBTCs()
            // 0xb3cffc6e
            CContributeBTCs *a = new CContributeBTCs;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("contributeBTCs", params, nItems, items);
            return a;

        } else if (encoding == func_contributeETH) {
            // function contributeETH()
            // 0x175323a8
            CContributeETH *a = new CContributeETH;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("contributeETH", params, nItems, items);
            return a;

        } else if (encoding == func_deposit) {
            // function deposit()
            // 0xd0e30db0
            CDeposit *a = new CDeposit;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("deposit", params, nItems, items);
            return a;

        } else if (encoding == func_destroy) {
            // function destroy(address _from, uint256 _amount)
            // 0xa24835d1
            CDestroy *a = new CDestroy;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("destroy", params, nItems, items);
            return a;

        } else if (encoding == func_destroyTokens) {
            // function destroyTokens(address _from, uint256 _amount)
            // 0xd3ce77fe
            CDestroyTokens *a = new CDestroyTokens;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("destroyTokens", params, nItems, items);
            return a;

        } else if (encoding == func_disableReservePurchases) {
            // function disableReservePurchases(address _reserveToken, bool _disable)
            // 0xe8c78934
            CDisableReservePurchases *a = new CDisableReservePurchases;
            a->CTransaction::operator=(*p);
            a->_reserveToken = str_2_Addr(extract(params, 0*64, 64));
            a->_disable = str_2_Int(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("disableReservePurchases", params, nItems, items);
            return a;

        } else if (encoding == func_disableTokenTransfers) {
            // function disableTokenTransfers(bool _disable)
            // 0x85d5e631
            CDisableTokenTransfers *a = new CDisableTokenTransfers;
            a->CTransaction::operator=(*p);
            a->_disable = str_2_Int(extract(params, 0*64, 64));
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("disableTokenTransfers", params, nItems, items);
            return a;

        } else if (encoding == func_disableTransfers) {
            // function disableTransfers(bool _disable)
            // 0x1608f18f
            CDisableTransfers *a = new CDisableTransfers;
            a->CTransaction::operator=(*p);
            a->_disable = str_2_Int(extract(params, 0*64, 64));
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("disableTransfers", params, nItems, items);
            return a;

        } else if (encoding == func_enableRealCap) {
            // function enableRealCap(uint256 _cap, uint256 _key)
            // 0xad03abc5
            CEnableRealCap *a = new CEnableRealCap;
            a->CTransaction::operator=(*p);
            a->_cap = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_key = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("enableRealCap", params, nItems, items);
            return a;

        } else if (encoding == func_issue) {
            // function issue(address _to, uint256 _amount)
            // 0x867904b4
            CIssue *a = new CIssue;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("issue", params, nItems, items);
            return a;

        } else if (encoding == func_issueTokens) {
            // function issueTokens(address _to, uint256 _amount)
            // 0x475a9fa9
            CIssueTokens *a = new CIssueTokens;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("issueTokens", params, nItems, items);
            return a;

        } else if (encoding == func_sell) {
            // function sell(address _reserveToken, uint256 _sellAmount, uint256 _minReturn)
            // 0x6a272462
            CSell *a = new CSell;
            a->CTransaction::operator=(*p);
            a->_reserveToken = str_2_Addr(extract(params, 0*64, 64));
            a->_sellAmount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_minReturn = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("sell", params, nItems, items);
            return a;

        } else if (encoding == func_setFormula) {
            // function setFormula(address _formula)
            // 0xecc06c76
            CSetFormula *a = new CSetFormula;
            a->CTransaction::operator=(*p);
            a->_formula = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setFormula", params, nItems, items);
            return a;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address _newOwner)
            // 0xf2fde38b
            CTransferOwnership *a = new CTransferOwnership;
            a->CTransaction::operator=(*p);
            a->_newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("transferOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_transferTokenOwnership) {
            // function transferTokenOwnership(address _newOwner)
            // 0x21e6b53d
            CTransferTokenOwnership *a = new CTransferTokenOwnership;
            a->CTransaction::operator=(*p);
            a->_newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("transferTokenOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_updateReserve) {
            // function updateReserve(address _reserveToken, uint8 _ratio, bool _enableVirtualBalance, uint256 _virtualBalance)
            // 0x466e561f
            CUpdateReserve *a = new CUpdateReserve;
            a->CTransaction::operator=(*p);
            a->_reserveToken = str_2_Addr(extract(params, 0*64, 64));
            a->_ratio = (uint32_t)str_2_Uint(extract(params, 1*64, 64));
            a->_enableVirtualBalance = str_2_Int(extract(params, 2*64, 64));
            a->_virtualBalance = str_2_Wei("0x" + extract(params, 3*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "bool";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("updateReserve", params, nItems, items);
            return a;

        } else if (encoding == func_withdraw) {
            // function withdraw(uint256 _amount)
            // 0x2e1a7d4d
            CWithdraw *a = new CWithdraw;
            a->CTransaction::operator=(*p);
            a->_amount = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdraw", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawFromToken) {
            // function withdrawFromToken(address _token, address _to, uint256 _amount)
            // 0x41a5b33d
            CWithdrawFromToken *a = new CWithdrawFromToken;
            a->CTransaction::operator=(*p);
            a->_token = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdrawFromToken", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawTokens) {
            // function withdrawTokens(address _token, address _to, uint256 _amount)
            // 0x5e35359e
            CWithdrawTokens *a = new CWithdrawTokens;
            a->CTransaction::operator=(*p);
            a->_token = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdrawTokens", params, nItems, items);
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
const string_q evt_Change = "0x24cee3d6b5651a987362aa6216b9d34a39212f0f1967dfd48c2c3a4fc3c576dc";
const string_q evt_Contribution = "0x5f7675b09617d2c9fa4fd13058ee5877a9538f626b0308816736e83748a45040";
const string_q evt_Destruction = "0x9a1b418bc061a5d80270261562e6986a35d995f8051145f277be16103abd3453";
const string_q evt_Issuance = "0x9386c90217c323f58030f9dadcbc938f807a940f4ff41cd4cead9562f5da7dc3";
const string_q evt_NewSmartToken = "0xf4cd1f8571e8d9c97ffcb81558807ab73f9803d54de5da6a0420593c82a4a9f0";
const string_q evt_OwnerUpdate = "0x343765429aea5a34b3ff6a3785a98a5abb2597aca87bfbb58632c173d585373a";

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

        if (topic_2_Str(p->topics[0]) % evt_Change) {
            // event Change(address indexed _fromToken, address indexed _toToken, address indexed _trader, uint256 _amount, uint256 _return)
            // 0x24cee3d6b5651a987362aa6216b9d34a39212f0f1967dfd48c2c3a4fc3c576dc
            CChangeEvent *a = new CChangeEvent;
            a->CLogEntry::operator=(*p);
            a->_fromToken = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_toToken = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_trader = str_2_Addr(nTops > 3 ? topic_2_Str(p->topics[3]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_return = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Change", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Contribution) {
            // event Contribution(address indexed _contributor, uint256 _amount, uint256 _return)
            // 0x5f7675b09617d2c9fa4fd13058ee5877a9538f626b0308816736e83748a45040
            CContributionEvent *a = new CContributionEvent;
            a->CLogEntry::operator=(*p);
            a->_contributor = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_return = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Contribution", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Destruction) {
            // event Destruction(uint256 _amount)
            // 0x9a1b418bc061a5d80270261562e6986a35d995f8051145f277be16103abd3453
            CDestructionEvent *a = new CDestructionEvent;
            a->CLogEntry::operator=(*p);
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Destruction", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Issuance) {
            // event Issuance(uint256 _amount)
            // 0x9386c90217c323f58030f9dadcbc938f807a940f4ff41cd4cead9562f5da7dc3
            CIssuanceEvent *a = new CIssuanceEvent;
            a->CLogEntry::operator=(*p);
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Issuance", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewSmartToken) {
            // event NewSmartToken(address _token)
            // 0xf4cd1f8571e8d9c97ffcb81558807ab73f9803d54de5da6a0420593c82a4a9f0
            CNewSmartTokenEvent *a = new CNewSmartTokenEvent;
            a->CLogEntry::operator=(*p);
            a->_token = str_2_Addr(extract(data, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("NewSmartToken", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnerUpdate) {
            // event OwnerUpdate(address _prevOwner, address _newOwner)
            // 0x343765429aea5a34b3ff6a3785a98a5abb2597aca87bfbb58632c173d585373a
            COwnerUpdateEvent *a = new COwnerUpdateEvent;
            a->CLogEntry::operator=(*p);
            a->_prevOwner = str_2_Addr(extract(data, 0*64, 64));
            a->_newOwner = str_2_Addr(extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnerUpdate", params, nItems, items);
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
    func_BTCSETHERCAP,
    func_DURATION,
    func_MAXGASPRICE,
    func_TOKENPRICED,
    func_TOKENPRICEN,
    func_acceptOwnership,
    func_acceptTokenOwnership,
    func_addReserve,
    func_beneficiary,
    func_btcs,
    func_buy,
    func_buy77a7,
    func_change,
    func_changeableToken,
    func_changeableTokenCount,
    func_changerType,
    func_computeRealCap,
    func_computeReturn,
    func_contributeBTCs,
    func_contributeETH,
    func_deposit,
    func_destroy,
    func_destroyTokens,
    func_disableReservePurchases,
    func_disableTokenTransfers,
    func_disableTransfers,
    func_enableRealCap,
    func_endTime,
    func_etherToken,
    func_formula,
    func_getPurchaseReturn,
    func_getReserveBalance,
    func_getReturn,
    func_getSaleReturn,
    func_issue,
    func_issueTokens,
    func_newOwner,
    func_owner,
    func_realEtherCapHash,
    func_reserveTokenCount,
    func_reserveTokens,
    func_reserves,
    func_sell,
    func_setFormula,
    func_standard,
    func_startTime,
    func_token,
    func_tokenChanger,
    func_totalEtherCap,
    func_totalEtherContributed,
    func_transferOwnership,
    func_transferTokenOwnership,
    func_transfersEnabled,
    func_updateReserve,
    func_withdraw,
    func_withdrawFromToken,
    func_withdrawTokens,
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
    evt_Change,
    evt_Contribution,
    evt_Destruction,
    evt_Issuance,
    evt_NewSmartToken,
    evt_OwnerUpdate,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_disable","type":"bool"}],"name":"disableTransfers","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"standard","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"issue","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_amount","type":"uint256"}],"name":"destroy","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"transfersEnabled","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_name","type":"string"},{"name":"_symbol","type":"string"},{"name":"_decimals","type":"uint8"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_token","type":"address"}],"name":"NewSmartToken","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Issuance","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Destruction","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_prevOwner","type":"address"},{"indexed":false,"name":"_newOwner","type":"address"}],"name":"OwnerUpdate","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xd76b5c2a23ef78368d8e34288b5b65d616b746ae
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_amount","type":"uint256"}],"name":"withdraw","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"standard","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"deposit","outputs":[],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"inputs":[],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Issuance","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Destruction","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_prevOwner","type":"address"},{"indexed":false,"name":"_newOwner","type":"address"}],"name":"OwnerUpdate","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

ABI for addr : 0xca83bd8c4c7b1c0409b25fbd7e70b1ef57629ff4
[{"constant":true,"inputs":[{"name":"_reserveToken","type":"address"}],"name":"getReserveBalance","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_fromToken","type":"address"},{"name":"_toToken","type":"address"},{"name":"_amount","type":"uint256"}],"name":"getReturn","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferTokenOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"changerType","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptTokenOwnership","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawFromToken","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_ratio","type":"uint8"},{"name":"_enableVirtualBalance","type":"bool"},{"name":"_virtualBalance","type":"uint256"}],"name":"updateReserve","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"issueTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"formula","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_tokenIndex","type":"uint16"}],"name":"changeableToken","outputs":[{"name":"tokenAddress","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"changeableTokenCount","outputs":[{"name":"count","type":"uint16"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawTokens","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_fromToken","type":"address"},{"name":"_toToken","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_minReturn","type":"uint256"}],"name":"change","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_sellAmount","type":"uint256"},{"name":"_minReturn","type":"uint256"}],"name":"sell","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_sellAmount","type":"uint256"}],"name":"getSaleReturn","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_disable","type":"bool"}],"name":"disableTokenTransfers","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"reserveTokenCount","outputs":[{"name":"count","type":"uint16"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_depositAmount","type":"uint256"}],"name":"getPurchaseReturn","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_depositAmount","type":"uint256"},{"name":"_minReturn","type":"uint256"}],"name":"buy","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_ratio","type":"uint8"},{"name":"_enableVirtualBalance","type":"bool"}],"name":"addReserve","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"reserveTokens","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_amount","type":"uint256"}],"name":"destroyTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"reserves","outputs":[{"name":"virtualBalance","type":"uint256"},{"name":"ratio","type":"uint8"},{"name":"isVirtualBalanceEnabled","type":"bool"},{"name":"isPurchaseEnabled","type":"bool"},{"name":"isSet","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_reserveToken","type":"address"},{"name":"_disable","type":"bool"}],"name":"disableReservePurchases","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_formula","type":"address"}],"name":"setFormula","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"token","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"inputs":[{"name":"_token","type":"address"},{"name":"_formula","type":"address"},{"name":"_reserveToken","type":"address"},{"name":"_reserveRatio","type":"uint8"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_fromToken","type":"address"},{"indexed":true,"name":"_toToken","type":"address"},{"indexed":true,"name":"_trader","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"},{"indexed":false,"name":"_return","type":"uint256"}],"name":"Change","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_prevOwner","type":"address"},{"indexed":false,"name":"_newOwner","type":"address"}],"name":"OwnerUpdate","type":"event"}]

ABI for addr : 0x77a77eca75445841875ebb67a33d0a97dc34d924
[{"constant":true,"inputs":[],"name":"tokenChanger","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawTokens","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"buy","outputs":[{"name":"amount","type":"uint256"}],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"etherToken","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_changer","type":"address"},{"name":"_etherToken","type":"address"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_prevOwner","type":"address"},{"indexed":false,"name":"_newOwner","type":"address"}],"name":"OwnerUpdate","type":"event"}]

ABI for addr : 0xbbc79794599b19274850492394004087cbf89710
[{"constant":true,"inputs":[],"name":"BTCS_ETHER_CAP","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_PRICE_D","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"contributeETH","outputs":[{"name":"amount","type":"uint256"}],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"DURATION","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_contribution","type":"uint256"}],"name":"computeReturn","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferTokenOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"endTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalEtherCap","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptTokenOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"beneficiary","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawFromToken","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"TOKEN_PRICE_N","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"issueTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_token","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"withdrawTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"startTime","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_disable","type":"bool"}],"name":"disableTokenTransfers","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"realEtherCapHash","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_cap","type":"uint256"},{"name":"_key","type":"uint256"}],"name":"enableRealCap","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"contributeBTCs","outputs":[{"name":"amount","type":"uint256"}],"payable":true,"type":"function"},{"constant":true,"inputs":[],"name":"totalEtherContributed","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"btcs","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_amount","type":"uint256"}],"name":"destroyTokens","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_cap","type":"uint256"},{"name":"_key","type":"uint256"}],"name":"computeRealCap","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"MAX_GAS_PRICE","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"token","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"inputs":[{"name":"_token","type":"address"},{"name":"_startTime","type":"uint256"},{"name":"_beneficiary","type":"address"},{"name":"_btcs","type":"address"},{"name":"_realEtherCapHash","type":"bytes32"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_contributor","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"},{"indexed":false,"name":"_return","type":"uint256"}],"name":"Contribution","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_prevOwner","type":"address"},{"indexed":false,"name":"_newOwner","type":"address"}],"name":"OwnerUpdate","type":"event"}]

ABI for addr : 0x5894110995b8c8401bd38262ba0c8ee41d4e4658
[]

*/
