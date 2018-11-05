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
#include "changeevent.h"
#include "contributionevent.h"
#include "destructionevent.h"
#include "issuanceevent.h"
#include "newsmarttokenevent.h"
#include "ownerupdateevent.h"
#include "acceptownership.h"
#include "accepttokenownership.h"
#include "addreserve.h"
#include "buy.h"
#include "buy77a7.h"
#include "change.h"
#include "contributebtcs.h"
#include "contributeeth.h"
#include "deposit.h"
#include "destroy.h"
#include "destroytokens.h"
#include "disablereservepurchases.h"
#include "disabletokentransfers.h"
#include "disabletransfers.h"
#include "enablerealcap.h"
#include "issue.h"
#include "issuetokens.h"
#include "sell.h"
#include "setformula.h"
#include "transferownership.h"
#include "transfertokenownership.h"
#include "updatereserve.h"
#include "withdraw.h"
#include "withdrawfromtoken.h"
#include "withdrawtokens.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_BTCSETHERCAP;
extern const string_q func_DURATION;
extern const string_q func_MAXGASPRICE;
extern const string_q func_TOKENPRICED;
extern const string_q func_TOKENPRICEN;
extern const string_q func_acceptOwnership;
extern const string_q func_acceptTokenOwnership;
extern const string_q func_addReserve;
extern const string_q func_beneficiary;
extern const string_q func_btcs;
extern const string_q func_buy;
extern const string_q func_buy77a7;
extern const string_q func_change;
extern const string_q func_changeableToken;
extern const string_q func_changeableTokenCount;
extern const string_q func_changerType;
extern const string_q func_computeRealCap;
extern const string_q func_computeReturn;
extern const string_q func_contributeBTCs;
extern const string_q func_contributeETH;
extern const string_q func_deposit;
extern const string_q func_destroy;
extern const string_q func_destroyTokens;
extern const string_q func_disableReservePurchases;
extern const string_q func_disableTokenTransfers;
extern const string_q func_disableTransfers;
extern const string_q func_enableRealCap;
extern const string_q func_endTime;
extern const string_q func_etherToken;
extern const string_q func_formula;
extern const string_q func_getPurchaseReturn;
extern const string_q func_getReserveBalance;
extern const string_q func_getReturn;
extern const string_q func_getSaleReturn;
extern const string_q func_issue;
extern const string_q func_issueTokens;
extern const string_q func_newOwner;
extern const string_q func_owner;
extern const string_q func_realEtherCapHash;
extern const string_q func_reserveTokenCount;
extern const string_q func_reserveTokens;
extern const string_q func_reserves;
extern const string_q func_sell;
extern const string_q func_setFormula;
extern const string_q func_standard;
extern const string_q func_startTime;
extern const string_q func_token;
extern const string_q func_tokenChanger;
extern const string_q func_totalEtherCap;
extern const string_q func_totalEtherContributed;
extern const string_q func_transferOwnership;
extern const string_q func_transferTokenOwnership;
extern const string_q func_transfersEnabled;
extern const string_q func_updateReserve;
extern const string_q func_withdraw;
extern const string_q func_withdrawFromToken;
extern const string_q func_withdrawTokens;

//------------------------------------------------------------------------
extern const string_q evt_Change;
extern const string_q evt_Contribution;
extern const string_q evt_Destruction;
extern const string_q evt_Issuance;
extern const string_q evt_NewSmartToken;
extern const string_q evt_OwnerUpdate;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
