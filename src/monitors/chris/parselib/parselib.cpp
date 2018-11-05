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
    CAcceptRequestEvent::registerClass();
    CAffirmStatusEvent::registerClass();
    CBurnTokensEvent::registerClass();
    CCancelRequestEvent::registerClass();
    CMakeRequestEvent::registerClass();
    CMessageEvent::registerClass();
    CMintTokensEvent::registerClass();
    CReclaimMisplacedTokensEvent::registerClass();
    CSettingChangeEvent::registerClass();
    CToggleTrustedVerifierEvent::registerClass();
    CTransferXEvent::registerClass();
    CVerifyStatusEvent::registerClass();
    CWithdrawPaymentEvent::registerClass();
    CAcceptExistingAskRequestAndBUY::registerClass();
    CAcceptExistingBidRequestAndSELL::registerClass();
    CAffirmStatusAsAccreditedInvestor::registerClass();
    CBurnTokens::registerClass();
    CCancelExistingRequest::registerClass();
    CChangeSetting::registerClass();
    CDepositPayment::registerClass();
    CMakeNewAskRequestToSELL::registerClass();
    CMakeNewBidRequestToBUY::registerClass();
    CMintTokens::registerClass();
    CReclaimMisplacedTokens::registerClass();
    CSetFiatContractAddress::registerClass();
    CTogglePause::registerClass();
    CToggleTrustedVerifier::registerClass();
    CTransferAnyERC20Token::registerClass();
    CTransferX::registerClass();
    CVerifyStatusAsAccreditedInvestor::registerClass();
    CWithdrawExcessEther::registerClass();
    CWithdrawPayment::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_acceptExistingAskRequestAndBUY = "0x91c37694";
const string_q func_acceptExistingBidRequestAndSELL = "0x4ccac20a";
const string_q func_admin = "0xf851a440";
const string_q func_affirmStatusAsAccreditedInvestor = "0x328d06c6";
const string_q func_burnTokens = "0x6d1b229d";
const string_q func_cancelExistingRequest = "0x0a3120f2";
const string_q func_changeSetting = "0xd6918013";
const string_q func_contractAddress = "0xf6b4dfb4";
const string_q func_contractSetting = "0x0619a976";
const string_q func_depositPayment = "0xaa5e2153";
const string_q func_exchangeRate = "0x3ba0b9a9";
const string_q func_fiatContractAddress = "0x79c18ecb";
const string_q func_getHolderInfo = "0x54e4067a";
const string_q func_getHolders = "0x5fe8e7cc";
const string_q func_getRequest = "0xc58343ef";
const string_q func_getRequestKeys = "0xc8c60a3b";
const string_q func_makeNewAskRequestToSELL = "0xfaa47056";
const string_q func_makeNewBidRequestToBUY = "0xeaca750d";
const string_q func_mintTokens = "0x97304ced";
const string_q func_pause = "0x8456cb59";
const string_q func_reclaimMisplacedTokens = "0x1ec1eabf";
const string_q func_setFiatContractAddress = "0x45243120";
const string_q func_togglePause = "0xc4ae3168";
const string_q func_toggleTrustedVerifier = "0x465f7840";
const string_q func_totalEtherDepositsMade = "0x5f71ccbc";
const string_q func_totalEtherPaymentsDue = "0x56ef80f5";
const string_q func_totalTokensCommitted = "0x919e93b5";
const string_q func_transferAnyERC20Token = "0xdc39d06d";
const string_q func_transferX = "0x39df656a";
const string_q func_verifyStatusAsAccreditedInvestor = "0x21daa08e";
const string_q func_withdrawExcessEther = "0x97d7392d";
const string_q func_withdrawPayment = "0xa118babd";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_acceptExistingAskRequestAndBUY) {
            // function acceptExistingAskRequestAndBUY(uint256 _key, uint256 _tokenAmount, uint256 _dollarPrice)
            // 0x91c37694
            CAcceptExistingAskRequestAndBUY *a = new CAcceptExistingAskRequestAndBUY;
            a->CTransaction::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_tokenAmount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("acceptExistingAskRequestAndBUY", params, nItems, items);
            return a;

        } else if (encoding == func_acceptExistingBidRequestAndSELL) {
            // function acceptExistingBidRequestAndSELL(uint256 _key, uint256 _tokenAmount, uint256 _dollarPrice)
            // 0x4ccac20a
            CAcceptExistingBidRequestAndSELL *a = new CAcceptExistingBidRequestAndSELL;
            a->CTransaction::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_tokenAmount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("acceptExistingBidRequestAndSELL", params, nItems, items);
            return a;

        } else if (encoding == func_affirmStatusAsAccreditedInvestor) {
            // function affirmStatusAsAccreditedInvestor(address _account)
            // 0x328d06c6
            CAffirmStatusAsAccreditedInvestor *a = new CAffirmStatusAsAccreditedInvestor;
            a->CTransaction::operator=(*p);
            a->_account = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("affirmStatusAsAccreditedInvestor", params, nItems, items);
            return a;

        } else if (encoding == func_burnTokens) {
            // function burnTokens(uint256 _amount)
            // 0x6d1b229d
            CBurnTokens *a = new CBurnTokens;
            a->CTransaction::operator=(*p);
            a->_amount = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("burnTokens", params, nItems, items);
            return a;

        } else if (encoding == func_cancelExistingRequest) {
            // function cancelExistingRequest(uint256 _key)
            // 0x0a3120f2
            CCancelExistingRequest *a = new CCancelExistingRequest;
            a->CTransaction::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("cancelExistingRequest", params, nItems, items);
            return a;

        } else if (encoding == func_changeSetting) {
            // function changeSetting(uint8 _setting, uint256 _value)
            // 0xd6918013
            CChangeSetting *a = new CChangeSetting;
            a->CTransaction::operator=(*p);
            a->_setting = (uint32_t)str_2_Uint(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint8";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeSetting", params, nItems, items);
            return a;

        } else if (encoding == func_depositPayment) {
            // function depositPayment()
            // 0xaa5e2153
            CDepositPayment *a = new CDepositPayment;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("depositPayment", params, nItems, items);
            return a;

        } else if (encoding == func_makeNewAskRequestToSELL) {
            // function makeNewAskRequestToSELL(uint256 _tokenAmount, uint256 _dollarPrice)
            // 0xfaa47056
            CMakeNewAskRequestToSELL *a = new CMakeNewAskRequestToSELL;
            a->CTransaction::operator=(*p);
            a->_tokenAmount = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("makeNewAskRequestToSELL", params, nItems, items);
            return a;

        } else if (encoding == func_makeNewBidRequestToBUY) {
            // function makeNewBidRequestToBUY(uint256 _tokenAmount, uint256 _dollarPrice)
            // 0xeaca750d
            CMakeNewBidRequestToBUY *a = new CMakeNewBidRequestToBUY;
            a->CTransaction::operator=(*p);
            a->_tokenAmount = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("makeNewBidRequestToBUY", params, nItems, items);
            return a;

        } else if (encoding == func_mintTokens) {
            // function mintTokens(uint256 _amount)
            // 0x97304ced
            CMintTokens *a = new CMintTokens;
            a->CTransaction::operator=(*p);
            a->_amount = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("mintTokens", params, nItems, items);
            return a;

        } else if (encoding == func_reclaimMisplacedTokens) {
            // function reclaimMisplacedTokens(address _account)
            // 0x1ec1eabf
            CReclaimMisplacedTokens *a = new CReclaimMisplacedTokens;
            a->CTransaction::operator=(*p);
            a->_account = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("reclaimMisplacedTokens", params, nItems, items);
            return a;

        } else if (encoding == func_setFiatContractAddress) {
            // function setFiatContractAddress(address _address)
            // 0x45243120
            CSetFiatContractAddress *a = new CSetFiatContractAddress;
            a->CTransaction::operator=(*p);
            a->_address = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setFiatContractAddress", params, nItems, items);
            return a;

        } else if (encoding == func_togglePause) {
            // function togglePause()
            // 0xc4ae3168
            CTogglePause *a = new CTogglePause;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("togglePause", params, nItems, items);
            return a;

        } else if (encoding == func_toggleTrustedVerifier) {
            // function toggleTrustedVerifier(address _verifier)
            // 0x465f7840
            CToggleTrustedVerifier *a = new CToggleTrustedVerifier;
            a->CTransaction::operator=(*p);
            a->_verifier = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("toggleTrustedVerifier", params, nItems, items);
            return a;

        } else if (encoding == func_transferAnyERC20Token) {
            // function transferAnyERC20Token(address _tokenAddress, uint256 _tokens)
            // 0xdc39d06d
            CTransferAnyERC20Token *a = new CTransferAnyERC20Token;
            a->CTransaction::operator=(*p);
            a->_tokenAddress = str_2_Addr(extract(params, 0*64, 64));
            a->_tokens = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("transferAnyERC20Token", params, nItems, items);
            return a;

        } else if (encoding == func_transferX) {
            // function transferX(address _to, uint256 _amount, string _transferMetaData)
            // 0x39df656a
            CTransferX *a = new CTransferX;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_transferMetaData = extract(params, 2*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            a->articulatedTx = decodeRLP("transferX", params, nItems, items);
            return a;

        } else if (encoding == func_verifyStatusAsAccreditedInvestor) {
            // function verifyStatusAsAccreditedInvestor(address _account, string _verifyMetaData)
            // 0x21daa08e
            CVerifyStatusAsAccreditedInvestor *a = new CVerifyStatusAsAccreditedInvestor;
            a->CTransaction::operator=(*p);
            a->_account = str_2_Addr(extract(params, 0*64, 64));
            a->_verifyMetaData = extract(params, 1*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "string";
            a->articulatedTx = decodeRLP("verifyStatusAsAccreditedInvestor", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawExcessEther) {
            // function withdrawExcessEther()
            // 0x97d7392d
            CWithdrawExcessEther *a = new CWithdrawExcessEther;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdrawExcessEther", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawPayment) {
            // function withdrawPayment()
            // 0xa118babd
            CWithdrawPayment *a = new CWithdrawPayment;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdrawPayment", params, nItems, items);
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
const string_q evt_AcceptRequest = "0xc62f8b1313b549bc5c94efe776ee0559a8f88efdb3db504331857fc163553c71";
const string_q evt_AffirmStatus = "0x561d06141a26ac6edf895e7e79d3002d07dd7689b6dcec5ea29f4aba8922caf0";
const string_q evt_BurnTokens = "0xf1337af994b33c56a7ce51266931ae8dedce9d6eb9b3eb127fcf7f006374f74e";
const string_q evt_CancelRequest = "0xc08c07687f5dc22e00846ec2d0afff7fb5d509f6fc5c60ad9f70fdeb0ab6a5c7";
const string_q evt_MakeRequest = "0x45ec157657ca4ad92c9a421e4b776d16042d8b7e497133b925e95bade7977c9f";
const string_q evt_Message = "0x8252da6cc8ba70656d63b4439295f2de59ffee7cf90c96f5aee10857159474e7";
const string_q evt_MintTokens = "0xcadacfb8c0e49204cf16077a3391d064f2d96ca5279ac8952e9fe94a586820a2";
const string_q evt_ReclaimMisplacedTokens = "0x21cb51cd5a443947666669cbd7817449c0c8655db9fdcbb54361bd7d5a3f6636";
const string_q evt_SettingChange = "0x5d3b22fc1b5791b3bd0e715ed4a863225e41557da6054af84331354b3caa5da6";
const string_q evt_ToggleTrustedVerifier = "0x023b74e8ede17df2114fd7c1891458b2191a5e35f416f6e0516d27a436586b36";
const string_q evt_TransferX = "0xc620c23091c77c537ae2f7deb703ecdea8b2cb3df9053ac018b281ffa1a9cae4";
const string_q evt_VerifyStatus = "0x915343c82f85b094d9cd849a66a07e32f510471c64a8d62cbd36f23148242b72";
const string_q evt_WithdrawPayment = "0x37bf10b25ac7f114ebfb7b1d314deb36f7b89f2d9b3f155f61ea8a23af43339e";

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

        if (topic_2_Str(p->topics[0]) % evt_AcceptRequest) {
            // event AcceptRequest(uint256 _key, uint256 _etherAmount, uint256 _tokenAmount, uint256 _dollarPrice, address indexed _acceptor)
            // 0xc62f8b1313b549bc5c94efe776ee0559a8f88efdb3db504331857fc163553c71
            CAcceptRequestEvent *a = new CAcceptRequestEvent;
            a->CLogEntry::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_etherAmount = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_tokenAmount = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->_acceptor = str_2_Addr(nTops > 5 ? topic_2_Str(p->topics[5]) : "");
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("AcceptRequest", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_AffirmStatus) {
            // event AffirmStatus(address indexed _sender, address indexed _account, uint256 _affirmDate, uint256 _value)
            // 0x561d06141a26ac6edf895e7e79d3002d07dd7689b6dcec5ea29f4aba8922caf0
            CAffirmStatusEvent *a = new CAffirmStatusEvent;
            a->CLogEntry::operator=(*p);
            a->_sender = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_account = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_affirmDate = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("AffirmStatus", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_BurnTokens) {
            // event BurnTokens(uint256 _value, uint256 _balance)
            // 0xf1337af994b33c56a7ce51266931ae8dedce9d6eb9b3eb127fcf7f006374f74e
            CBurnTokensEvent *a = new CBurnTokensEvent;
            a->CLogEntry::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_balance = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("BurnTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_CancelRequest) {
            // event CancelRequest(uint256 _key, address indexed _sender, uint256 _deposit, uint256 _amount)
            // 0xc08c07687f5dc22e00846ec2d0afff7fb5d509f6fc5c60ad9f70fdeb0ab6a5c7
            CCancelRequestEvent *a = new CCancelRequestEvent;
            a->CLogEntry::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_sender = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_deposit = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_amount = str_2_Wei("0x" + extract(data, 2*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("CancelRequest", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_MakeRequest) {
            // event MakeRequest(uint256 _key, uint256 _date, uint256 _tokenAmount, uint256 _dollarPrice, uint8 _requestType, address indexed _requester)
            // 0x45ec157657ca4ad92c9a421e4b776d16042d8b7e497133b925e95bade7977c9f
            CMakeRequestEvent *a = new CMakeRequestEvent;
            a->CLogEntry::operator=(*p);
            a->_key = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_date = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->_tokenAmount = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->_dollarPrice = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->_requestType = (uint32_t)str_2_Uint("0x" + extract(data, 4*64, 64));
            a->_requester = str_2_Addr(nTops > 6 ? topic_2_Str(p->topics[6]) : "");
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint8";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("MakeRequest", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Message) {
            // event Message(string fName, string message)
            // 0x8252da6cc8ba70656d63b4439295f2de59ffee7cf90c96f5aee10857159474e7
            CMessageEvent *a = new CMessageEvent;
            a->CLogEntry::operator=(*p);
            a->fName = "0x" + extract(data, 0*64, 64);
            a->message = "0x" + extract(data, 1*64, 64);
            items[nItems++] = "string";
            items[nItems++] = "string";
            a->articulatedLog = decodeRLP("Message", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_MintTokens) {
            // event MintTokens(uint256 _value, uint256 _balance)
            // 0xcadacfb8c0e49204cf16077a3391d064f2d96ca5279ac8952e9fe94a586820a2
            CMintTokensEvent *a = new CMintTokensEvent;
            a->CLogEntry::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_balance = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("MintTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ReclaimMisplacedTokens) {
            // event ReclaimMisplacedTokens(address indexed _from, uint256 _tokenAmount)
            // 0x21cb51cd5a443947666669cbd7817449c0c8655db9fdcbb54361bd7d5a3f6636
            CReclaimMisplacedTokensEvent *a = new CReclaimMisplacedTokensEvent;
            a->CLogEntry::operator=(*p);
            a->_from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_tokenAmount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ReclaimMisplacedTokens", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_SettingChange) {
            // event SettingChange(address _admin, uint8 _setting, uint256 _value)
            // 0x5d3b22fc1b5791b3bd0e715ed4a863225e41557da6054af84331354b3caa5da6
            CSettingChangeEvent *a = new CSettingChangeEvent;
            a->CLogEntry::operator=(*p);
            a->_admin = str_2_Addr(extract(data, 0*64, 64));
            a->_setting = (uint32_t)str_2_Uint("0x" + extract(data, 1*64, 64));
            a->_value = str_2_Wei("0x" + extract(data, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint8";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("SettingChange", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ToggleTrustedVerifier) {
            // event ToggleTrustedVerifier(address indexed _verifier, bool _isTrustedVerifier)
            // 0x023b74e8ede17df2114fd7c1891458b2191a5e35f416f6e0516d27a436586b36
            CToggleTrustedVerifierEvent *a = new CToggleTrustedVerifierEvent;
            a->CLogEntry::operator=(*p);
            a->_verifier = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_isTrustedVerifier = str_2_Int("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedLog = decodeRLP("ToggleTrustedVerifier", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_TransferX) {
            // event TransferX(address indexed _from, address indexed _to, uint256 _value, string _transferMetaData)
            // 0xc620c23091c77c537ae2f7deb703ecdea8b2cb3df9053ac018b281ffa1a9cae4
            CTransferXEvent *a = new CTransferXEvent;
            a->CLogEntry::operator=(*p);
            a->_from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_value = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_transferMetaData = "0x" + extract(data, 1*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            a->articulatedLog = decodeRLP("TransferX", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_VerifyStatus) {
            // event VerifyStatus(address indexed _verifier, address indexed _account, uint256 _verifyDate, string _verifyMetaData)
            // 0x915343c82f85b094d9cd849a66a07e32f510471c64a8d62cbd36f23148242b72
            CVerifyStatusEvent *a = new CVerifyStatusEvent;
            a->CLogEntry::operator=(*p);
            a->_verifier = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_account = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_verifyDate = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->_verifyMetaData = "0x" + extract(data, 1*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            a->articulatedLog = decodeRLP("VerifyStatus", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_WithdrawPayment) {
            // event WithdrawPayment(address indexed _account, uint256 _payment)
            // 0x37bf10b25ac7f114ebfb7b1d314deb36f7b89f2d9b3f155f61ea8a23af43339e
            CWithdrawPaymentEvent *a = new CWithdrawPaymentEvent;
            a->CLogEntry::operator=(*p);
            a->_account = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_payment = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("WithdrawPayment", params, nItems, items);
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
    func_acceptExistingAskRequestAndBUY,
    func_acceptExistingBidRequestAndSELL,
    func_admin,
    func_affirmStatusAsAccreditedInvestor,
    func_burnTokens,
    func_cancelExistingRequest,
    func_changeSetting,
    func_contractAddress,
    func_contractSetting,
    func_depositPayment,
    func_exchangeRate,
    func_fiatContractAddress,
    func_getHolderInfo,
    func_getHolders,
    func_getRequest,
    func_getRequestKeys,
    func_makeNewAskRequestToSELL,
    func_makeNewBidRequestToBUY,
    func_mintTokens,
    func_pause,
    func_reclaimMisplacedTokens,
    func_setFiatContractAddress,
    func_togglePause,
    func_toggleTrustedVerifier,
    func_totalEtherDepositsMade,
    func_totalEtherPaymentsDue,
    func_totalTokensCommitted,
    func_transferAnyERC20Token,
    func_transferX,
    func_verifyStatusAsAccreditedInvestor,
    func_withdrawExcessEther,
    func_withdrawPayment,
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
    evt_AcceptRequest,
    evt_AffirmStatus,
    evt_BurnTokens,
    evt_CancelRequest,
    evt_MakeRequest,
    evt_Message,
    evt_MintTokens,
    evt_ReclaimMisplacedTokens,
    evt_SettingChange,
    evt_ToggleTrustedVerifier,
    evt_TransferX,
    evt_VerifyStatus,
    evt_WithdrawPayment,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xf8faffbd5a3e3b5abf567521d1811b96036a300f
[{"constant":true,"inputs":[{"name":"_setting","type":"uint8"}],"name":"contractSetting","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_key","type":"uint256"}],"name":"cancelExistingRequest","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_account","type":"address"}],"name":"reclaimMisplacedTokens","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_account","type":"address"},{"name":"_verifyMetaData","type":"string"}],"name":"verifyStatusAsAccreditedInvestor","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_account","type":"address"}],"name":"affirmStatusAsAccreditedInvestor","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_transferMetaData","type":"string"}],"name":"transferX","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"exchangeRate","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_address","type":"address"}],"name":"setFiatContractAddress","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_verifier","type":"address"}],"name":"toggleTrustedVerifier","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_key","type":"uint256"},{"name":"_tokenAmount","type":"uint256"},{"name":"_dollarPrice","type":"uint256"}],"name":"acceptExistingBidRequestAndSELL","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_address","type":"address"}],"name":"getHolderInfo","outputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"string"},{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"totalEtherPaymentsDue","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"totalEtherDepositsMade","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"getHolders","outputs":[{"name":"","type":"address[]"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_amount","type":"uint256"}],"name":"burnTokens","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"tokenOwner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"fiatContractAddress","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"pause","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"totalTokensCommitted","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_key","type":"uint256"},{"name":"_tokenAmount","type":"uint256"},{"name":"_dollarPrice","type":"uint256"}],"name":"acceptExistingAskRequestAndBUY","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_amount","type":"uint256"}],"name":"mintTokens","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"withdrawExcessEther","outputs":[{"name":"amount","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"withdrawPayment","outputs":[{"name":"paymentMade","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_amount","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"depositPayment","outputs":[{"name":"","type":"bool"}],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[],"name":"togglePause","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_key","type":"uint256"}],"name":"getRequest","outputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"address"},{"name":"","type":"bool"},{"name":"","type":"uint256"},{"name":"","type":"address"},{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"getRequestKeys","outputs":[{"name":"","type":"uint256[]"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_setting","type":"uint8"},{"name":"_value","type":"uint256"}],"name":"changeSetting","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_tokenAddress","type":"address"},{"name":"_tokens","type":"uint256"}],"name":"transferAnyERC20Token","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_tokenAmount","type":"uint256"},{"name":"_dollarPrice","type":"uint256"}],"name":"makeNewBidRequestToBUY","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"contractAddress","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"admin","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_tokenAmount","type":"uint256"},{"name":"_dollarPrice","type":"uint256"}],"name":"makeNewAskRequestToSELL","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_sender","type":"address"},{"indexed":true,"name":"_account","type":"address"},{"indexed":false,"name":"_affirmDate","type":"uint256"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"AffirmStatus","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_verifier","type":"address"},{"indexed":true,"name":"_account","type":"address"},{"indexed":false,"name":"_verifyDate","type":"uint256"},{"indexed":false,"name":"_verifyMetaData","type":"string"}],"name":"VerifyStatus","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"},{"indexed":false,"name":"_transferMetaData","type":"string"}],"name":"TransferX","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_key","type":"uint256"},{"indexed":false,"name":"_date","type":"uint256"},{"indexed":false,"name":"_tokenAmount","type":"uint256"},{"indexed":false,"name":"_dollarPrice","type":"uint256"},{"indexed":false,"name":"_requestType","type":"uint8"},{"indexed":true,"name":"_requester","type":"address"}],"name":"MakeRequest","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_key","type":"uint256"},{"indexed":true,"name":"_sender","type":"address"},{"indexed":false,"name":"_deposit","type":"uint256"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"CancelRequest","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_key","type":"uint256"},{"indexed":false,"name":"_etherAmount","type":"uint256"},{"indexed":false,"name":"_tokenAmount","type":"uint256"},{"indexed":false,"name":"_dollarPrice","type":"uint256"},{"indexed":true,"name":"_acceptor","type":"address"}],"name":"AcceptRequest","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_account","type":"address"},{"indexed":false,"name":"_payment","type":"uint256"}],"name":"WithdrawPayment","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_value","type":"uint256"},{"indexed":false,"name":"_balance","type":"uint256"}],"name":"MintTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_value","type":"uint256"},{"indexed":false,"name":"_balance","type":"uint256"}],"name":"BurnTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":false,"name":"_tokenAmount","type":"uint256"}],"name":"ReclaimMisplacedTokens","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_verifier","type":"address"},{"indexed":false,"name":"_isTrustedVerifier","type":"bool"}],"name":"ToggleTrustedVerifier","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_admin","type":"address"},{"indexed":false,"name":"_setting","type":"uint8"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"SettingChange","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"fName","type":"string"},{"indexed":false,"name":"message","type":"string"}],"name":"Message","type":"event"}]

*/
