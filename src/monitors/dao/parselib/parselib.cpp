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
    CAllowedRecipientChangedEvent::registerClass();
    CCreatedTokenEvent::registerClass();
    CFuelingToDateEvent::registerClass();
    CNewCuratorEvent::registerClass();
    CProposalAddedEvent::registerClass();
    CProposalTalliedEvent::registerClass();
    CRefundEvent::registerClass();
    CVotedEvent::registerClass();
    CChangeAllowedRecipients::registerClass();
    CChangeProposalDeposit::registerClass();
    CCreateTokenProxy::registerClass();
    CExecuteProposal::registerClass();
    CGetMyReward::registerClass();
    CHalveMinQuorum::registerClass();
    CNewContract::registerClass();
    CNewProposal::registerClass();
    CReceiveEther::registerClass();
    CRefund::registerClass();
    CRetrieveDAOReward::registerClass();
    CSplitDAO::registerClass();
    CTransferFromWithoutReward::registerClass();
    CTransferWithoutReward::registerClass();
    CTrusteeWithdraw::registerClass();
    CUnblockMe::registerClass();
    CVote::registerClass();
    CWithdraw::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_DAOpaidOut = "0x643f7cdd";
const string_q func_DAOrewardAccount = "0x82bf6464";
const string_q func_actualBalance = "0x39d1f908";
const string_q func_allowedRecipients = "0x4df6d6cc";
const string_q func_blocked = "0xe5962195";
const string_q func_changeAllowedRecipients = "0x749f9889";
const string_q func_changeProposalDeposit = "0xe33734fd";
const string_q func_checkProposalCode = "0xeceb2945";
const string_q func_closingTime = "0x4b6753bc";
const string_q func_createTokenProxy = "0xbaac5300";
const string_q func_curator = "0xe66f53b7";
const string_q func_daoCreator = "0x149acf9a";
const string_q func_divisor = "0x1f2dc5ef";
const string_q func_executeProposal = "0x237e9492";
const string_q func_extraBalance = "0x21b5b8dd";
const string_q func_getMyReward = "0xcc9ae3f6";
const string_q func_getNewDAOAddress = "0xbe7c29c1";
const string_q func_halveMinQuorum = "0x78524b2e";
const string_q func_isFueled = "0xb7bc2c84";
const string_q func_lastTimeMinQuorumMet = "0x96d7f3f5";
const string_q func_mainDAO = "0xeedcf50a";
const string_q func_minQuorumDivisor = "0x674ed066";
const string_q func_minTokensToCreate = "0x0c3b7b96";
const string_q func_newContract = "0x6837ff1e";
const string_q func_newProposal = "0x612e45a3";
const string_q func_numberOfProposals = "0x8d7af473";
const string_q func_paidOut = "0x81f03fcb";
const string_q func_privateCreation = "0xf8c80d26";
const string_q func_proposalDeposit = "0x8b15a605";
const string_q func_proposals = "0x013cf08b";
const string_q func_receiveEther = "0xa3912ec8";
const string_q func_refund = "0x590e1ae3";
const string_q func_retrieveDAOReward = "0xa1da2fb9";
const string_q func_rewardAccount = "0x0e708203";
const string_q func_rewardToken = "0xcdef91d0";
const string_q func_splitDAO = "0x82661dc4";
const string_q func_totalRewardToken = "0x34145808";
const string_q func_transferFromWithoutReward = "0xdbde1988";
const string_q func_transferWithoutReward = "0x4e10c3ee";
const string_q func_trustee = "0xfdf97cb2";
const string_q func_trusteeWithdraw = "0x2e6e504a";
const string_q func_unblockMe = "0x2632bf20";
const string_q func_vote = "0xc9d27afe";
const string_q func_withdraw = "0x3ccfd60b";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        if (encoding == "0x") {
            encoding = func_createTokenProxy;
            params = padLeft(extract(p->from, 2), 64, '0');
            ((CTransaction*)p)->input = func_createTokenProxy + params;
        }
        // EXISTING_CODE

        if (encoding == func_changeAllowedRecipients) {
            // function changeAllowedRecipients(address _recipient, bool _allowed)
            // 0x749f9889
            CChangeAllowedRecipients *a = new CChangeAllowedRecipients;
            a->CTransaction::operator=(*p);
            a->_recipient = str_2_Addr(extract(params, 0*64, 64));
            a->_allowed = str_2_Int(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("changeAllowedRecipients", params, nItems, items);
            return a;

        } else if (encoding == func_changeProposalDeposit) {
            // function changeProposalDeposit(uint256 _proposalDeposit)
            // 0xe33734fd
            CChangeProposalDeposit *a = new CChangeProposalDeposit;
            a->CTransaction::operator=(*p);
            a->_proposalDeposit = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changeProposalDeposit", params, nItems, items);
            return a;

        } else if (encoding == func_createTokenProxy) {
            // function createTokenProxy(address _tokenHolder)
            // 0xbaac5300
            CCreateTokenProxy *a = new CCreateTokenProxy;
            a->CTransaction::operator=(*p);
            a->_tokenHolder = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("createTokenProxy", params, nItems, items);
            return a;

        } else if (encoding == func_executeProposal) {
            // function executeProposal(uint256 _proposalID, bytes _transactionData)
            // 0x237e9492
            CExecuteProposal *a = new CExecuteProposal;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_transactionData = extract(params, 1*64);
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->articulatedTx = decodeRLP("executeProposal", params, nItems, items);
            return a;

        } else if (encoding == func_getMyReward) {
            // function getMyReward()
            // 0xcc9ae3f6
            CGetMyReward *a = new CGetMyReward;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("getMyReward", params, nItems, items);
            return a;

        } else if (encoding == func_halveMinQuorum) {
            // function halveMinQuorum()
            // 0x78524b2e
            CHalveMinQuorum *a = new CHalveMinQuorum;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("halveMinQuorum", params, nItems, items);
            return a;

        } else if (encoding == func_newContract) {
            // function newContract(address _newContract)
            // 0x6837ff1e
            CNewContract *a = new CNewContract;
            a->CTransaction::operator=(*p);
            a->_newContract = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("newContract", params, nItems, items);
            return a;

        } else if (encoding == func_newProposal) {
            // function newProposal(address _recipient, uint256 _amount, string _description, bytes _transactionData, uint256 _debatingPeriod, bool _newCurator)
            // 0x612e45a3
            CNewProposal *a = new CNewProposal;
            a->CTransaction::operator=(*p);
            a->_recipient = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_description = extract(params, 2*64, 64);
            a->_transactionData = extract(params, 3*64);
            a->_debatingPeriod = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->_newCurator = str_2_Int(extract(params, 5*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            items[nItems++] = "bytes";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("newProposal", params, nItems, items);
            return a;

        } else if (encoding == func_receiveEther) {
            // function receiveEther()
            // 0xa3912ec8
            CReceiveEther *a = new CReceiveEther;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("receiveEther", params, nItems, items);
            return a;

        } else if (encoding == func_refund) {
            // function refund()
            // 0x590e1ae3
            CRefund *a = new CRefund;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("refund", params, nItems, items);
            return a;

        } else if (encoding == func_retrieveDAOReward) {
            // function retrieveDAOReward(bool _toMembers)
            // 0xa1da2fb9
            CRetrieveDAOReward *a = new CRetrieveDAOReward;
            a->CTransaction::operator=(*p);
            a->_toMembers = str_2_Int(extract(params, 0*64, 64));
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("retrieveDAOReward", params, nItems, items);
            return a;

        } else if (encoding == func_splitDAO) {
            // function splitDAO(uint256 _proposalID, address _newCurator)
            // 0x82661dc4
            CSplitDAO *a = new CSplitDAO;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_newCurator = str_2_Addr(extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("splitDAO", params, nItems, items);
            return a;

        } else if (encoding == func_transferFromWithoutReward) {
            // function transferFromWithoutReward(address _from, address _to, uint256 _value)
            // 0xdbde1988
            CTransferFromWithoutReward *a = new CTransferFromWithoutReward;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("transferFromWithoutReward", params, nItems, items);
            return a;

        } else if (encoding == func_transferWithoutReward) {
            // function transferWithoutReward(address _to, uint256 _value)
            // 0x4e10c3ee
            CTransferWithoutReward *a = new CTransferWithoutReward;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("transferWithoutReward", params, nItems, items);
            return a;

        } else if (encoding == func_trusteeWithdraw) {
            // function trusteeWithdraw()
            // 0x2e6e504a
            CTrusteeWithdraw *a = new CTrusteeWithdraw;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("trusteeWithdraw", params, nItems, items);
            return a;

        } else if (encoding == func_unblockMe) {
            // function unblockMe()
            // 0x2632bf20
            CUnblockMe *a = new CUnblockMe;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("unblockMe", params, nItems, items);
            return a;

        } else if (encoding == func_vote) {
            // function vote(uint256 _proposalID, bool _supportsProposal)
            // 0xc9d27afe
            CVote *a = new CVote;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_supportsProposal = str_2_Int(extract(params, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            a->articulatedTx = decodeRLP("vote", params, nItems, items);
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
const string_q evt_AllowedRecipientChanged = "0x73ad2a153c8b67991df9459024950b318a609782cee8c7eeda47b905f9baa91f";
const string_q evt_CreatedToken = "0xdbccb92686efceafb9bb7e0394df7f58f71b954061b81afb57109bf247d3d75a";
const string_q evt_FuelingToDate = "0xf381a3e2428fdda36615919e8d9c35878d9eb0cf85ac6edf575088e80e4c147e";
const string_q evt_NewCurator = "0x9046fefd66f538ab35263248a44217dcb70e2eb2cd136629e141b8b8f9f03b60";
const string_q evt_ProposalAdded = "0x5790de2c279e58269b93b12828f56fd5f2bc8ad15e61ce08572585c81a38756f";
const string_q evt_ProposalTallied = "0xdfc78bdca8e3e0b18c16c5c99323c6cb9eb5e00afde190b4e7273f5158702b07";
const string_q evt_Refund = "0xbb28353e4598c3b9199101a66e0989549b659a59a54d2c27fbb183f1932c8e6d";
const string_q evt_Voted = "0x86abfce99b7dd908bec0169288797f85049ec73cbe046ed9de818fab3a497ae0";

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

        if (topic_2_Str(p->topics[0]) % evt_AllowedRecipientChanged) {
            // event AllowedRecipientChanged(address indexed _recipient, bool _allowed)
            // 0x73ad2a153c8b67991df9459024950b318a609782cee8c7eeda47b905f9baa91f
            CAllowedRecipientChangedEvent *a = new CAllowedRecipientChangedEvent;
            a->CLogEntry::operator=(*p);
            a->_recipient = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_allowed = str_2_Int("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->articulatedLog = decodeRLP("AllowedRecipientChanged", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_CreatedToken) {
            // event CreatedToken(address indexed to, uint256 amount)
            // 0xdbccb92686efceafb9bb7e0394df7f58f71b954061b81afb57109bf247d3d75a
            CCreatedTokenEvent *a = new CCreatedTokenEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("CreatedToken", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_FuelingToDate) {
            // event FuelingToDate(uint256 value)
            // 0xf381a3e2428fdda36615919e8d9c35878d9eb0cf85ac6edf575088e80e4c147e
            CFuelingToDateEvent *a = new CFuelingToDateEvent;
            a->CLogEntry::operator=(*p);
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("FuelingToDate", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewCurator) {
            // event NewCurator(address indexed _newCurator)
            // 0x9046fefd66f538ab35263248a44217dcb70e2eb2cd136629e141b8b8f9f03b60
            CNewCuratorEvent *a = new CNewCuratorEvent;
            a->CLogEntry::operator=(*p);
            a->_newCurator = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("NewCurator", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalAdded) {
            // event ProposalAdded(uint256 indexed proposalID, address recipient, uint256 amount, bool newCurator, string description)
            // 0x5790de2c279e58269b93b12828f56fd5f2bc8ad15e61ce08572585c81a38756f
            CProposalAddedEvent *a = new CProposalAddedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->recipient = str_2_Addr(extract(data, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->newCurator = str_2_Int("0x" + extract(data, 2*64, 64));
            a->description = "0x" + extract(data, 3*64, 64);
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            items[nItems++] = "string";
            a->articulatedLog = decodeRLP("ProposalAdded", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalTallied) {
            // event ProposalTallied(uint256 indexed proposalID, bool result, uint256 quorum)
            // 0xdfc78bdca8e3e0b18c16c5c99323c6cb9eb5e00afde190b4e7273f5158702b07
            CProposalTalliedEvent *a = new CProposalTalliedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->result = str_2_Int("0x" + extract(data, 0*64, 64));
            a->quorum = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ProposalTallied", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Refund) {
            // event Refund(address indexed to, uint256 value)
            // 0xbb28353e4598c3b9199101a66e0989549b659a59a54d2c27fbb183f1932c8e6d
            CRefundEvent *a = new CRefundEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Refund", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Voted) {
            // event Voted(uint256 indexed proposalID, bool position, address indexed voter)
            // 0x86abfce99b7dd908bec0169288797f85049ec73cbe046ed9de818fab3a497ae0
            CVotedEvent *a = new CVotedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->position = str_2_Int("0x" + extract(data, 0*64, 64));
            a->voter = str_2_Addr(nTops > 3 ? topic_2_Str(p->topics[3]) : "");
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Voted", params, nItems, items);
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
    func_DAOpaidOut,
    func_DAOrewardAccount,
    func_actualBalance,
    func_allowedRecipients,
    func_blocked,
    func_changeAllowedRecipients,
    func_changeProposalDeposit,
    func_checkProposalCode,
    func_closingTime,
    func_createTokenProxy,
    func_curator,
    func_daoCreator,
    func_divisor,
    func_executeProposal,
    func_extraBalance,
    func_getMyReward,
    func_getNewDAOAddress,
    func_halveMinQuorum,
    func_isFueled,
    func_lastTimeMinQuorumMet,
    func_mainDAO,
    func_minQuorumDivisor,
    func_minTokensToCreate,
    func_newContract,
    func_newProposal,
    func_numberOfProposals,
    func_paidOut,
    func_privateCreation,
    func_proposalDeposit,
    func_proposals,
    func_receiveEther,
    func_refund,
    func_retrieveDAOReward,
    func_rewardAccount,
    func_rewardToken,
    func_splitDAO,
    func_totalRewardToken,
    func_transferFromWithoutReward,
    func_transferWithoutReward,
    func_trustee,
    func_trusteeWithdraw,
    func_unblockMe,
    func_vote,
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
    evt_AllowedRecipientChanged,
    evt_CreatedToken,
    evt_FuelingToDate,
    evt_NewCurator,
    evt_ProposalAdded,
    evt_ProposalTallied,
    evt_Refund,
    evt_Voted,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xbb9bc244d798123fde783fcc1c72d3bb8c189413
[{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"proposals","outputs":[{"name":"recipient","type":"address"},{"name":"amount","type":"uint256"},{"name":"description","type":"string"},{"name":"votingDeadline","type":"uint256"},{"name":"open","type":"bool"},{"name":"proposalPassed","type":"bool"},{"name":"proposalHash","type":"bytes32"},{"name":"proposalDeposit","type":"uint256"},{"name":"newCurator","type":"bool"},{"name":"yea","type":"uint256"},{"name":"nay","type":"uint256"},{"name":"creator","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_amount","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"minTokensToCreate","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"rewardAccount","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"daoCreator","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"divisor","outputs":[{"name":"divisor","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"extraBalance","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"_proposalID","type":"uint256"},{"name":"_transactionData","type":"bytes"}],"name":"executeProposal","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"unblockMe","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalRewardToken","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"actualBalance","outputs":[{"name":"_actualBalance","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"closingTime","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"allowedRecipients","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferWithoutReward","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"refund","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"_recipient","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_description","type":"string"},{"name":"_transactionData","type":"bytes"},{"name":"_debatingPeriod","type":"uint256"},{"name":"_newCurator","type":"bool"}],"name":"newProposal","outputs":[{"name":"_proposalID","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"DAOpaidOut","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"minQuorumDivisor","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_newContract","type":"address"}],"name":"newContract","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_recipient","type":"address"},{"name":"_allowed","type":"bool"}],"name":"changeAllowedRecipients","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"halveMinQuorum","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"paidOut","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_proposalID","type":"uint256"},{"name":"_newCurator","type":"address"}],"name":"splitDAO","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"DAOrewardAccount","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"proposalDeposit","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"numberOfProposals","outputs":[{"name":"_numberOfProposals","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"lastTimeMinQuorumMet","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_toMembers","type":"bool"}],"name":"retrieveDAOReward","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[],"name":"receiveEther","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"isFueled","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_tokenHolder","type":"address"}],"name":"createTokenProxy","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_proposalID","type":"uint256"}],"name":"getNewDAOAddress","outputs":[{"name":"_newDAO","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"_proposalID","type":"uint256"},{"name":"_supportsProposal","type":"bool"}],"name":"vote","outputs":[{"name":"_voteID","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[],"name":"getMyReward","outputs":[{"name":"_success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"rewardToken","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFromWithoutReward","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_proposalDeposit","type":"uint256"}],"name":"changeProposalDeposit","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"blocked","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"curator","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[{"name":"_proposalID","type":"uint256"},{"name":"_recipient","type":"address"},{"name":"_amount","type":"uint256"},{"name":"_transactionData","type":"bytes"}],"name":"checkProposalCode","outputs":[{"name":"_codeChecksOut","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"privateCreation","outputs":[{"name":"","type":"address"}],"type":"function"},{"inputs":[{"name":"_curator","type":"address"},{"name":"_daoCreator","type":"address"},{"name":"_proposalDeposit","type":"uint256"},{"name":"_minTokensToCreate","type":"uint256"},{"name":"_closingTime","type":"uint256"},{"name":"_privateCreation","type":"address"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_amount","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"value","type":"uint256"}],"name":"FuelingToDate","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"CreatedToken","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Refund","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"recipient","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"newCurator","type":"bool"},{"indexed":false,"name":"description","type":"string"}],"name":"ProposalAdded","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"position","type":"bool"},{"indexed":true,"name":"voter","type":"address"}],"name":"Voted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"result","type":"bool"},{"indexed":false,"name":"quorum","type":"uint256"}],"name":"ProposalTallied","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_newCurator","type":"address"}],"name":"NewCurator","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_recipient","type":"address"},{"indexed":false,"name":"_allowed","type":"bool"}],"name":"AllowedRecipientChanged","type":"event"}]

ABI for addr : 0xbf4ed7b27f1d666546e30d74d50d173d20bca754
[{"constant":false,"inputs":[],"name":"trusteeWithdraw","outputs":[],"type":"function"},{"constant":false,"inputs":[],"name":"withdraw","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"mainDAO","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"trustee","outputs":[{"name":"","type":"address"}],"type":"function"}]

ABI for addr : 0xc0ee9db1a9e07ca63e4ff0d5fb6f86bf68d47b89
[]

ABI for addr : 0xf835a0247b0063c04ef22006ebe57c5f11977cc4
[]

*/
