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
#include "acceptrequestevent.h"
#include "affirmstatusevent.h"
#include "burntokensevent.h"
#include "cancelrequestevent.h"
#include "makerequestevent.h"
#include "messageevent.h"
#include "minttokensevent.h"
#include "reclaimmisplacedtokensevent.h"
#include "settingchangeevent.h"
#include "toggletrustedverifierevent.h"
#include "transferxevent.h"
#include "verifystatusevent.h"
#include "withdrawpaymentevent.h"
#include "acceptexistingaskrequestandbuy.h"
#include "acceptexistingbidrequestandsell.h"
#include "affirmstatusasaccreditedinvestor.h"
#include "burntokens.h"
#include "cancelexistingrequest.h"
#include "changesetting.h"
#include "depositpayment.h"
#include "makenewaskrequesttosell.h"
#include "makenewbidrequesttobuy.h"
#include "minttokens.h"
#include "reclaimmisplacedtokens.h"
#include "setfiatcontractaddress.h"
#include "togglepause.h"
#include "toggletrustedverifier.h"
#include "transferanyerc20token.h"
#include "transferx.h"
#include "verifystatusasaccreditedinvestor.h"
#include "withdrawexcessether.h"
#include "withdrawpayment.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_acceptExistingAskRequestAndBUY;
extern const string_q func_acceptExistingBidRequestAndSELL;
extern const string_q func_admin;
extern const string_q func_affirmStatusAsAccreditedInvestor;
extern const string_q func_burnTokens;
extern const string_q func_cancelExistingRequest;
extern const string_q func_changeSetting;
extern const string_q func_contractAddress;
extern const string_q func_contractSetting;
extern const string_q func_depositPayment;
extern const string_q func_exchangeRate;
extern const string_q func_fiatContractAddress;
extern const string_q func_getHolderInfo;
extern const string_q func_getHolders;
extern const string_q func_getRequest;
extern const string_q func_getRequestKeys;
extern const string_q func_makeNewAskRequestToSELL;
extern const string_q func_makeNewBidRequestToBUY;
extern const string_q func_mintTokens;
extern const string_q func_pause;
extern const string_q func_reclaimMisplacedTokens;
extern const string_q func_setFiatContractAddress;
extern const string_q func_togglePause;
extern const string_q func_toggleTrustedVerifier;
extern const string_q func_totalEtherDepositsMade;
extern const string_q func_totalEtherPaymentsDue;
extern const string_q func_totalTokensCommitted;
extern const string_q func_transferAnyERC20Token;
extern const string_q func_transferX;
extern const string_q func_verifyStatusAsAccreditedInvestor;
extern const string_q func_withdrawExcessEther;
extern const string_q func_withdrawPayment;

//------------------------------------------------------------------------
extern const string_q evt_AcceptRequest;
extern const string_q evt_AffirmStatus;
extern const string_q evt_BurnTokens;
extern const string_q evt_CancelRequest;
extern const string_q evt_MakeRequest;
extern const string_q evt_Message;
extern const string_q evt_MintTokens;
extern const string_q evt_ReclaimMisplacedTokens;
extern const string_q evt_SettingChange;
extern const string_q evt_ToggleTrustedVerifier;
extern const string_q evt_TransferX;
extern const string_q evt_VerifyStatus;
extern const string_q evt_WithdrawPayment;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
