#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
 /*
  *
  * This code was generated automatically from grabABI and makeClass You may 
  * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "etherlib.h"
#include "tokenlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "activate.h"
#include "buyxaddr.h"
#include "buyxid.h"
#include "buyxname.h"
#include "cancelmigration.h"
#include "deposit.h"
#include "finishmigration.h"
#include "onaffiliatepayoutevent.h"
#include "onbuyanddistributeevent.h"
#include "onendtxevent.h"
#include "onnewnameevent.h"
#include "onpotswapdepositevent.h"
#include "onreloadanddistributeevent.h"
#include "onwithdrawevent.h"
#include "onwithdrawanddistributeevent.h"
#include "potswap.h"
#include "reloadxaddr.h"
#include "reloadxid.h"
#include "reloadxname.h"
#include "receiveplayerinfo.h"
#include "receiveplayernamelist.h"
#include "registernamexid.h"
#include "registernamexaddr.h"
#include "registernamexname.h"
#include "setotherfomo.h"
#include "setup.h"
#include "startmigration.h"
#include "withdraw.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_activate;
extern const string_q func_activated;
extern const string_q func_airDropPot;
extern const string_q func_airDropTracker;
extern const string_q func_buyXaddr;
extern const string_q func_buyXid;
extern const string_q func_buyXname;
extern const string_q func_calcKeysReceived;
extern const string_q func_cancelMigration;
extern const string_q func_deposit;
extern const string_q func_fees;
extern const string_q func_finishMigration;
extern const string_q func_getBuyPrice;
extern const string_q func_getCurrentRoundInfo;
extern const string_q func_getPlayerInfoByAddress;
extern const string_q func_getPlayerVaults;
extern const string_q func_getTimeLeft;
extern const string_q func_iWantXKeys;
extern const string_q func_pIDxAddr;
extern const string_q func_pIDxName;
extern const string_q func_plyr;
extern const string_q func_plyrNames;
extern const string_q func_plyrRnds;
extern const string_q func_potSplit;
extern const string_q func_potSwap;
extern const string_q func_rID;
extern const string_q func_reLoadXaddr;
extern const string_q func_reLoadXid;
extern const string_q func_reLoadXname;
extern const string_q func_receivePlayerInfo;
extern const string_q func_receivePlayerNameList;
extern const string_q func_registerNameXID;
extern const string_q func_registerNameXaddr;
extern const string_q func_registerNameXname;
extern const string_q func_rndTmEth;
extern const string_q func_round;
extern const string_q func_setOtherFomo;
extern const string_q func_setup;
extern const string_q func_startMigration;
extern const string_q func_status;
extern const string_q func_withdraw;

//------------------------------------------------------------------------
extern const string_q evt_onAffiliatePayout;
extern const string_q evt_onBuyAndDistribute;
extern const string_q evt_onEndTx;
extern const string_q evt_onNewName;
extern const string_q evt_onPotSwapDeposit;
extern const string_q evt_onReLoadAndDistribute;
extern const string_q evt_onWithdraw;
extern const string_q evt_onWithdrawAndDistribute;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
