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
    CActivate::registerClass();
    CBuyXaddr::registerClass();
    CBuyXid::registerClass();
    CBuyXname::registerClass();
    CCancelMigration::registerClass();
    CDeposit::registerClass();
    CFinishMigration::registerClass();
    COnAffiliatePayoutEvent::registerClass();
    COnBuyAndDistributeEvent::registerClass();
    COnEndTxEvent::registerClass();
    COnNewNameEvent::registerClass();
    COnPotSwapDepositEvent::registerClass();
    COnReLoadAndDistributeEvent::registerClass();
    COnWithdrawEvent::registerClass();
    COnWithdrawAndDistributeEvent::registerClass();
    CPotSwap::registerClass();
    CReLoadXaddr::registerClass();
    CReLoadXid::registerClass();
    CReLoadXname::registerClass();
    CReceivePlayerInfo::registerClass();
    CReceivePlayerNameList::registerClass();
    CRegisterNameXID::registerClass();
    CRegisterNameXaddr::registerClass();
    CRegisterNameXname::registerClass();
    CSetOtherFomo::registerClass();
    CSetup::registerClass();
    CStartMigration::registerClass();
    CWithdraw::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_activate = "0x0f15f4c0";
const string_q func_activated = "0x186601ca";
const string_q func_airDropPot = "0x0c06b91d";
const string_q func_airDropTracker = "0xf42e8446";
const string_q func_buyXaddr = "0x98a0871d";
const string_q func_buyXid = "0x8f38f309";
const string_q func_buyXname = "0xa65b37a1";
const string_q func_calcKeysReceived = "0xce89c80c";
const string_q func_cancelMigration = "0x10639ea0";
const string_q func_deposit = "0xd0e30db0";
const string_q func_fees = "0x4acc79ed";
const string_q func_finishMigration = "0x88d761f2";
const string_q func_getBuyPrice = "0x018a25e8";
const string_q func_getCurrentRoundInfo = "0x747dff42";
const string_q func_getPlayerInfoByAddress = "0xee0b5d8b";
const string_q func_getPlayerVaults = "0x63066434";
const string_q func_getTimeLeft = "0xc7e284b8";
const string_q func_iWantXKeys = "0xcf808000";
const string_q func_pIDxAddr = "0x045dd93b";
const string_q func_pIDxName = "0x9374327e";
const string_q func_plyr = "0x54c6de48";
const string_q func_plyrNames = "0x2722cd34";
const string_q func_plyrRnds = "0xa7f354a5";
const string_q func_potSplit = "0x0d3682d9";
const string_q func_potSwap = "0xed78cf4a";
const string_q func_rID = "0x9b6ed777";
const string_q func_reLoadXaddr = "0x82bfc739";
const string_q func_reLoadXid = "0x349cdcac";
const string_q func_reLoadXname = "0x079ce327";
const string_q func_receivePlayerInfo = "0x49cc635d";
const string_q func_receivePlayerNameList = "0x8f7140ea";
const string_q func_registerNameXID = "0x921dec21";
const string_q func_registerNameXaddr = "0x3ddd4698";
const string_q func_registerNameXname = "0x685ffd83";
const string_q func_rndTmEth = "0xe7409e4d";
const string_q func_round = "0x34d5f37b";
const string_q func_setOtherFomo = "0xb483c054";
const string_q func_setup = "0x66d38203";
const string_q func_startMigration = "0xa0f52da0";
const string_q func_status = "0x200d2ed2";
const string_q func_withdraw = "0x3ccfd60b";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_activate) {
            // function activate()
            // 0x0f15f4c0
            CActivate *a = new CActivate;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("activate", params, nItems, items);
            return a;

        } else if (encoding == func_buyXaddr) {
            // function buyXaddr(address _affCode, uint256 _team)
            // 0x98a0871d
            CBuyXaddr *a = new CBuyXaddr;
            a->CTransaction::operator=(*p);
            a->_affCode = str_2_Addr(extract(params, 0*64, 64));
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("buyXaddr", params, nItems, items);
            return a;

        } else if (encoding == func_buyXid) {
            // function buyXid(uint256 _affCode, uint256 _team)
            // 0x8f38f309
            CBuyXid *a = new CBuyXid;
            a->CTransaction::operator=(*p);
            a->_affCode = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("buyXid", params, nItems, items);
            return a;

        } else if (encoding == func_buyXname) {
            // function buyXname(bytes32 _affCode, uint256 _team)
            // 0xa65b37a1
            CBuyXname *a = new CBuyXname;
            a->CTransaction::operator=(*p);
            a->_affCode = extract(params, 0*64, 64);
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("buyXname", params, nItems, items);
            return a;

        } else if (encoding == func_cancelMigration) {
            // function cancelMigration()
            // 0x10639ea0
            CCancelMigration *a = new CCancelMigration;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("cancelMigration", params, nItems, items);
            return a;

        } else if (encoding == func_deposit) {
            // function deposit()
            // 0xd0e30db0
            CDeposit *a = new CDeposit;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("deposit", params, nItems, items);
            return a;

        } else if (encoding == func_finishMigration) {
            // function finishMigration()
            // 0x88d761f2
            CFinishMigration *a = new CFinishMigration;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("finishMigration", params, nItems, items);
            return a;

        } else if (encoding == func_potSwap) {
            // function potSwap()
            // 0xed78cf4a
            CPotSwap *a = new CPotSwap;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("potSwap", params, nItems, items);
            return a;

        } else if (encoding == func_reLoadXaddr) {
            // function reLoadXaddr(address _affCode, uint256 _team, uint256 _eth)
            // 0x82bfc739
            CReLoadXaddr *a = new CReLoadXaddr;
            a->CTransaction::operator=(*p);
            a->_affCode = str_2_Addr(extract(params, 0*64, 64));
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_eth = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("reLoadXaddr", params, nItems, items);
            return a;

        } else if (encoding == func_reLoadXid) {
            // function reLoadXid(uint256 _affCode, uint256 _team, uint256 _eth)
            // 0x349cdcac
            CReLoadXid *a = new CReLoadXid;
            a->CTransaction::operator=(*p);
            a->_affCode = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_eth = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("reLoadXid", params, nItems, items);
            return a;

        } else if (encoding == func_reLoadXname) {
            // function reLoadXname(bytes32 _affCode, uint256 _team, uint256 _eth)
            // 0x079ce327
            CReLoadXname *a = new CReLoadXname;
            a->CTransaction::operator=(*p);
            a->_affCode = extract(params, 0*64, 64);
            a->_team = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_eth = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("reLoadXname", params, nItems, items);
            return a;

        } else if (encoding == func_receivePlayerInfo) {
            // function receivePlayerInfo(uint256 _pID, address _addr, bytes32 _name, uint256 _laff)
            // 0x49cc635d
            CReceivePlayerInfo *a = new CReceivePlayerInfo;
            a->CTransaction::operator=(*p);
            a->_pID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_addr = str_2_Addr(extract(params, 1*64, 64));
            a->_name = extract(params, 2*64, 64);
            a->_laff = str_2_Wei("0x" + extract(params, 3*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("receivePlayerInfo", params, nItems, items);
            return a;

        } else if (encoding == func_receivePlayerNameList) {
            // function receivePlayerNameList(uint256 _pID, bytes32 _name)
            // 0x8f7140ea
            CReceivePlayerNameList *a = new CReceivePlayerNameList;
            a->CTransaction::operator=(*p);
            a->_pID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_name = extract(params, 1*64, 64);
            items[nItems++] = "uint256";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("receivePlayerNameList", params, nItems, items);
            return a;

        } else if (encoding == func_registerNameXID) {
            // function registerNameXID(string _nameString, uint256 _affCode, bool _all)
            // 0x921dec21
            CRegisterNameXID *a = new CRegisterNameXID;
            a->CTransaction::operator=(*p);
            a->_nameString = extract(params, 0*64, 64);
            a->_affCode = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_all = str_2_Int(extract(params, 2*64, 64));
            items[nItems++] = "string";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("registerNameXID", params, nItems, items);
            return a;

        } else if (encoding == func_registerNameXaddr) {
            // function registerNameXaddr(string _nameString, address _affCode, bool _all)
            // 0x3ddd4698
            CRegisterNameXaddr *a = new CRegisterNameXaddr;
            a->CTransaction::operator=(*p);
            a->_nameString = extract(params, 0*64, 64);
            a->_affCode = str_2_Addr(extract(params, 1*64, 64));
            a->_all = str_2_Int(extract(params, 2*64, 64));
            items[nItems++] = "string";
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("registerNameXaddr", params, nItems, items);
            return a;

        } else if (encoding == func_registerNameXname) {
            // function registerNameXname(string _nameString, bytes32 _affCode, bool _all)
            // 0x685ffd83
            CRegisterNameXname *a = new CRegisterNameXname;
            a->CTransaction::operator=(*p);
            a->_nameString = extract(params, 0*64, 64);
            a->_affCode = extract(params, 1*64, 64);
            a->_all = str_2_Int(extract(params, 2*64, 64));
            items[nItems++] = "string";
            items[nItems++] = "bytes32";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("registerNameXname", params, nItems, items);
            return a;

        } else if (encoding == func_setOtherFomo) {
            // function setOtherFomo(address _otherF3D)
            // 0xb483c054
            CSetOtherFomo *a = new CSetOtherFomo;
            a->CTransaction::operator=(*p);
            a->_otherF3D = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setOtherFomo", params, nItems, items);
            return a;

        } else if (encoding == func_setup) {
            // function setup(address _firstCorpBank)
            // 0x66d38203
            CSetup *a = new CSetup;
            a->CTransaction::operator=(*p);
            a->_firstCorpBank = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setup", params, nItems, items);
            return a;

        } else if (encoding == func_startMigration) {
            // function startMigration(address _newCorpBank)
            // 0xa0f52da0
            CStartMigration *a = new CStartMigration;
            a->CTransaction::operator=(*p);
            a->_newCorpBank = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("startMigration", params, nItems, items);
            return a;

        } else if (encoding == func_withdraw) {
            // function withdraw()
            // 0x3ccfd60b
            CWithdraw *a = new CWithdraw;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdraw", params, nItems, items);
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
const string_q evt_onAffiliatePayout = "0x590bbc0fc16915a85269a48f74783c39842b7ae9eceb7c295c95dbe8b3ec7331";
const string_q evt_onBuyAndDistribute = "0xa7801a70b37e729a11492aad44fd3dba89b4149f0609dc0f6837bf9e57e2671a";
const string_q evt_onEndTx = "0x500e72a0e114930aebdbcb371ccdbf43922c49f979794b5de4257ff7e310c746";
const string_q evt_onNewName = "0xdd6176433ff5026bbce96b068584b7bbe3514227e72df9c630b749ae87e64442";
const string_q evt_onPotSwapDeposit = "0x74b1d2f771e0eff1b2c36c38499febdbea80fe4013bdace4fc4b653322c2895c";
const string_q evt_onReLoadAndDistribute = "0x88261ac70d02d5ea73e54fa6da17043c974de1021109573ec1f6f57111c823dd";
const string_q evt_onWithdraw = "0x8f36579a548bc439baa172a6521207464154da77f411e2da3db2f53affe6cc3a";
const string_q evt_onWithdrawAndDistribute = "0x0bd0dba8ab932212fa78150cdb7b0275da72e255875967b5cad11464cf71bedc";

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

        if (topic_2_Str(p->topics[0]) % evt_onAffiliatePayout) {
            // event onAffiliatePayout(uint256 indexed affiliateID, address affiliateAddress, bytes32 affiliateName, uint256 indexed roundID, uint256 indexed buyerID, uint256 amount, uint256 timeStamp)
            // 0x590bbc0fc16915a85269a48f74783c39842b7ae9eceb7c295c95dbe8b3ec7331
            COnAffiliatePayoutEvent *a = new COnAffiliatePayoutEvent;
            a->CLogEntry::operator=(*p);
            a->affiliateID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->affiliateAddress = str_2_Addr(extract(data, 0*64, 64));
            a->affiliateName = "0x" + extract(data, 1*64, 64);
            a->roundID = str_2_Wei(nTops > 4 ? topic_2_Str(p->topics[4]) : "");
            a->buyerID = str_2_Wei(nTops > 5 ? topic_2_Str(p->topics[5]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->timeStamp = str_2_Wei("0x" + extract(data, 3*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onAffiliatePayout", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onBuyAndDistribute) {
            // event onBuyAndDistribute(address playerAddress, bytes32 playerName, uint256 ethIn, uint256 compressedData, uint256 compressedIDs, address winnerAddr, bytes32 winnerName, uint256 amountWon, uint256 newPot, uint256 P3DAmount, uint256 genAmount)
            // 0xa7801a70b37e729a11492aad44fd3dba89b4149f0609dc0f6837bf9e57e2671a
            COnBuyAndDistributeEvent *a = new COnBuyAndDistributeEvent;
            a->CLogEntry::operator=(*p);
            a->playerAddress = str_2_Addr(extract(data, 0*64, 64));
            a->playerName = "0x" + extract(data, 1*64, 64);
            a->ethIn = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->compressedData = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->compressedIDs = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->winnerAddr = str_2_Addr(extract(data, 5*64, 64));
            a->winnerName = "0x" + extract(data, 6*64, 64);
            a->amountWon = str_2_Wei("0x" + extract(data, 7*64, 64));
            a->newPot = str_2_Wei("0x" + extract(data, 8*64, 64));
            a->P3DAmount = str_2_Wei("0x" + extract(data, 9*64, 64));
            a->genAmount = str_2_Wei("0x" + extract(data, 10*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onBuyAndDistribute", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onEndTx) {
            // event onEndTx(uint256 compressedData, uint256 compressedIDs, bytes32 playerName, address playerAddress, uint256 ethIn, uint256 keysBought, address winnerAddr, bytes32 winnerName, uint256 amountWon, uint256 newPot, uint256 P3DAmount, uint256 genAmount, uint256 potAmount, uint256 airDropPot)
            // 0x500e72a0e114930aebdbcb371ccdbf43922c49f979794b5de4257ff7e310c746
            COnEndTxEvent *a = new COnEndTxEvent;
            a->CLogEntry::operator=(*p);
            a->compressedData = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->compressedIDs = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->playerName = "0x" + extract(data, 2*64, 64);
            a->playerAddress = str_2_Addr(extract(data, 3*64, 64));
            a->ethIn = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->keysBought = str_2_Wei("0x" + extract(data, 5*64, 64));
            a->winnerAddr = str_2_Addr(extract(data, 6*64, 64));
            a->winnerName = "0x" + extract(data, 7*64, 64);
            a->amountWon = str_2_Wei("0x" + extract(data, 8*64, 64));
            a->newPot = str_2_Wei("0x" + extract(data, 9*64, 64));
            a->P3DAmount = str_2_Wei("0x" + extract(data, 10*64, 64));
            a->genAmount = str_2_Wei("0x" + extract(data, 11*64, 64));
            a->potAmount = str_2_Wei("0x" + extract(data, 12*64, 64));
            a->airDropPot = str_2_Wei("0x" + extract(data, 13*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onEndTx", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onNewName) {
            // event onNewName(uint256 indexed playerID, address indexed playerAddress, bytes32 indexed playerName, bool isNewPlayer, uint256 affiliateID, address affiliateAddress, bytes32 affiliateName, uint256 amountPaid, uint256 timeStamp)
            // 0xdd6176433ff5026bbce96b068584b7bbe3514227e72df9c630b749ae87e64442
            COnNewNameEvent *a = new COnNewNameEvent;
            a->CLogEntry::operator=(*p);
            a->playerID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->playerAddress = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->playerName = nTops > 3 ? topic_2_Str(p->topics[3]) : "";
            a->isNewPlayer = str_2_Int("0x" + extract(data, 0*64, 64));
            a->affiliateID = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->affiliateAddress = str_2_Addr(extract(data, 2*64, 64));
            a->affiliateName = "0x" + extract(data, 3*64, 64);
            a->amountPaid = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->timeStamp = str_2_Wei("0x" + extract(data, 5*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "bool";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onNewName", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onPotSwapDeposit) {
            // event onPotSwapDeposit(uint256 roundID, uint256 amountAddedToPot)
            // 0x74b1d2f771e0eff1b2c36c38499febdbea80fe4013bdace4fc4b653322c2895c
            COnPotSwapDepositEvent *a = new COnPotSwapDepositEvent;
            a->CLogEntry::operator=(*p);
            a->roundID = str_2_Wei("0x" + extract(data, 0*64, 64));
            a->amountAddedToPot = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onPotSwapDeposit", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onReLoadAndDistribute) {
            // event onReLoadAndDistribute(address playerAddress, bytes32 playerName, uint256 compressedData, uint256 compressedIDs, address winnerAddr, bytes32 winnerName, uint256 amountWon, uint256 newPot, uint256 P3DAmount, uint256 genAmount)
            // 0x88261ac70d02d5ea73e54fa6da17043c974de1021109573ec1f6f57111c823dd
            COnReLoadAndDistributeEvent *a = new COnReLoadAndDistributeEvent;
            a->CLogEntry::operator=(*p);
            a->playerAddress = str_2_Addr(extract(data, 0*64, 64));
            a->playerName = "0x" + extract(data, 1*64, 64);
            a->compressedData = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->compressedIDs = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->winnerAddr = str_2_Addr(extract(data, 4*64, 64));
            a->winnerName = "0x" + extract(data, 5*64, 64);
            a->amountWon = str_2_Wei("0x" + extract(data, 6*64, 64));
            a->newPot = str_2_Wei("0x" + extract(data, 7*64, 64));
            a->P3DAmount = str_2_Wei("0x" + extract(data, 8*64, 64));
            a->genAmount = str_2_Wei("0x" + extract(data, 9*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onReLoadAndDistribute", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onWithdraw) {
            // event onWithdraw(uint256 indexed playerID, address playerAddress, bytes32 playerName, uint256 ethOut, uint256 timeStamp)
            // 0x8f36579a548bc439baa172a6521207464154da77f411e2da3db2f53affe6cc3a
            COnWithdrawEvent *a = new COnWithdrawEvent;
            a->CLogEntry::operator=(*p);
            a->playerID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->playerAddress = str_2_Addr(extract(data, 0*64, 64));
            a->playerName = "0x" + extract(data, 1*64, 64);
            a->ethOut = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->timeStamp = str_2_Wei("0x" + extract(data, 3*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onWithdraw", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_onWithdrawAndDistribute) {
            // event onWithdrawAndDistribute(address playerAddress, bytes32 playerName, uint256 ethOut, uint256 compressedData, uint256 compressedIDs, address winnerAddr, bytes32 winnerName, uint256 amountWon, uint256 newPot, uint256 P3DAmount, uint256 genAmount)
            // 0x0bd0dba8ab932212fa78150cdb7b0275da72e255875967b5cad11464cf71bedc
            COnWithdrawAndDistributeEvent *a = new COnWithdrawAndDistributeEvent;
            a->CLogEntry::operator=(*p);
            a->playerAddress = str_2_Addr(extract(data, 0*64, 64));
            a->playerName = "0x" + extract(data, 1*64, 64);
            a->ethOut = str_2_Wei("0x" + extract(data, 2*64, 64));
            a->compressedData = str_2_Wei("0x" + extract(data, 3*64, 64));
            a->compressedIDs = str_2_Wei("0x" + extract(data, 4*64, 64));
            a->winnerAddr = str_2_Addr(extract(data, 5*64, 64));
            a->winnerName = "0x" + extract(data, 6*64, 64);
            a->amountWon = str_2_Wei("0x" + extract(data, 7*64, 64));
            a->newPot = str_2_Wei("0x" + extract(data, 8*64, 64));
            a->P3DAmount = str_2_Wei("0x" + extract(data, 9*64, 64));
            a->genAmount = str_2_Wei("0x" + extract(data, 10*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("onWithdrawAndDistribute", params, nItems, items);
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
    func_activate,
    func_activated,
    func_airDropPot,
    func_airDropTracker,
    func_buyXaddr,
    func_buyXid,
    func_buyXname,
    func_calcKeysReceived,
    func_cancelMigration,
    func_deposit,
    func_fees,
    func_finishMigration,
    func_getBuyPrice,
    func_getCurrentRoundInfo,
    func_getPlayerInfoByAddress,
    func_getPlayerVaults,
    func_getTimeLeft,
    func_iWantXKeys,
    func_pIDxAddr,
    func_pIDxName,
    func_plyr,
    func_plyrNames,
    func_plyrRnds,
    func_potSplit,
    func_potSwap,
    func_rID,
    func_reLoadXaddr,
    func_reLoadXid,
    func_reLoadXname,
    func_receivePlayerInfo,
    func_receivePlayerNameList,
    func_registerNameXID,
    func_registerNameXaddr,
    func_registerNameXname,
    func_rndTmEth,
    func_round,
    func_setOtherFomo,
    func_setup,
    func_startMigration,
    func_status,
    func_withdraw,
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
    evt_onAffiliatePayout,
    evt_onBuyAndDistribute,
    evt_onEndTx,
    evt_onNewName,
    evt_onPotSwapDeposit,
    evt_onReLoadAndDistribute,
    evt_onWithdraw,
    evt_onWithdrawAndDistribute,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xf9ba0955b0509ac6138908ccc50d5bd296e48d7d
{"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

ABI for addr : 0x4c7b8591c50f4ad308d07d6294f2945e074420f5
{"status":"0","message":"NOTOK","result":"Contractsourcecodenotverified"}

ABI for addr : 0xa62142888aba8370742be823c1782d17a0389da1
[{"constant":true,"inputs":[],"name":"getBuyPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"bytes32"},{"name":"_team","type":"uint256"},{"name":"_eth","type":"uint256"}],"name":"reLoadXname","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"activate","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"pIDxAddr_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"airDropTracker_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"round_","outputs":[{"name":"plyr","type":"uint256"},{"name":"team","type":"uint256"},{"name":"end","type":"uint256"},{"name":"ended","type":"bool"},{"name":"strt","type":"uint256"},{"name":"keys","type":"uint256"},{"name":"eth","type":"uint256"},{"name":"pot","type":"uint256"},{"name":"mask","type":"uint256"},{"name":"ico","type":"uint256"},{"name":"icoGen","type":"uint256"},{"name":"icoAvg","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"},{"name":"","type":"bytes32"}],"name":"plyrNames_","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"fees_","outputs":[{"name":"gen","type":"uint256"},{"name":"p3d","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"bytes32"}],"name":"pIDxName_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"uint256"},{"name":"_team","type":"uint256"},{"name":"_eth","type":"uint256"}],"name":"reLoadXid","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"withdraw","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_nameString","type":"string"},{"name":"_affCode","type":"address"},{"name":"_all","type":"bool"}],"name":"registerNameXaddr","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[{"name":"_pID","type":"uint256"},{"name":"_addr","type":"address"},{"name":"_name","type":"bytes32"},{"name":"_laff","type":"uint256"}],"name":"receivePlayerInfo","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"name":"rndTmEth_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"rID_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_pID","type":"uint256"}],"name":"getPlayerVaults","outputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_nameString","type":"string"},{"name":"_affCode","type":"bytes32"},{"name":"_all","type":"bool"}],"name":"registerNameXname","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"getCurrentRoundInfo","outputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"address"},{"name":"","type":"bytes32"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"address"},{"name":"_team","type":"uint256"},{"name":"_eth","type":"uint256"}],"name":"reLoadXaddr","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"uint256"},{"name":"_team","type":"uint256"}],"name":"buyXid","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[{"name":"_pID","type":"uint256"},{"name":"_name","type":"bytes32"}],"name":"receivePlayerNameList","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_nameString","type":"string"},{"name":"_affCode","type":"uint256"},{"name":"_all","type":"bool"}],"name":"registerNameXID","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"address"},{"name":"_team","type":"uint256"}],"name":"buyXaddr","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"name":"plyrRnds_","outputs":[{"name":"eth","type":"uint256"},{"name":"keys","type":"uint256"},{"name":"mask","type":"uint256"},{"name":"ico","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_affCode","type":"bytes32"},{"name":"_team","type":"uint256"}],"name":"buyXname","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[{"name":"_otherF3D","type":"address"}],"name":"setOtherFomo","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"potSplit_","outputs":[{"name":"gen","type":"uint256"},{"name":"p3d","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"getTimeLeft","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_rID","type":"uint256"},{"name":"_eth","type":"uint256"}],"name":"calcKeysReceived","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_keys","type":"uint256"}],"name":"iWantXKeys","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"activated_","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"airDropPot_","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"plyr_","outputs":[{"name":"addr","type":"address"},{"name":"name","type":"bytes32"},{"name":"win","type":"uint256"},{"name":"gen","type":"uint256"},{"name":"aff","type":"uint256"},{"name":"lrnd","type":"uint256"},{"name":"laff","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"potSwap","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[{"name":"_addr","type":"address"}],"name":"getPlayerInfoByAddress","outputs":[{"name":"","type":"uint256"},{"name":"","type":"bytes32"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"playerID","type":"uint256"},{"indexed":true,"name":"playerAddress","type":"address"},{"indexed":true,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"isNewPlayer","type":"bool"},{"indexed":false,"name":"affiliateID","type":"uint256"},{"indexed":false,"name":"affiliateAddress","type":"address"},{"indexed":false,"name":"affiliateName","type":"bytes32"},{"indexed":false,"name":"amountPaid","type":"uint256"},{"indexed":false,"name":"timeStamp","type":"uint256"}],"name":"onNewName","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"compressedData","type":"uint256"},{"indexed":false,"name":"compressedIDs","type":"uint256"},{"indexed":false,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"playerAddress","type":"address"},{"indexed":false,"name":"ethIn","type":"uint256"},{"indexed":false,"name":"keysBought","type":"uint256"},{"indexed":false,"name":"winnerAddr","type":"address"},{"indexed":false,"name":"winnerName","type":"bytes32"},{"indexed":false,"name":"amountWon","type":"uint256"},{"indexed":false,"name":"newPot","type":"uint256"},{"indexed":false,"name":"P3DAmount","type":"uint256"},{"indexed":false,"name":"genAmount","type":"uint256"},{"indexed":false,"name":"potAmount","type":"uint256"},{"indexed":false,"name":"airDropPot","type":"uint256"}],"name":"onEndTx","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"playerID","type":"uint256"},{"indexed":false,"name":"playerAddress","type":"address"},{"indexed":false,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"ethOut","type":"uint256"},{"indexed":false,"name":"timeStamp","type":"uint256"}],"name":"onWithdraw","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"playerAddress","type":"address"},{"indexed":false,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"ethOut","type":"uint256"},{"indexed":false,"name":"compressedData","type":"uint256"},{"indexed":false,"name":"compressedIDs","type":"uint256"},{"indexed":false,"name":"winnerAddr","type":"address"},{"indexed":false,"name":"winnerName","type":"bytes32"},{"indexed":false,"name":"amountWon","type":"uint256"},{"indexed":false,"name":"newPot","type":"uint256"},{"indexed":false,"name":"P3DAmount","type":"uint256"},{"indexed":false,"name":"genAmount","type":"uint256"}],"name":"onWithdrawAndDistribute","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"playerAddress","type":"address"},{"indexed":false,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"ethIn","type":"uint256"},{"indexed":false,"name":"compressedData","type":"uint256"},{"indexed":false,"name":"compressedIDs","type":"uint256"},{"indexed":false,"name":"winnerAddr","type":"address"},{"indexed":false,"name":"winnerName","type":"bytes32"},{"indexed":false,"name":"amountWon","type":"uint256"},{"indexed":false,"name":"newPot","type":"uint256"},{"indexed":false,"name":"P3DAmount","type":"uint256"},{"indexed":false,"name":"genAmount","type":"uint256"}],"name":"onBuyAndDistribute","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"playerAddress","type":"address"},{"indexed":false,"name":"playerName","type":"bytes32"},{"indexed":false,"name":"compressedData","type":"uint256"},{"indexed":false,"name":"compressedIDs","type":"uint256"},{"indexed":false,"name":"winnerAddr","type":"address"},{"indexed":false,"name":"winnerName","type":"bytes32"},{"indexed":false,"name":"amountWon","type":"uint256"},{"indexed":false,"name":"newPot","type":"uint256"},{"indexed":false,"name":"P3DAmount","type":"uint256"},{"indexed":false,"name":"genAmount","type":"uint256"}],"name":"onReLoadAndDistribute","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"affiliateID","type":"uint256"},{"indexed":false,"name":"affiliateAddress","type":"address"},{"indexed":false,"name":"affiliateName","type":"bytes32"},{"indexed":true,"name":"roundID","type":"uint256"},{"indexed":true,"name":"buyerID","type":"uint256"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"timeStamp","type":"uint256"}],"name":"onAffiliatePayout","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"roundID","type":"uint256"},{"indexed":false,"name":"amountAddedToPot","type":"uint256"}],"name":"onPotSwapDeposit","type":"event"}]

ABI for addr : 0xdd4950f977ee28d2c132f1353d1595035db444ee
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"cancelMigration","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"status","outputs":[{"name":"","type":"address"},{"name":"","type":"address"},{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_firstCorpBank","type":"address"}],"name":"setup","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"finishMigration","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_newCorpBank","type":"address"}],"name":"startMigration","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[],"name":"deposit","outputs":[{"name":"","type":"bool"}],"payable":true,"stateMutability":"payable","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"}]

*/
